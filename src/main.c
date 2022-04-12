#include "../includes/philo.h"

t_philo	*create_struct(int *args, t_mutexes *mutexes, int *deadman)
{
	t_philo		*philo;
	int			i;
	t_fd		*fistDead;

	i = -1;
	// deadman = 0;
	fistDead = (t_fd *)malloc(sizeof(t_fd));
	if (!fistDead)
	{
		end(MALLOC, args, NULL, NULL);
		return (NULL);
	}
	fistDead->num = -1;
	philo = (t_philo *)malloc(sizeof(t_philo) * args[0]);
	if (!philo)
	{
		free(fistDead);
		end(MALLOC, args, NULL, NULL);
		return (NULL);
	}
	// if (args[4] != -1)
	// {
	// 	args[4] = args[0] * args[4];
	// }
	while (++i < args[0])
	{
		// philo[i].startTime = 0;
		// philo[i].lastMeal = 0;
		philo[i].args = args;
		philo[i].num = i;
		philo[i].mutexes = *mutexes;
		philo[i].someoneDead = deadman;
		philo[i].firstDead = fistDead;
		// philo[i].needtoeat = NULL;
		// if (args[4] != -1)
		// 	philo[i].needtoeat = &args[4];
	}

	return (philo);
}

void	start(int *args)
{
	pthread_t	*t;
	t_mutexes	mutexes;
	t_philo		*philo;
	int			i;
	long int	time;
	int	pchel=0;
	int *deadman=&pchel;

	i = -1;
	t = (pthread_t *)malloc(sizeof(pthread_t) * args[0]);
	if (!t)
		return end(MALLOC, args, NULL, NULL);
	mutexes = init_mutexes(args);
	philo = create_struct(args, &mutexes, deadman);
	if (!philo)
	{
		dest_mutexes(mutexes, args);
		return end(MALLOC, args, NULL, t);
	}
	gettime(&time);
	while (++i < args[0])
	{
		philo[i].startTime = time;
		philo[i].lastMeal = time;
		if (pthread_create(&t[i], NULL, philosopher, &philo[i]))
		{
			dest_mutexes(mutexes, args);
			return end(THREAD, args, philo, t);
		}
	}
	i = -1;
	while (++i < args[0])
	{
		if (pthread_join(t[i], NULL))
			return end(JOIN, args, philo, t);
	}
	printf("%ld %d has died\n", philo[0].firstDead->time - philo[0].startTime, philo[0].firstDead->num + 1);
	printf("%ld %ld %ld\n", philo[0].lastMeal, philo[0].startTime, philo[0].firstDead->time);
	dest_mutexes(mutexes, args);
	return end(OK, args, philo, t);
}

int	main(int argc, char **argv)
{
	int	*args;

	args = validate(argc, argv);
	if (!args)
		return (0);
	start(args);
}

#include "../includes/philo.h"

t_philo	*create_struct(long int *args, t_mutexes *mutexes, int *fdnum, long int *fdtime)
{
	t_philo		*philo;
	int			i;
	t_fd		*fistDead;

	i = -1;
	fistDead = (t_fd *)malloc(sizeof(t_fd));
	if (!fistDead)
	{
		end(MALLOC, args, NULL, NULL);
		return (NULL);
	}
	fistDead->num = fdnum;
	fistDead->time = fdtime;
	philo = (t_philo *)malloc(sizeof(t_philo) * args[0]);
	if (!philo)
	{
		free(fistDead);
		end(MALLOC, args, NULL, NULL);
		return (NULL);
	}
	while (++i < args[0])
	{
		philo[i].args = args;
		philo[i].num = i;
		philo[i].mutexes = *mutexes;
		philo[i].firstDead = fistDead;
	}
	return (philo);
}

void	start(long int *args)
{
	pthread_t	*t;
	t_mutexes	mutexes;
	t_philo		*philo;
	int			i;
	int	fdnum=-1;
	int *fdn=&fdnum;
	long int	fdtime=0;
	long int *fdt=&fdtime;

	i = -1;
	t = (pthread_t *)malloc(sizeof(pthread_t) * args[0]);
	if (!t)
		return end(MALLOC, args, NULL, NULL);
	mutexes = init_mutexes(args);
	philo = create_struct(args, &mutexes, fdn, fdt);
	if (!philo)
	{
		dest_mutexes(mutexes, args);
		return end(MALLOC, args, NULL, t);
	}
	philo[0].startTime = gettime(NULL);
	while (++i < args[0])
	{
		philo[i].startTime = philo[0].startTime;
		philo[i].lastMeal = philo[0].startTime;
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
	if (fdnum != -1)
		printf("%ld %d has died\n", fdtime - philo[0].startTime, fdnum + 1);
	dest_mutexes(mutexes, args);
	return end(OK, args, philo, t);
}

int	main(int argc, char **argv)
{
	long int	*args;

	args = validate(argc, argv);
	if (!args)
		return (0);
	start(args);
}

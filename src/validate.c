#include "../includes/philo.h"

long int	*matoi(char **argv, long int *args)
{
	args[0] = ft_atoi(argv[1], 1);
	args[1] = ft_atoi(argv[2], 1);
	args[2] = ft_atoi(argv[3], 1);
	args[3] = ft_atoi(argv[4], 1);
	return (args);
}

long int	*check_digits(int argc, char **argv)
{
	long int	*args;

	args = (long int *)malloc(sizeof(long int) * argc);
	if (!args)
		return (NULL);
	args = matoi(argv, args);
	if (argc == 5)
		args[4] = -1;
	else
	{
		args[4] = ft_atoi(argv[5], 1);
		if (args[4] <= 0)
		{
			end(ARGS, args, NULL, NULL);
			return (NULL);
		}
	}
	if (args[0] <= 0 || args[1] <= 0 || args[2] <= 0 || args[3] <= 0)
	{
		end(ARGS, args, NULL, NULL);
		return (NULL);
	}
	return (args);
}

long int	*validate(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		end(ARGS, NULL, NULL, NULL);
		return (NULL);
	}
	return (check_digits(argc, argv));
}

void	dest_mutexes(t_mutexes mutexes, long int *args)
{
	int	i;

	i = -1;
	pthread_mutex_destroy(&mutexes.mutexprintf);
	pthread_mutex_destroy(&mutexes.mutexend);
	while (++i < args[0])
		pthread_mutex_destroy(&mutexes.forks[i]);
	if (mutexes.forks)
	{
		free(mutexes.forks);
		mutexes.forks = NULL;
	}
}

t_mutexes	init_mutexes(long int *args)
{
	t_mutexes	mutexes;
	int			i;

	i = -1;
	pthread_mutex_init(&mutexes.mutexprintf, NULL);
	pthread_mutex_init(&mutexes.mutexend, NULL);
	mutexes.forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* args[0]);
	if (!mutexes.forks)
		return (mutexes);
	while (++i < args[0])
		pthread_mutex_init(&mutexes.forks[i], NULL);
	return (mutexes);
}

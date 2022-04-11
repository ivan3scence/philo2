#include "../includes/philo.h"

int	*checkDigits(int argc, char **argv)
{
	int	*args;

	args = (int *)malloc(sizeof(int) * argc);
	if (!args)
	{
		end(MALLOC, NULL, NULL, NULL);
		return (NULL);
	}
	args[0] = ft_atoi(argv[1]);
	args[1] = ft_atoi(argv[2]);
	args[2] = ft_atoi(argv[3]);
	args[3] = ft_atoi(argv[4]);
	if (argc == 5)
		args[4] = -1;
	else
		args[4] = ft_atoi(argv[5]);
	if (!(args[0] && args[1] && args[2] && args[3] && args[4]))
	{
		end(ARGS, args, NULL, NULL);
		return (NULL);
	}
	return (args);
}

int	*validate(int argc, char **argv)
{

	if (argc < 5 || argc > 6)
	{
		end(ARGS, NULL, NULL, NULL);
		return (NULL);
	}
	return (checkDigits(argc, argv));
}

void	dest_mutexes(t_mutexes mutexes, int *args)
{
	int	i;

	i = -1;
	pthread_mutex_destroy(&mutexes.mutexPrintf);
	pthread_mutex_destroy(&mutexes.mutexHaveEaten);
	pthread_mutex_destroy(&mutexes.mutexEnd);
	while (++i < args[0])
		pthread_mutex_destroy(&mutexes.forks[i]);
	if (mutexes.forks)
	{
		free(mutexes.forks);
		mutexes.forks = NULL;
	}
	if (args)
	{
		free(args);
		args = NULL;
	}
}

// int	checkEnd(int *args)
// {
// 	int	i;
// 	int	full;

// 	while (1)
// 	{
// 		i = -1;
// 		while (++i < args->quant)
// 		{
// 			if (args->flags.someoneDied != 0)
// 			{
// 				printf("%ld %d died\n", -args->startTime +
// 						gettime(NULL), args->flags.someoneDied);
// 				return (-1);
// 			}
// 			if (args->flags.haveEaten[i] == 1)
// 				full++;
// 		}
// 		if (full == args->quant)
// 			return (1);
// 		full = 0;
// 	}
// }

t_mutexes	init_mutexes(int *args)
{
	t_mutexes	mutexes;
	int	i;

	i = -1;
	pthread_mutex_init(&mutexes.mutexPrintf, NULL);
	pthread_mutex_init(&mutexes.mutexHaveEaten, NULL);
	pthread_mutex_init(&mutexes.mutexEnd, NULL);
	mutexes.forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* args[0]);
	if (!mutexes.forks)
		exit(1);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	while (++i < args[0])
		pthread_mutex_init(&mutexes.forks[i], NULL);
	return (mutexes);
}
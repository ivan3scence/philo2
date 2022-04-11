#include "../includes/philo.h"

// void	finish(t_args *args, t_philo *philo)
// {
// 	if (args)
// 	{
// 		// free(args->flags.haveEaten);
// 		// args->flags.haveEaten = NULL;
// 		free(args);
// 		args = NULL;
// 	}
// 	if (philo)
// 	{
// 		if (philo->mutexes.forks)
// 		{
// 			free(philo->mutexes.forks);
// 			philo->mutexes.forks = NULL;
// 		}
// 		free(philo);
// 		philo = NULL;
// 	}
// } 

//void	*dead(t_philo *philo)
//{
//	//pthread_mutex_lock(&philo->mutexes.mutexPrintf);
//	//pthread_mutex_lock(&philo->mutexes.mutexEnd);
//	//printf("%ld %d died\n", pretty_time(philo, 0)
//			- philo->args->startTime, philo->num + 1);
//	//pthread_mutex_unlock(&philo->mutexes.mutexEnd);
//	//pthread_mutex_unlock(&philo->mutexes.mutexPrintf);
//	return ((void *)1);
//}



t_philo	*create_struct(int *args, t_mutexes *mutexes, int *deadman)
{
	t_philo		*philo;
	int			i;
	// int			deadman;

	i = -1;
	// deadman = 0;
	philo = (t_philo *)malloc(sizeof(t_philo) * args[0]);
	if (!philo)
		return (NULL);//!!!!!!!!!!!!!!!!
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
		return end(MALLOC, NULL, NULL, t);
	}
	gettime(&time);
	while (++i < args[0])
	{
		philo[i].startTime = time;
		philo[i].lastMeal = time;
		if (pthread_create(&t[i], NULL, philosopher, &philo[i]))
		{
			dest_mutexes(mutexes, args);
			return end(THREAD, NULL, philo, t);
		}
	}
	i = -1;
	while (++i < args[0])
	{
		if (pthread_join(t[i], NULL))
			return end(JOIN, args, philo, t);
	}
	// if (checkEnd(args) == -1)
	// {
	// 	while (++i < args[0])
	// 		pthread_detach(t[i]);
	// 	dest_mutexes(mutexes, args);
	// 	finish(args, NULL);
	// 	free(philo);
	// 	printf("umer\n");
	// 	return ;
	// }
	// while (++i < args->quant)
	// {
	// 	if (!pthread_join(t[i], NULL))
	// 			return ;
	// }
	dest_mutexes(mutexes, args);
	// finish(args, NULL);
	free(philo);
	philo = NULL;
	free(t);
	t = NULL;
}

int	main(int argc, char **argv)
{
	int	*args;

	args = validate(argc, argv);
	if (!args)
		return (0);
	start(args);
	//finish(args, NULL);
}

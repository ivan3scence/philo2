#include "../includes/philo.h"

void	*checkDeath(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	// usleep(1000 * philo->args[1]);
	pthread_mutex_lock(&philo->mutexes.mutexHaveEaten);
	while (philo->args[4] == -1 || philo->args[4] > philo->meals)
	{
		if (gettime(NULL) - philo->lastMeal > philo->args[1])
			break ;
		pthread_mutex_unlock(&philo->mutexes.mutexHaveEaten);
		// usleep(1000 * (philo->args[1] - (gettime(NULL)
		// 			- philo->lastMeal)));
		pthread_mutex_lock(&philo->mutexes.mutexHaveEaten);
	}
	if (gettime(NULL) - philo->lastMeal > philo->args[1])
	{
		// pthread_mutex_lock(&philo->mutexes.mutexEnd);
		++(*(philo->someoneDead));
		pthread_mutex_unlock(&philo->mutexes.mutexHaveEaten);
		// philo->args->flags.someoneDied = philo->num;
		//pthread_mutex_lock(&philo->mutexes.mutexPrintf);
		return ((void *)1);
	}
	// philo->args->flags.haveEaten[philo->num + 1] = 1;
	pthread_mutex_unlock(&philo->mutexes.mutexHaveEaten);
	return ((void *)2);
}

void	*philosopher(void	*a)
{
	t_philo	*philo;
	pthread_t	pdeath;
	pthread_t	plife;
	// int		retur;

	philo = (t_philo *)a;
	if (pthread_create(&pdeath, NULL, checkDeath, philo))
			return (NULL);
	if (pthread_create(&plife, NULL, philo_life, philo))
			return (NULL);
	// pthread_join(pdeath, (void *)&retur);
	// if (retur == 1)
	// 	pthread_detach(plife);
	// else
	pthread_join(plife, NULL);
	pthread_detach(pdeath);
	free(philo);
	// if (retur == 1)
		// return ((void *)1);
	return (NULL);
}

void	sleep_ph(t_philo *philo)
{
	// pthread_t	psleep;

	// if (pthread_create(&psleep, NULL, my_sleep, &philo->args[3]))
	// 	exit(1);
	pthread_mutex_lock(&philo->mutexes.mutexPrintf);
	printf("%ld %d is sleeping\n", -philo->startTime
			+ gettime(NULL), philo->num + 1);
	pthread_mutex_unlock(&philo->mutexes.mutexPrintf);
	usleep(philo->args[3] * 1000);
	// pthread_join(psleep, NULL);
	pthread_mutex_lock(&philo->mutexes.mutexPrintf);
	printf("%ld %d is thinking\n", -philo->startTime
			+ gettime(NULL), philo->num + 1);
	pthread_mutex_unlock(&philo->mutexes.mutexPrintf);
}

void	*philo_life(void *p)
{
	t_philo	*philo;
	int		meals;

	meals = 0;
	philo = (t_philo *)p;
	pthread_mutex_lock(&philo->mutexes.mutexEnd);
	// while (!philo->args->flags.someoneDied && (philo->args[4] == -1
	while (!*(philo->someoneDead) && (philo->args[4] == -1
				|| ++meals < philo->args[4]))
	{
		// pthread_mutex_unlock(&philo->mutexes.mutexEnd);
		take_forks(philo);
		eat(philo);
		put_forks(philo);
		sleep_ph(philo);
		// pthread_mutex_lock(&philo->mutexes.mutexEnd);
	}
	return (NULL);
}
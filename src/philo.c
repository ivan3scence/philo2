#include "../includes/philo.h"

void	*checkDeath(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	// usleep(1000 * philo->args[1]);
	// pthread_mutex_lock(&philo->mutexes.mutexHaveEaten);
	while (1)
	{
		pthread_mutex_lock(&philo->mutexes.mutexHaveEaten);
		if ((philo->lastMeal - philo->startTime) > philo->args[1])
		{
			pthread_mutex_lock(&philo->mutexes.mutexEnd);
			*(philo->someoneDead) = 1;
			if (*(philo->firstDead->num) == -1)
			{
				*(philo->firstDead->time) = gettime(NULL);
				*(philo->firstDead->num) = philo->num;
				pthread_mutex_lock(&philo->mutexes.mutexPrintf);
				printf("lastmeal %ld starttime %ld fd %ld\n", philo->lastMeal, philo->startTime, *(philo->firstDead->time));
				printf("ya ne poel %d %ld\n", *(philo->firstDead->num) + 1, *(philo->firstDead->time) - philo->startTime);
			}
			printf("lastmeal %ld starttime %ld fd %ld\n", philo->lastMeal, philo->startTime, *(philo->firstDead->time));
			printf("ya ne poel %d %ld\n", *(philo->firstDead->num) + 1, *(philo->firstDead->time) - philo->startTime);
			pthread_mutex_unlock(&philo->mutexes.mutexPrintf);
			pthread_mutex_unlock(&philo->mutexes.mutexEnd);
			pthread_mutex_unlock(&philo->mutexes.mutexHaveEaten);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->mutexes.mutexHaveEaten);
	}
	return (NULL);
}

void	*philosopher(void	*a)
{
	t_philo	*philo;
	// pthread_t	pdeath;
	pthread_t	plife;

	philo = (t_philo *)a;
	// retur = 0;
	if (philo->num % 2 == 1)
		usleep(100);
	// printf("jojo%d\n", *(philo->someoneDead));
	// if (pthread_create(&pdeath, NULL, checkDeath, philo))
	// 		return (NULL);
	if (pthread_create(&plife, NULL, philo_life, philo))
			return (NULL);
	// pthread_join(pdeath, (void *)&retur);
	// if (retur == 1)
	// 	pthread_detach(plife);
	// else
	pthread_join(plife, NULL);
	// pthread_detach(pdeath);
	if (philo->lastMeal == -1)
	{
		printf("%ld %d poelllllllllllllllllllllll\n", gettime(NULL) - philo->startTime, philo->num);
		return ((void *)2);
	}
	// printf("%ld %d has died\n", gettime(NULL) - philo->startTime, philo->num + 1);
	return ((void *)1);
}

void	sleep_ph(t_philo *philo)
{
	// pthread_t	psleep;

	// if (pthread_create(&psleep, NULL, my_sleep, &philo->args[3]))
	// 	exit(1);
	pthread_mutex_lock(&philo->mutexes.mutexPrintf);
	// pthread_mutex_lock(&philo->mutexes.mutexEnd);
	if (philo->lastMeal - philo->startTime > philo->args[0])
		return died(philo);
	printf("%ld %d is sleeping\n", -philo->startTime
			+ gettime(NULL), philo->num + 1);
	// pthread_mutex_unlock(&philo->mutexes.mutexEnd);
	pthread_mutex_unlock(&philo->mutexes.mutexPrintf);
	usleep(philo->args[3] * 1000);
	// pthread_join(psleep, NULL);
	pthread_mutex_lock(&philo->mutexes.mutexPrintf);
	// pthread_mutex_lock(&philo->mutexes.mutexEnd);
	if (philo->lastMeal - philo->startTime > philo->args[0])
		return died(philo);
	printf("%ld %d is thinking\n", -philo->startTime
			+ gettime(NULL), philo->num + 1);
	// pthread_mutex_unlock(&philo->mutexes.mutexEnd);
	pthread_mutex_unlock(&philo->mutexes.mutexPrintf);
}

void	died(t_philo *philo)
{
	*(philo->firstDead->num) = philo->num;
	*(philo->firstDead->time) = gettime(NULL);
	return (NULL);
}

void	*philo_life(void *p)
{
	t_philo	*philo;
	int		meals;

	meals = 0;
	philo = (t_philo *)p;
	// pthread_mutex_lock(&philo->mutexes.mutexHaveEaten);
	philo->startTime = gettime(&(philo->lastMeal));
	// pthread_mutex_unlock(&philo->mutexes.mutexHaveEaten);
	// pthread_mutex_lock(&philo->mutexes.mutexEnd);
	// printf("jkk%d\n", *(philo->someoneDead));
	// while (!philo->args->flags.someoneDied && (philo->args[4] == -1
	while (*(philo->someoneDead) == 0 && (philo->args[4] == -1
				|| meals++ < philo->args[4]))
	{
		// printf("pop%d %d %d args\n", philo->num, meals, philo->args[4]);
		// pthread_mutex_unlock(&philo->mutexes.mutexEnd);
		take_forks(philo);
		eat(philo);
		put_forks(philo);
		sleep_ph(philo);
		// pthread_mutex_lock(&philo->mutexes.mutexEnd);
		// printf("%d %d\n", *(philo->someoneDead), meals);
		if (philo->lastMeal - philo->startTime > philo->args[0])
			return (NULL);
	}
	if (meals >= philo->args[4] && philo->args[4] != -1)
		philo->lastMeal = -1;
	return (NULL);
}
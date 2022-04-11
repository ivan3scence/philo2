#include "../includes/philo.h"

void	*checkDeath(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	// usleep(1000 * philo->args[1]);
	pthread_mutex_lock(&philo->mutexes.mutexHaveEaten);
	while (philo->args[4] == -1 || philo->args[4] > philo->meals)
	{
		if ((philo->lastMeal - philo->startTime) / 1000 > philo->args[1] / 1000)
		{
			*(philo->someoneDead) = 1;
			printf("%dya ne poel %d %ld\n", philo->num, philo->num, philo->lastMeal - philo->startTime);
			pthread_mutex_unlock(&philo->mutexes.mutexHaveEaten);
			return ((void *)1);
		}
		pthread_mutex_unlock(&philo->mutexes.mutexHaveEaten);
		pthread_mutex_lock(&philo->mutexes.mutexHaveEaten);
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
	int		retur;
	int		retur1;

	philo = (t_philo *)a;
	retur = 0;
	if (philo->num % 2 == 1)
		usleep(100);
	// printf("jojo%d\n", *(philo->someoneDead));
	if (pthread_create(&pdeath, NULL, checkDeath, philo))
			return (NULL);
	if (pthread_create(&plife, NULL, philo_life, philo))
			return (NULL);
	// pthread_join(pdeath, (void *)&retur);
	// if (retur == 1)
	// 	pthread_detach(plife);
	// else
	pthread_join(plife, (void *)&retur1);
	if (retur1 == 1)
	{
		pthread_detach(pdeath);
		printf("%ld %d poelllllllllllllllllllllll\n", gettime(NULL) - philo->startTime, philo->num);
		return ((void *)2);
	}
	pthread_join(pdeath, (void *)&retur);
	if (retur == 1)
	{
		printf("%ld %d has died\n", gettime(NULL) - philo->startTime, philo->num + 1);
		return ((void *)1);
	}
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
	pthread_mutex_lock(&philo->mutexes.mutexHaveEaten);
	philo->startTime = gettime(&(philo->lastMeal));
	pthread_mutex_unlock(&philo->mutexes.mutexHaveEaten);
	// pthread_mutex_lock(&philo->mutexes.mutexEnd);
	// printf("jkk%d\n", *(philo->someoneDead));
	// while (!philo->args->flags.someoneDied && (philo->args[4] == -1
	while (*(philo->someoneDead) == 0 && (philo->args[4] == -1
				|| ++meals < philo->args[4]))
	{
		printf("pop%d %d %d args\n", philo->num, meals, philo->args[4]);
		// pthread_mutex_unlock(&philo->mutexes.mutexEnd);
		take_forks(philo);
		eat(philo);
		put_forks(philo);
		sleep_ph(philo);
		// pthread_mutex_lock(&philo->mutexes.mutexEnd);
		printf("%d\n", *(philo->someoneDead));
	}
	return (NULL);
}
#include "../includes/philo.h"

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutexes.forks[philo->num]);
	pthread_mutex_lock(&philo->mutexes.mutexPrintf);
	// pthread_mutex_lock(&philo->mutexes.mutexEnd);
	if (philo->lastMeal - philo->startTime > philo->args[0])
		return died(philo);
	printf("%ld %d has taken a fork\n", -philo->startTime
			+ gettime(NULL), philo->num + 1);
	// pthread_mutex_unlock(&philo->mutexes.mutexEnd);
	pthread_mutex_unlock(&philo->mutexes.mutexPrintf);
	if (philo->num == 0)
		pthread_mutex_lock(&philo->mutexes.forks[philo->args[0] - 1]);
	else
		pthread_mutex_lock(&philo->mutexes.forks[philo->num - 1]);
	pthread_mutex_lock(&philo->mutexes.mutexPrintf);
	// pthread_mutex_lock(&philo->mutexes.mutexEnd);
	if (philo->lastMeal - philo->startTime > philo->args[0])
		return died(philo);
	printf("%ld %d has taken a fork\n", -philo->startTime
			+ gettime(NULL), philo->num + 1);
	pthread_mutex_unlock(&philo->mutexes.mutexPrintf);

	// pthread_mutex_unlock(&philo->mutexes.mutexEnd);
}

void	put_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->mutexes.forks[philo->num]);
	if (philo->num == 0)
		pthread_mutex_unlock(&philo->mutexes.forks[philo->args[0] - 1]);
	else
		pthread_mutex_unlock(&philo->mutexes.forks[philo->num - 1]);
}

void	*my_sleep(void *data)
{
	int	time;

	time = *(int *)data;
	usleep(time * 1000);
	return (NULL);
}

void	eat(t_philo *philo)
{
	// pthread_t	peat;

	pthread_mutex_lock(&philo->mutexes.mutexPrintf);
	// pthread_mutex_lock(&philo->mutexes.mutexEnd);
	// pthread_mutex_lock(&philo->mutexes.mutexHaveEaten);
	if (philo->lastMeal - philo->startTime > philo->args[0])
		return died(philo);
	printf("%ld %d is eating\n", -philo->startTime
			+ gettime(&(philo->lastMeal)), philo->num + 1);
	
	// pthread_mutex_unlock(&philo->mutexes.mutexEnd);
	pthread_mutex_unlock(&philo->mutexes.mutexPrintf);
	// pthread_mutex_unlock(&philo->mutexes.mutexHaveEaten);
	usleep(philo->args[2] * 1000);
	// if (pthread_create(&peat, NULL, my_sleep, &philo->args[2]))
	// 	exit(1);

	// if (philo->args[4] != -1)
	// {
	// 	if (philo->needtoeat)
	// 		--(*philo->needtoeat);
	// 	++philo->meals;
	// }

	// pthread_join(peat, NULL);
}
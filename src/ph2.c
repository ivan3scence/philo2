#include "../includes/philo.h"

int	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutexes.forks[philo->num]);
	pthread_mutex_lock(&philo->mutexes.mutexPrintf);
	if (gettime(NULL) - philo->lastMeal > philo->args[1])
		return (died(philo));
	printf("%ld %d has taken a fork\n", -philo->startTime
			+ gettime(NULL), philo->num + 1);
	pthread_mutex_unlock(&philo->mutexes.mutexPrintf);
	if (philo->args[0] == 1)
		{
			while(gettime(NULL) - philo->lastMeal <= philo->args[1])
				;
			return (died(philo));
		}
	if (philo->num == 0)
		pthread_mutex_lock(&philo->mutexes.forks[philo->args[0] - 1]);
	else
		pthread_mutex_lock(&philo->mutexes.forks[philo->num - 1]);
	pthread_mutex_lock(&philo->mutexes.mutexPrintf);
	if (gettime(NULL) - philo->lastMeal > philo->args[1])
		return (died(philo));
	printf("%ld %d has taken a fork\n", -philo->startTime
			+ gettime(NULL), philo->num + 1);
	pthread_mutex_unlock(&philo->mutexes.mutexPrintf);
	return (0);
}

void	put_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->mutexes.forks[philo->num]);
	if (philo->num == 0)
		pthread_mutex_unlock(&philo->mutexes.forks[philo->args[0] - 1]);
	else
		pthread_mutex_unlock(&philo->mutexes.forks[philo->num - 1]);
	// (void)philo;
	// return ;
}

int	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutexes.mutexPrintf);
	if (gettime(NULL) - philo->lastMeal > philo->args[1])
		return (died(philo));
	printf("%ld %d is eating\n", -philo->startTime
			+ gettime(&(philo->lastMeal)), philo->num + 1);
	pthread_mutex_unlock(&philo->mutexes.mutexPrintf);
	ftsleep(philo->args[2]);
	return (0);
}

void	ftsleep(long int time)
{
	long int	start;

	start = gettime(NULL);
	while (gettime(NULL) - start < time)
		usleep(42);
}
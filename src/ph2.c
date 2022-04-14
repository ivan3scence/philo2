#include "../includes/philo.h"

int	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutexes.forks[philo->num]);
	pthread_mutex_lock(&philo->mutexes.mutexprintf);
	if (gettime(NULL) - philo->lastmeal > philo->args[1])
		return (died(philo));
	printf("%ld %d has taken a fork\n", -philo->starttime
			+ gettime(NULL), philo->num + 1);
	pthread_mutex_unlock(&philo->mutexes.mutexprintf);
	if (philo->args[0] == 1)
		{
			while(gettime(NULL) - philo->lastmeal <= philo->args[1])
				;
			return (died(philo));
		}
	if (philo->num == 0)
		pthread_mutex_lock(&philo->mutexes.forks[philo->args[0] - 1]);
	else
		pthread_mutex_lock(&philo->mutexes.forks[philo->num - 1]);
	pthread_mutex_lock(&philo->mutexes.mutexprintf);
	if (gettime(NULL) - philo->lastmeal > philo->args[1])
		return (died(philo));
	printf("%ld %d has taken a fork\n", -philo->starttime
			+ gettime(NULL), philo->num + 1);
	pthread_mutex_unlock(&philo->mutexes.mutexprintf);
	return (0);
}

void	put_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->mutexes.forks[philo->num]);
	if (philo->num == 0)
		pthread_mutex_unlock(&philo->mutexes.forks[philo->args[0] - 1]);
	else
		pthread_mutex_unlock(&philo->mutexes.forks[philo->num - 1]);
}

int	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutexes.mutexprintf);
	if (gettime(NULL) - philo->lastmeal > philo->args[1])
		return (died(philo));
	printf("%ld %d is eating\n", -philo->starttime
			+ gettime(&(philo->lastmeal)), philo->num + 1);
	pthread_mutex_unlock(&philo->mutexes.mutexprintf);
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
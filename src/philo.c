#include "../includes/philo.h"

void	*check_death(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	while (gettime(NULL) - philo->lastmeal <= philo->args[1])
		usleep(42);
	died(philo);
	return (NULL);
}

void	*philosopher(void	*a)
{
	t_philo		*philo;
	pthread_t	pdeath;

	philo = (t_philo *)a;
	pthread_create(&pdeath, NULL, check_death, (void *)philo);
	if ((philo->num == 1 + philo->args[0] && philo->args[0] != 1)
		|| philo->num % 2 == 1)
		usleep(philo->args[2] / 2 * 1000);
	philo_life(philo);
	pthread_detach(pdeath);
	return (NULL);
}

int	sleep_ph(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutexes.mutexprintf);
	if (gettime(NULL) - philo->lastmeal > philo->args[1])
		return (died(philo));
	printf("%ld %d is sleeping\n", -philo->starttime
			+ gettime(NULL), philo->num + 1);
	pthread_mutex_unlock(&philo->mutexes.mutexprintf);
	ftsleep(philo->args[3]);
	pthread_mutex_lock(&philo->mutexes.mutexprintf);
	if (gettime(NULL) - philo->lastmeal > philo->args[1])
		return (died(philo));
	printf("%ld %d is thinking\n", -philo->starttime
			+ gettime(NULL), philo->num + 1);
	pthread_mutex_unlock(&philo->mutexes.mutexprintf);
	return (0);
}

int	died(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutexes.mutexend);
	if (*(philo->firstdead->num) == -1)
	{
		philo->args[1] = -1;
		*(philo->firstdead->num) = philo->num;
		*(philo->firstdead->time) = gettime(NULL);
	}
	pthread_mutex_unlock(&philo->mutexes.mutexend);
	
	return (1);
}

int	philo_life(t_philo *philo)
{
	int	meals;
	int	tf;

	meals = 0;
	while (philo->args[4] == -1 || meals < philo->args[4])
	{
		if (gettime(NULL) - philo->lastmeal > philo->args[1])
		{
			died(philo);
			return (1);
		}
		tf = take_forks(philo);
		if (tf == -1)
			continue ;
		if (tf)
			return (1);
		if (eat(philo))
			return (1);
		put_forks(philo);
		if (philo->args[4] != -1 && ++meals >= philo->args[4])
			return (2);
		if (sleep_ph(philo))
			return (1);
	}
	return (2);
}
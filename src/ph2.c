/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdonny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 13:07:44 by sdonny            #+#    #+#             */
/*   Updated: 2022/02/07 13:07:46 by sdonny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		while (gettime(NULL) - philo->lastmeal <= philo->args[1])
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
	if (ftsleep(philo->args[2], philo))
		return (died(philo));
	return (0);
}

int	ftsleep(long int time, t_philo *philo)
{
	long int	start;

	start = gettime(NULL);
	while (gettime(NULL) - start < time)
	{
		if (gettime(NULL) - philo->lastmeal > philo->args[1])
			return (1);
		usleep(42);
	}
	return (0);
}

t_philo	*insert(t_philo *philo, long int *args,
	t_mutexes *mutexes, t_fd *fistdead)
{
	int	i;

	i = -1;
	if (!philo)
	{
		free(fistdead);
		dest_mutexes(*mutexes, args);
		end(MALLOC, args, NULL, NULL);
		return (NULL);
	}
	while (++i < args[0])
	{
		philo[i].args = args;
		philo[i].num = i;
		philo[i].mutexes = *mutexes;
		philo[i].firstdead = fistdead;
	}
	return (philo);
}

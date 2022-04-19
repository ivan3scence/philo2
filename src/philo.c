/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdonny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 13:07:44 by sdonny            #+#    #+#             */
/*   Updated: 2022/02/07 13:07:46 by sdonny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*check_death(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	while (gettime(NULL) - philo->lastmeal <= philo->args[1])
		usleep(250);
	died(philo);
	return (NULL);
}

void	*philosopher(void	*a)
{
	t_philo		*philo;
	pthread_t	pdeath;
	int			ret;

	philo = (t_philo *)a;
	if (pthread_create(&pdeath, NULL, check_death, (void *)philo))
		return (NULL);
	if ((philo->num == philo->args[0] - 1 && philo->args[0] != 1)
		|| philo->num % 2 == 1)
		usleep(600);
	ret = philo_life(philo);
	if (pthread_join(pdeath, NULL))
		return (NULL);
	if (ret == 2)
		*(philo->firstdead->num) = -1;
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
	if (ftsleep(philo->args[3], philo))
		return (died(philo));
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
		*(philo->firstdead->time) = gettime(NULL);
		philo->args[1] = -1;
		philo->lastmeal = 0;
		*(philo->firstdead->num) = philo->num;
	}
	pthread_mutex_unlock(&philo->mutexes.mutexend);
	put_forks(philo);
	return (1);
}

int	philo_life(t_philo *philo)
{
	int	meals;

	meals = 0;
	while (philo->args[4] == -1 || meals < philo->args[4])
	{
		if (gettime(NULL) - philo->lastmeal > philo->args[1])
			return (died(philo));
		if (take_forks(philo))
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

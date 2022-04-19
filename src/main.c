/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdonny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 13:07:44 by sdonny            #+#    #+#             */
/*   Updated: 2022/02/07 13:07:46 by sdonny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

t_philo	*create_struct(long int *args, t_mutexes *mutexes,
	int *fdnum, long int *fdtime)
{
	t_philo		*philo;
	t_fd		*fistdead;

	if (!(mutexes->forks))
	{
		end(MALLOC, args, NULL, NULL);
		return (NULL);
	}
	fistdead = (t_fd *)malloc(sizeof(t_fd));
	if (!fistdead)
	{
		dest_mutexes(*mutexes, args);
		end(MALLOC, args, NULL, NULL);
		return (NULL);
	}
	fistdead->num = fdnum;
	fistdead->time = fdtime;
	philo = (t_philo *)malloc(sizeof(t_philo) * args[0]);
	return (insert(philo, args, mutexes, fistdead));
}

int	threading(t_philo *philo)
{
	pthread_t	*t;
	int			i;

	i = -1;
	t = (pthread_t *)malloc(sizeof(pthread_t) * philo[0].args[0]);
	if (!t)
		return (0);
	philo[0].starttime = gettime(NULL);
	while (++i < philo[0].args[0])
	{
		philo[i].starttime = philo[0].starttime;
		philo[i].lastmeal = philo[0].starttime;
		if (pthread_create(&t[i], NULL, philosopher, &philo[i]))
		{
			free(t);
			return (0);
		}
	}
	return (jointhreads(philo, t));
}

int	jointhreads(t_philo *philo, pthread_t *t)
{
	int	i;

	i = -1;
	while (++i < philo[0].args[0])
	{
		if (pthread_join(t[i], NULL))
		{
			free(t);
			return (0);
		}
	}
	free(t);
	t = NULL;
	return (1);
}

int	main(int argc, char **argv)
{
	long int	*args;
	t_philo		*philo;
	int			fdnum;
	long int	fdtime;
	t_mutexes	mutexes;

	fdnum = -1;
	fdtime = -1;
	args = validate(argc, argv);
	if (!args)
		return (0);
	mutexes = init_mutexes(args);
	philo = create_struct(args, &mutexes, &fdnum, &fdtime);
	if (!philo)
		return (0);
	if (threading(philo))
	{
		if (fdnum != -1)
			printf("%ld %d has died\n", fdtime - philo[0].starttime, fdnum + 1);
	}
	else
		printf("threading rip\n");
	dest_mutexes(mutexes, args);
	end(OK, args, philo, NULL);
}

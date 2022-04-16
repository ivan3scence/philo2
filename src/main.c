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
		end(MALLOC, args, NULL, NULL);
		return (NULL);
	}
	fistdead->num = fdnum;
	fistdead->time = fdtime;
	philo = (t_philo *)malloc(sizeof(t_philo) * args[0]);
	if (!philo)
	{
		free(fistdead);
		end(MALLOC, args, NULL, NULL);
		return (NULL);
	}
	return (insert(philo, args, mutexes, fistdead));
}

void	start(long int *args)
{
	t_philo		*philo;
	pthread_t	*t;
	int			fdnum;
	long int	fdtime;
	t_mutexes	mutexes;

	fdnum = -1;
	fdtime = 0;
	mutexes = init_mutexes(args);
	philo = create_struct(args, &mutexes, &fdnum, &fdtime);
	if (!philo)
	{
		dest_mutexes(mutexes, args);
		return (end(MALLOC, args, NULL, NULL));
	}
	t = threading(philo);
	if (fdnum != -1)
		printf("%ld %d has died\n", fdtime - philo[0].starttime, fdnum + 1);
	dest_mutexes(mutexes, args);
	return (end(OK, args, philo, t));
}

pthread_t	*threading(t_philo *philo)
{
	pthread_t	*t;
	int			i;

	i = -1;
	t = (pthread_t *)malloc(sizeof(pthread_t) * philo[0].args[0]);
	if (!t)
		return (NULL);
	philo[0].starttime = gettime(NULL);
	while (++i < philo[0].args[0])
	{
		philo[i].starttime = philo[0].starttime;
		philo[i].lastmeal = philo[0].starttime;
		if (pthread_create(&t[i], NULL, philosopher, &philo[i]))
		{
			dest_mutexes(philo[0].mutexes, philo[0].args);
			end(THREAD, philo[0].args, philo, t);
			return (NULL);
		}
	}
	return (jointhreads(philo, t));
}

pthread_t	*jointhreads(t_philo *philo, pthread_t *t)
{
	int	i;

	i = -1;
	while (++i < philo[0].args[0])
	{
		if (pthread_join(t[i], NULL))
		{
			end(THREAD, philo[0].args, philo, t);
			return (NULL);
		}
	}
	return (t);
}

int	main(int argc, char **argv)
{
	long int	*args;

	args = validate(argc, argv);
	if (!args)
		end(-1, NULL, NULL, NULL);
	else
		start(args);
}

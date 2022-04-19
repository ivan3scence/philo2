/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdonny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 13:07:44 by sdonny            #+#    #+#             */
/*   Updated: 2022/02/07 13:07:46 by sdonny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

# define ARGS 1
# define MALLOC 2
# define THREAD 3
# define JOIN 4
# define OK 5

typedef struct s_fd
{
	int			*num;
	long int	*time;
}	t_fd;

typedef struct s_mutexes
{
	pthread_mutex_t	mutexprintf;
	pthread_mutex_t	mutexend;
	pthread_mutex_t	*forks;
}	t_mutexes;

typedef struct s_philo
{
	long int		*args;
	int				num;
	long int		starttime;
	long int		lastmeal;
	t_mutexes		mutexes;
	t_fd			*firstdead;
}	t_philo;

long int	pretty_time(t_philo *philo, int flag);
void		put_forks(t_philo *philo);
int			threading(t_philo *philo);
int			take_forks(t_philo *philo);
int			eat(t_philo *philo);
int			ftsleep(long int time, t_philo *philo);
int			died(t_philo *philo);
t_philo		*insert(t_philo *philo, long int *args,
				t_mutexes *mutexes, t_fd *fistdead);
void		*check_death(void *data);
void		*philosopher(void	*a);
int			sleep_ph(t_philo *philo);
int			philo_life(t_philo *philo);
size_t		ft_strlen(const char *str);
void		end(int err_no, long int *args,
				t_philo *philo, pthread_t *t);
void		ft_putstr_fd(char *s, int fd);
long int	gettime(long int *time);
long int	ft_atoi(const char *str, long long int k);
long int	*check_digits(int argc, char **argv);
long int	*validate(int argc, char **argv);
int			jointhreads(t_philo *philo, pthread_t *t);
void		dest_mutexes(t_mutexes mutexes, long int *args);
t_mutexes	init_mutexes(long int *args);
t_philo		*create_struct(long int *args, t_mutexes *mutexes,
				int *fdnum, long int *fdtime);
void		start(long int *args);
long int	*matoi(char **argv, long int *args);

#endif

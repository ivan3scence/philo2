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


typedef struct s_flags
{
	int	*haveEaten;
	int	someoneDied;
}	t_flags;

typedef struct s_args
{
	int				*pars;
	int				quant;
	int				toDie;
	int				toEat;
	int				toSleep;
	int				lunches;
	long int		startTime;
	t_flags			flags;
}	t_args;

typedef struct s_mutexes
{
	pthread_mutex_t	mutexPrintf;
	pthread_mutex_t	mutexHaveEaten;
	pthread_mutex_t	mutexEnd;
	pthread_mutex_t	*forks;
}	t_mutexes;

typedef struct s_philo
{
	int				*args;
	int				num;
	int				meals;
	long int		lastMeal;
	t_mutexes		mutexes;
}	t_philo;

long int	pretty_time(t_philo *philo, int flag);

# endif

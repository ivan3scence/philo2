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


typedef struct s_flags
{
	int	*haveEaten;
	int	someoneDied;
}	t_flags;

typedef struct s_fd
{
	int			num;
	long int	time;
}	t_fd;
// typedef struct s_args
// {
// 	int				*pars;
// 	int				quant;
// 	int				toDie;
// 	int				toEat;
// 	int				toSleep;
// 	int				lunches;
// 	long int		startTime;
// 	t_flags			flags;
// }	t_args;

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
	// int				*needtoeat;
	int				num;
	int				*someoneDead;
	int				meals;
	long int		startTime;
	long int		lastMeal;
	t_mutexes		mutexes;
	t_fd			*firstDead;
}	t_philo;

long int	pretty_time(t_philo *philo, int flag);
void		put_forks(t_philo *philo);
void		take_forks(t_philo *philo);
void		*my_sleep(void *data);
void		eat(t_philo *philo);
void		*checkDeath(void *data);
void		*philosopher(void	*a);
void		sleep_ph(t_philo *philo);
void		*philo_life(void *p);
size_t		ft_strlen(const char *str);
void		end(int err_no, int *args, t_philo *philo, pthread_t *t);
void		ft_putstr_fd(char *s, int fd);
long int	gettime(long int *time);
int			ft_atoi(const char *str);
int			*checkDigits(int argc, char **argv);
int			*validate(int argc, char **argv);
void		dest_mutexes(t_mutexes mutexes, int *args);
// int			checkEnd(int *args);
t_mutexes	init_mutexes(int *args);
t_philo		*create_struct(int *args, t_mutexes *mutexes, int *deadman);
void		start(int *args);

# endif

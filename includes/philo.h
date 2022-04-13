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
	pthread_mutex_t	mutexPrintf;
	pthread_mutex_t	mutexEnd;
	pthread_mutex_t	*forks;
}	t_mutexes;

typedef struct s_philo
{
	long int		*args;
	int				num;
	long int		startTime;
	long int		lastMeal;
	t_mutexes		mutexes;
	t_fd			*firstDead;
}	t_philo;

long int	pretty_time(t_philo *philo, int flag);
void		put_forks(t_philo *philo);
int			take_forks(t_philo *philo);
int			eat(t_philo *philo);
void		ftsleep(long int time);
int			died(t_philo *philo);
void		*checkDeath(void *data);
void		*philosopher(void	*a);
int			sleep_ph(t_philo *philo);
int			philo_life(t_philo *philo);
size_t		ft_strlen(const char *str);
void		end(int err_no, long int *args,
t_philo *philo, pthread_t *t);
void		ft_putstr_fd(char *s, int fd);
long int	gettime(long int *time);
long int	ft_atoi(const char *str);
long int	*checkDigits(int argc, char **argv);
long int	*validate(int argc, char **argv);
void		dest_mutexes(t_mutexes mutexes, long int *args);
t_mutexes	init_mutexes(long int *args);
t_philo		*create_struct(long int *args, t_mutexes *mutexes,
int *fdnum, long int *fdtime);
void		start(long int *args);

# endif

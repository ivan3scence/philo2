#include "../includes/philo.h"

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	if (fd != -1)
		write(fd, s, ft_strlen(s));
}

static void	end(int err_no, t_args *args)
{
	if (args)
	{
		free(args);
		args = NULL;
	}
	if (err_no == ARGS)
		ft_putstr_fd("Invalid argements!\n", 2);
	else if (err_no == MALLOC)
		ft_putstr_fd("Malloc rip\n", 2);
}

long int	gettime(int *time)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	if (time)
		*time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	ft_atoi(const char *str)
{
	long long int	n;
	long long int	k;
	int				i;

	k = 1;
	n = 0;
	i = 0;
	while ((str[i]) && (((str[i] > 8) && (str[i] < 14)) || (str[i] == 32)))
		i++;
	if (str[i] == 45 || str[i] == 43)
	{
		if (str[i++] == 45)
			k = k * (-1);
	}
	while (str[i] && !(str[i] < 48 || str[i] > 57))
	{
		n = n * 10 + str[i++] - 48;
		if (n < 0)
		{
			if (k > 0)
				return (-1);
			return (0);
		}
	}
	if (str[i])
		return (0);
	return (n * k);
}

static t_args	*checkDigits(int argc, char **argv)
{
	t_args	*args;

	args = (t_args *)malloc(sizeof(t_args));
	if (!args)
	{
		end(MALLOC, NULL);
		return (NULL);
	}
	if (argc == 5)
		args->lunches = -1;
	else
	{
		args->lunches = ft_atoi(argv[5]);
		if (!args->lunches)
		{
			end(ARGS, args);
			return (NULL);
		}
	}
	args->quant = ft_atoi(argv[1]);
	args->toDie = ft_atoi(argv[2]);
	args->toEat = ft_atoi(argv[3]);
	args->toSleep = ft_atoi(argv[4]);
	args->flags.someoneDied = 0;
	if (!(args->quant && args->toEat && args->toDie && args->toSleep))
	{
		end(ARGS, args);
		return (NULL);
	}
	args->flags.haveEaten = (int *)malloc(sizeof(int) * args->quant);
	if (!args->flags.haveEaten)
		exit(1);//!!!!!!!!!!!!!!!!!!!!!!!!!!
	memset(args->flags.haveEaten, 0, sizeof(int));
	return (args);
}

static t_args	*validate(int argc, char **argv)
{
	t_args	*args;

	if (argc < 5 || argc > 6)
	{
		end(ARGS, NULL);
		return (NULL);
	}
	args = checkDigits(argc, argv);
	return (args);
}

static void	finish(t_args *args, t_philo *philo)
{
	if (args)
	{
		free(args->flags.haveEaten);
		args->flags.haveEaten = NULL;
		free(args);
		args = NULL;
	}
	if (philo)
	{
		if (philo->mutexes.forks)
		{
			free(philo->mutexes.forks);
			philo->mutexes.forks = NULL;
		}
		free(philo);
		philo = NULL;
	}
} 

long int	pretty_time(t_philo *philo, int flag)
{
	long int		time;
	struct timeval	new_time;

	if (gettimeofday(&new_time, NULL))
		return (0);
	time = new_time.tv_sec * 1000 + new_time.tv_usec / 1000;
	if (flag)
		philo->lastMeal = time;
	return (time);
}

static void	set_time(int *args)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	args->startTime = 1000 * tv.tv_sec + tv.tv_usec / 1000;
	return ;
}

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutexes.forks[philo->num]);
	pthread_mutex_lock(&philo->mutexes.mutexPrintf);
	printf("%ld %d has taken a fork\n", -philo->args->startTime
			+ pretty_time(philo, 0), philo->num + 1);
	pthread_mutex_unlock(&philo->mutexes.mutexPrintf);
	if (philo->num == 0)
		pthread_mutex_lock(&philo->mutexes.forks[philo->args->quant - 1]);
	else
		pthread_mutex_lock(&philo->mutexes.forks[philo->num - 1]);
	pthread_mutex_lock(&philo->mutexes.mutexPrintf);
	printf("%ld %d has taken a fork\n", -philo->args->startTime
			+ pretty_time(philo, 0), philo->num + 1);
	pthread_mutex_unlock(&philo->mutexes.mutexPrintf);
}

void	put_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->mutexes.forks[philo->num]);
	if (philo->num == 0)
		pthread_mutex_unlock(&philo->mutexes.forks[philo->args->quant - 1]);
	else
		pthread_mutex_unlock(&philo->mutexes.forks[philo->num - 1]);
}

void	*my_sleep(void *data)
{
	int	time;

	time = *(int *)data;
	usleep(time * 1000);
	return (NULL);
}

void	eat(t_philo *philo)
{
	pthread_t	peat;

	if (pthread_create(&peat, NULL, my_sleep, &philo->args[2]))
		exit(1);
	pthread_mutex_lock(&philo->mutexes.mutexPrintf);
	pthread_mutex_lock(&philo->mutexes.mutexHaveEaten);
	printf("%ld %d is eating\n", -philo->startTime
			+ gettime(NULL), philo->num + 1);
	pthread_mutex_unlock(&philo->mutexes.mutexPrintf);
	if (philo->args[3] != -1)
	{
		if (philo->needtoeat)
			--(*philo->needtoeat);
		++philo->meals;
	}
	pthread_mutex_unlock(&philo->mutexes.mutexHaveEaten);
	pthread_join(peat, NULL);
}

void	sleep_ph(t_philo *philo)
{
	pthread_t	psleep;

	if (pthread_create(&psleep, NULL, my_sleep, &philo->args[3]))
		exit(1);
	pthread_mutex_lock(&philo->mutexes.mutexPrintf);
	printf("%ld %d is sleeping\n", -philo->startTime
			+ pretty_time(philo, 0), philo->num + 1);
	pthread_mutex_unlock(&philo->mutexes.mutexPrintf);
	pthread_join(psleep, NULL);
}

void	think(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutexes.mutexPrintf);
	printf("%ld %d is thinking\n", -philo->args->startTime
			+ pretty_time(philo, 0), philo->num + 1);
	pthread_mutex_unlock(&philo->mutexes.mutexPrintf);
}

void	*philo_life(void *p)
{
	t_philo	*philo;
	int		meals;

	meals = 0;
	philo = (t_philo *)p;
	pthread_mutex_lock(&philo->mutexes.mutexEnd);
	// while (!philo->args->flags.someoneDied && (philo->args[4] == -1
	while (!*(philo->someoneDead) && (philo->args[4] == -1
				|| ++meals < philo->args[4]))
	{
		// pthread_mutex_unlock(&philo->mutexes.mutexEnd);
		take_forks(philo);
		eat(philo);
		put_forks(philo);
		sleep_ph(philo);
		think(philo);
		// pthread_mutex_lock(&philo->mutexes.mutexEnd);
	}
	return (NULL);
}

//void	*dead(t_philo *philo)
//{
//	//pthread_mutex_lock(&philo->mutexes.mutexPrintf);
//	//pthread_mutex_lock(&philo->mutexes.mutexEnd);
//	//printf("%ld %d died\n", pretty_time(philo, 0)
//			- philo->args->startTime, philo->num + 1);
//	//pthread_mutex_unlock(&philo->mutexes.mutexEnd);
//	//pthread_mutex_unlock(&philo->mutexes.mutexPrintf);
//	return ((void *)1);
//}

void	*checkDeath(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	// usleep(1000 * philo->args[1]);
	pthread_mutex_lock(&philo->mutexes.mutexHaveEaten);
	while (philo->args[4] == -1 || philo->args[4] > philo->meals)
	{
		pthread_mutex_unlock(&philo->mutexes.mutexHaveEaten);
		if (gettime(NULL) - philo->lastMeal > philo->args[1])
			break ;
		// usleep(1000 * (philo->args[1] - (gettime(NULL)
		// 			- philo->lastMeal)));
		pthread_mutex_lock(&philo->mutexes.mutexHaveEaten);
	}
	if (pretty_time(philo, 0) - philo->lastMeal > philo->args[1])
	{
		// pthread_mutex_lock(&philo->mutexes.mutexEnd);
		*(philo->someoneDead)++;
		// philo->args->flags.someoneDied = philo->num;
		//pthread_mutex_lock(&philo->mutexes.mutexPrintf);
		return ((void *)1);
	}
	// philo->args->flags.haveEaten[philo->num + 1] = 1;
	pthread_mutex_unlock(&philo->mutexes.mutexHaveEaten);
	return ((void *)2);
}

static void	*philosopher(void	*a)
{
	t_philo	*philo;
	pthread_t	pdeath;
	pthread_t	plife;
	int		retur;

	philo = (t_philo *)a;
	if (pthread_create(&pdeath, NULL, checkDeath, philo))
			return (NULL);
	if (pthread_create(&plife, NULL, philo_life, philo))
			return (NULL);
	pthread_join(pdeath, (void *)&retur);
	if (retur == 1)
		pthread_detach(plife);
	else
		pthread_join(plife, NULL);
	free(philo);
	if (retur == 1)
		return ((void *)1);
	return (NULL);
}

static void	dest_mutexes(t_mutexes mutexes, int *args)
{
	int	i;

	i = -1;
	pthread_mutex_destroy(&mutexes.mutexPrintf);
	pthread_mutex_destroy(&mutexes.mutexHaveEaten);
	pthread_mutex_destroy(&mutexes.mutexEnd);
	while (++i < args->quant)
		pthread_mutex_destroy(&mutexes.forks[i]);
	free(mutexes.forks);
}

static t_mutexes	init_mutexes(t_args *args)
{
	t_mutexes	mutexes;
	int	i;

	i = -1;
	pthread_mutex_init(&mutexes.mutexPrintf, NULL);
	pthread_mutex_init(&mutexes.mutexHaveEaten, NULL);
	pthread_mutex_init(&mutexes.mutexEnd, NULL);
	mutexes.forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* args->quant);
	if (!mutexes.forks)
		exit(1);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	while (++i < args->quant)
		pthread_mutex_init(&mutexes.forks[i], NULL);
	return (mutexes);
}

static t_philo	*create_struct(int *args, t_mutexes *mutexes)
{
	t_philo		*philo;
	int			i;
	long int	time;
	int			deadman;

	i = -1;
	deadman = 0;
	gettime(&time);
	philo = (t_philo *)malloc(sizeof(t_philo) * args[0]);
	if (!philo)
		return (NULL);//!!!!!!!!!!!!!!!!
	if (args[4] != -1)
	{
		args[4] = args[0] * args[4];
	}
	while (++i < args[0])
	{
		philo[i].startTime = time;
		philo[i].args = args;
		philo[i].num = i;
		philo[i].mutexes = *mutexes;
		philo[i].needtoeat = NULL;
		if (args[4] != -1)
			philo[i].needtoeat = &args[4];
	}

	return (philo);
}

int	checkEnd(int *args)
{
	int	i;
	int	full;

	while (1)
	{
		i = -1;
		while (++i < args->quant)
		{
			if (args->flags.someoneDied != 0)
			{
				printf("%ld %d died\n", -args->startTime +
						pretty_time(NULL, 0), args->flags.someoneDied);
				return (-1);
			}
			if (args->flags.haveEaten[i] == 1)
				full++;
		}
		if (full == args->quant)
			return (1);
		full = 0;
	}
}

void	start(int *args)
{
	pthread_t	t[args->quant];
	t_mutexes	mutexes;
	t_philo		*philo;
	int			i;
	//int			fd;

	i = -1;
	set_time(args);
	mutexes = init_mutexes(args);
	philo = create_struct(args, &mutexes);
	if (!philo)
		return ;
	while (++i < args[0])
	{
		if (pthread_create(&t[i], NULL, philosopher, &philo[i]))
		{
			printf("Threads rip!\n");
			free(philo);
			free(args);
			free(mutexes.forks);
			return ;
		}
	}
	i = -1;
	if (checkEnd(args) == -1)
	{
		while (++i < args[0])
			pthread_detach(t[i]);
		dest_mutexes(mutexes, args);
		finish(args, NULL);
		free(philo);
		printf("umer\n");
		return ;
	}
	while (++i < args->quant)
	{
		if (!pthread_join(t[i], NULL))
				return ;
	}
	dest_mutexes(mutexes, args);
	finish(args, NULL);
	free(philo);
}

int	main(int argc, char **argv)
{
	t_args	*args;

	args = validate(argc, argv);
	if (!args)
		return (0);
	start(args);
	//finish(args, NULL);
}

#include "../includes/philo.h"

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	end(int err_no, long int *args, t_philo *philo, pthread_t *t)
{
	if (philo)
	{
		if (philo->firstdead)
			free(philo->firstdead);
		free(philo);
	}
	if (args)
		free(args);
	if (t)
		free(t);
	if (err_no == ARGS)
		ft_putstr_fd("Invalid argements!\n", 2);
	else if (err_no == MALLOC)
		ft_putstr_fd("Malloc rip\n", 2);
	else if (err_no == THREAD)
		ft_putstr_fd("Tread rip\n", 2);
	else if (err_no == JOIN)
		ft_putstr_fd("Join rip\n", 2);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	if (fd != -1)
		write(fd, s, ft_strlen(s));
}

long int	gettime(long int *time)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	if (time)
		*time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

long int	ft_atoi(const char *str)
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
			return (-1);
		}
	}
	if (str[i])
		return (0);
	return (n * k);
}

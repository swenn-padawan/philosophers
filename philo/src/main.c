/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stetrel <stetrel@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 11:47:11 by stetrel           #+#    #+#             */
/*   Updated: 2025/01/14 19:41:45 by stetrel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <pthread.h>
#include <sys/time.h>

long	get_ms(struct timeval val)
{
	return (val.tv_sec * 1000) + (val.tv_usec / 1000);
}

void	print_philo(long ms, t_philo *philo)
{
	printf("%ldms | Philo %d has taken a fork |\n", ms, philo->id);
}

void	*routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	struct timeval	tv;
	long	res;

	while (*(philo->flag) == 0)
		continue;
	while (1)
	{
		gettimeofday(&tv, NULL);
		res = get_ms(tv);
		long res2 = get_ms(philo->time);
		pthread_mutex_lock(philo->mutex);

		print_philo(res - res2, philo);

		pthread_mutex_unlock(philo->mutex);
		if (res - res2 == philo->data.time_to_die)
			break ;
	}
	return (NULL);
}

int	check_data(char **argv, t_data *data, int argc)
{
	int	error;

	error = 0;
	if (argc <= 4 || argc >= 7)
	{
		handle_error(ERR_NUMBER_ARGS);
		return (1);
	}
	philo_parse_args(data, argv, argc, &error);
	if (error)
		return (1);
	return (0);
}

void	sim_init(t_philo **philos, t_data src_data, int *flag)
{
	*philos = malloc(sizeof(t_philo) * src_data.nb_philo);
	if (__builtin_expect(*philos == NULL, 0))
		return ;
	if (!*philos)
		return ;
	*flag = 1;
}

int	main(int argc, char **argv)
{
	static t_philo		*philos = {0};
	int					i = -1;
	t_data				data;
	static int			flag = 0;
	pthread_mutex_t	routine_mutex;

	if (check_data(argv, &data, argc))
		return (1);
	sim_init(&philos, data, &flag);
	pthread_mutex_init(&routine_mutex, NULL);
	while (++i < data.nb_philo)
	{
		ft_memcpy(&philos[i].data, &data, sizeof(t_data));
		philos[i].flag = &flag;
		philos[i].id = i;
		philos[i].mutex = &routine_mutex;
		gettimeofday(&philos[i].time, NULL);
		pthread_create(&philos[i].fork, NULL, routine, &philos[i]);
	}
	flag = 1;
	i = -1;
	while (++i < (*philos).data.nb_philo)
		pthread_join(philos[i].fork, NULL);
	free(philos);
}

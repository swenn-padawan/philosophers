/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stetrel <stetrel@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 11:47:11 by stetrel           #+#    #+#             */
/*   Updated: 2025/01/13 20:55:37 by stetrel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <pthread.h>
#include <sys/time.h>

long	get_ms(struct timeval val)
{
	return (val.tv_sec * 1000) + (val.tv_usec / 1000);
}

void	*routine(void *arg)
{
	t_simulation *sim = (t_simulation*)arg;
	struct timeval	tv;
	long	res;

	while (1)
	{
		gettimeofday(&tv, NULL);
		res = get_ms(tv);
		long res2 = get_ms(sim->time);
		pthread_mutex_lock(&sim->mutex);
		printf("elapsed time = %ld ms\n", res - res2);
		pthread_mutex_unlock(&sim->mutex);
		// if philo dead: break
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

void	sim_init(t_simulation *simulation, t_philo **philos)
{
	int	i;

	i = -1;
	*philos = malloc(sizeof(t_philo) * simulation->data.nb_philo);
	if (!*philos)
		return ;
	while (++i < simulation->data.nb_philo)
	{
		(*philos)[i].id = i;
		pthread_create(&(*philos)[i].fork, NULL, routine, simulation);
	}
}

int	main(int argc, char **argv)
{
	t_philo				*philos;
	int					i;
	t_simulation		simulation = {0};

	if (check_data(argv, &simulation.data, argc))
		return (1);
	sim_init(&simulation, &philos);
	i = -1;
	gettimeofday(&simulation.time, NULL);
	while (++i < simulation.data.nb_philo)
		pthread_join(philos[i].fork, NULL);
	free(philos);
}

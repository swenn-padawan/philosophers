/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stetrel <stetrel@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 11:47:11 by stetrel           #+#    #+#             */
/*   Updated: 2025/01/13 16:38:56 by stetrel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <pthread.h>
#include <stdatomic.h>
#include <sys/time.h>


long	get_time_in_ms(struct timeval time)
{
	return (time.tv_sec * 1000L + time.tv_usec / 1000L);
}

void	*routine(void *arg)
{
	t_simulation *sim = (t_simulation*)arg;
	struct timeval	tv;

	pthread_mutex_lock(&sim->mutex);
	gettimeofday(&tv, NULL);
	long	time_sim_ms = get_time_in_ms(sim->time);
	long	time_routine_ms = get_time_in_ms(tv);
	printf("time sim = %ld ms\n", time_sim_ms);
	printf("time routine = %ld ms\n", time_routine_ms);
	printf("elapsed time = %ld ms\n", time_routine_ms - time_sim_ms);
	pthread_mutex_unlock(&sim->mutex);
	return (NULL);
}

static int	check_data(char **argv, t_data *data, int argc)
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

static void	sim_init(t_simulation *simulation, t_philo **philos)
{
	int	i;

	i = -1;
	*philos = malloc(sizeof(t_philo) * simulation->data.nb_philo);
	if (!*philos)
		return ;
	pthread_mutex_init(&simulation->mutex, NULL);
	while (++i < simulation->data.nb_philo)
	{
		(*philos)[i].id = i;
		pthread_create(&(*philos)[i].fork, NULL, routine, &simulation);
	}
}

int	main(int argc, char **argv)
{
	t_philo				*philos;
	int					i;
	static t_simulation	simulation = {0};

	if (check_data(argv, &simulation.data, argc))
		return (1);
	gettimeofday(&simulation.time, NULL);
	sim_init(&simulation, &philos);
	i = -1;
	printf("jgl;kdfs = %ld\n", simulation.time.tv_usec);
	while (++i < simulation.data.nb_philo)
		pthread_join(philos[i].fork, NULL);
	pthread_mutex_destroy(&simulation.mutex);
	free(philos);
}

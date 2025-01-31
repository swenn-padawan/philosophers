/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stetrel <stetrel@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 11:47:11 by stetrel           #+#    #+#             */
/*   Updated: 2025/01/31 02:35:04 by stetrel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <pthread.h>
#include <sys/time.h>

long	get_ms(struct timeval val)
{
	return (val.tv_sec * 1000L) + (val.tv_usec / 1000L);
}

long get_elapsed_ms(void)
{
	static int	flag = 0;
	static struct timeval	start;
	struct timeval	current;

	if (!flag)
	{
		gettimeofday(&start, NULL);
		flag = 1;
	}
	gettimeofday(&current, NULL);
    return ((current.tv_sec - start.tv_sec) * 1000L) +
           ((current.tv_usec - start.tv_usec) / 1000L);
}

void print_state(long ms, t_philo *philo, char *state, char *color)
{
	if (*(philo->flag) == 0)
		return ;
    pthread_mutex_lock(philo->mutex);
    printf("%s%ldms | Philosopher %d %s%s\n", color, ms, philo->id + 1, state, RESET);
    pthread_mutex_unlock(philo->mutex);
}


int has_died(t_philo *philo, long time_to_die, pthread_mutex_t *mutex)
{
    long elapsed_time;

    pthread_mutex_lock(mutex);
    elapsed_time = get_elapsed_ms() - philo->last_eat;
    if (elapsed_time >= time_to_die)
    {
        print_state(elapsed_time, philo, "has died", RED);
        *(philo->flag) = 0;
        pthread_mutex_unlock(mutex);
        return (1);
    }
    pthread_mutex_unlock(mutex);
    return (0);
}

void	*routine(t_philo *philo)
{
		
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

void cleanup(t_philo *philos, t_data data)
{
	int	i;

	i = 0;
    while (i < data.nb_philo) 
	{
		pthread_mutex_destroy(&data.forks[i]);
        pthread_mutex_destroy(philos[i].mutex);
        free(philos[i].mutex);
		i++;
    }
    free(philos);
}

void	sim_init(t_simulation *sim, int *error)
{
	int	i;

	i = 0;
	pthread_mutex_lock(sim->philos[i].start);
	while (i < sim->data.nb_philo)
	{
		sim->philos[i].id = i;
		sim->philos[i].left = &sim->fork[i];
		sim->philos[i].right = &sim->fork[i + 1];
		sim->philos[i].data = sim->data;
		pthread_create(&sim->philos[i].fork, NULL, (void *(*)(void *))routine, &sim->philos[i]);
	}
	usleep(1000);
	pthread_mutex_unlock(sim->philos[i].start);
}

int	main(int argc, char **argv)
{
	t_data				data;
	int					error;
	static t_simulation	sim = {0};

	philo_parse_args(&data, argv, argc, &error);
	if (error)
		return (1);
	



}

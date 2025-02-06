/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stetrel <stetrel@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 11:47:11 by stetrel           #+#    #+#             */
/*   Updated: 2025/02/06 19:17:22 by stetrel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

long	get_ms(struct timeval val)
{
	return (val.tv_sec * 1000L) + (val.tv_usec / 1000L);
}

long get_elapsed_ms(void)
{
	static int	flag = 0;
	static struct timeval	start;
	struct timeval	current;
	struct timeval ts_start;

	if (!flag)
	{
		gettimeofday(&start, NULL);
		flag = 1;
	}
	ts_start = start;
	gettimeofday(&current, NULL);
    return ((current.tv_sec - ts_start.tv_sec) * 1000L) +
           ((current.tv_usec - ts_start.tv_usec) / 1000L);
}

int has_died(t_philo *philo, long time_to_die)
{
    long elapsed_time;

	if (philo->flag_must_eat && !philo->data.nb_must_eat)
		return (1);
	pthread_mutex_lock(philo->dead);
    elapsed_time = get_elapsed_ms() - philo->last_eat;
	pthread_mutex_unlock(philo->dead);
    if (elapsed_time >= time_to_die)
    {
		pthread_mutex_lock(philo->print);
		printf("%s%ldms | Philosopher %d %s%s\n", RED, get_elapsed_ms(), philo->id + 1, DIED, RESET);
		pthread_mutex_unlock(philo->print);
		*(philo->is_dead) = 1;
        return (1);
    }
    return (0);
}

int print_state(long ms, t_philo *philo, char *state, char *color)
{
	if (*(philo->is_dead) || has_died(philo, philo->data.time_to_die))
		return (1);
	pthread_mutex_lock(philo->print);
	printf("%s%ldms | Philosopher %d %s%s\n", color, ms, philo->id + 1, state, RESET);
	pthread_mutex_unlock(philo->print);
	return (0);
}


void	*routine(t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(philo->left);
		if (print_state(get_elapsed_ms(), philo, LEFT, CYAN))
		{
			pthread_mutex_unlock(philo->left);
			break ;
		}
		pthread_mutex_lock(philo->right);
		if (print_state(get_elapsed_ms(), philo, LEFT, CYAN))
		{
			pthread_mutex_unlock(philo->left);
			pthread_mutex_unlock(philo->right);
			break ;
		}
		philo->last_eat  = get_elapsed_ms();
		if (philo->flag_must_eat)
			philo->data.nb_must_eat--;
		if (print_state(get_elapsed_ms(), philo, EAT, MAGENTA))
			break ;
		usleep(1000 * philo->data.time_to_eat);
		pthread_mutex_unlock(philo->left);
		pthread_mutex_unlock(philo->right);
		if (print_state(get_elapsed_ms(), philo, SLEEP, BLUE))
			break ;
		usleep(1000 * philo->data.time_to_sleep);
		if (print_state(get_elapsed_ms(), philo, THINK, BLUE))
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

void	sim_init(t_simulation *sim, int *error)
{
	int	i;

	i = 0;
	while (i < sim->data.nb_philo)
	{
		sim->philos[i].id = i;
		sim->philos[i].left = &sim->fork[i];
		sim->philos[i].right = &sim->fork[(i + 1) % sim->data.nb_philo];
		memcpy(&sim->philos[i].data, &sim->data, sizeof(sim->data));
		sim->philos[i].dead = &sim->dead;
		sim->philos[i].is_dead = &sim->is_dead;
		sim->philos[i].print = &sim->print;
		sim->philos[i].last_eat_m = &sim->last_eat_m;
		sim->philos[i].flag_must_eat = (sim->data.nb_must_eat != 0);
		if (pthread_create(&sim->philos[i].fork, NULL, (void *(*)(void *))routine, &sim->philos[i]) != 0)
		{
			*error = ERR_THREAD_FAILED;
			return ;
		}
		i++;
	}
	*error = 0;
}

int	main(int argc, char **argv)
{
	t_simulation		sim;
	int					i = 0;
	int					error;

	sim = (t_simulation){0};
	if (check_data(argv, &sim.data, argc))
		return (1);
	sim_init(&sim, &error);
	if (error)
		return (1);
	while (i < sim.data.nb_philo)
	{
		pthread_join(sim.philos[i].fork, NULL);
		i++;
	}
	return (0);
}

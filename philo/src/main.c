/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stetrel <stetrel@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 11:47:11 by stetrel           #+#    #+#             */
/*   Updated: 2025/02/01 22:50:11 by stetrel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
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

int has_died(t_philo *philo, long time_to_die)
{
    long elapsed_time;

    elapsed_time = get_elapsed_ms() - philo->last_eat;
	pthread_mutex_lock(philo->dead);
    if (elapsed_time >= time_to_die)
    {
		philo->is_dead = '1';
		pthread_mutex_lock(philo->print);
		printf("%s%ldms | Philosopher %d %s%s\n", RED, get_elapsed_ms(), philo->id + 1, DIED, RESET);
		pthread_mutex_unlock(philo->print);
        return (1);
    }
	pthread_mutex_unlock(philo->dead);
    return (0);
}

void	manager(t_simulation *sim)
{
	int	i;

	while (1)
	{
		i = 0;
		while (i < sim->data.nb_philo)
		{
			if (has_died(&sim->philos[i], get_elapsed_ms()))
				exit(1);
			i++;	
		}
	}
}

void print_state(long ms, t_philo *philo, char *state, char *color)
{
	pthread_mutex_lock(philo->print);
	printf("%s%ldms | Philosopher %d %s%s\n", color, ms, philo->id + 1, state, RESET);
	pthread_mutex_unlock(philo->print);
}

void	*routine(t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(philo->left);
		print_state(get_elapsed_ms(), philo, LEFT, CYAN);
		pthread_mutex_lock(philo->right);
		print_state(get_elapsed_ms(), philo, RIGHT, CYAN);
		pthread_mutex_unlock(philo->left);
		pthread_mutex_unlock(philo->right);
		print_state(get_elapsed_ms(), philo, EAT, GREEN);
		usleep(1000 * philo->data.time_to_eat);
		print_state(get_elapsed_ms(), philo, SLEEP, BLUE);
		usleep(1000 * philo->data.time_to_sleep);
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
		sim->philos[i].data = sim->data;
		sim->philos[i].dead = &sim->dead;
		sim->philos[i].print = &sim->print;
		if (pthread_create(&sim->philos[i].fork, NULL, (void *(*)(void *))routine, &sim->philos[i]) != 0)
		{
			*error = ERR_THREAD_FAILED;
			return ;
		}
		i++;
	}
	usleep(1000);
}

int	main(int argc, char **argv)
{
	int					error = 0;
	t_simulation		sim;
	int					i = 0;

	sim = (t_simulation){0};
	philo_parse_args(&sim.data, argv, argc, &error);
	if (error)
		return (1);
	sim_init(&sim, &error);
	manager(&sim);
	while (i < sim.data.nb_philo)
	{
		pthread_join(sim.philos[i].fork, NULL);
		i++;
	}
}

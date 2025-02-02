/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stetrel <stetrel@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 11:47:11 by stetrel           #+#    #+#             */
/*   Updated: 2025/02/02 01:13:16 by stetrel          ###   ########.fr       */
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
	static pthread_mutex_t time = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_lock(&time);
	if (!flag)
	{
		gettimeofday(&start, NULL);
		flag = 1;
	}
	ts_start = start;
	gettimeofday(&current, NULL);
	pthread_mutex_unlock(&time);
    return ((current.tv_sec - ts_start.tv_sec) * 1000L) +
           ((current.tv_usec - ts_start.tv_usec) / 1000L);
}

int has_died(t_philo *philo, long time_to_die)
{
    long elapsed_time;
	static pthread_mutex_t	time = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_lock(&time);
	if (!philo->data.nb_must_eat)
	{
		pthread_mutex_unlock(&time);
		return (1);
	}
	elapsed_time = get_elapsed_ms() - philo->last_eat;
    if (elapsed_time >= time_to_die)
    {
		pthread_mutex_lock(philo->print);
		printf("%s%ldms | Philosopher %d %s%s\n", RED, get_elapsed_ms(), philo->id + 1, DIED, RESET);
		pthread_mutex_unlock(philo->print);
		pthread_mutex_unlock(&time);
        return (1);
    }
	pthread_mutex_unlock(&time);
    return (0);
}

void	*ft_manager(t_simulation *sim)
{
	int	i;

	while (1)
	{
		i = 0;
		while (i < sim->data.nb_philo)
		{
			if (has_died(&sim->philos[i], sim->data.time_to_die))
				exit(1);
			i++;	
		}
	}
	return (NULL);
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
		philo->last_eat  = get_elapsed_ms();
		philo->data.nb_must_eat--;
		print_state(get_elapsed_ms(), philo, EAT, MAGENTA);
		pthread_mutex_unlock(philo->left);
		pthread_mutex_unlock(philo->right);
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
		memcpy(&sim->philos[i].data, &sim->data, sizeof(sim->data));
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
}

int	main(int argc, char **argv)
{
	t_simulation		sim;
	int					i = 0;
	int					error;

	sim = (t_simulation){0};
	if (check_data(argv, &sim.data, argc))
		return (1);
	pthread_t manager;
	sim_init(&sim, &error);
	if (error)
		return (1);
	pthread_create(&manager, NULL, (void *(*)(void *))ft_manager, &sim);
	while (i < sim.data.nb_philo)
	{
		pthread_join(sim.philos[i].fork, NULL);
		i++;
	}
	pthread_join(manager, NULL);
	return (0);
}

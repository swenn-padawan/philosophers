/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stetrel <stetrel@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 13:28:49 by stetrel           #+#    #+#             */
/*   Updated: 2025/02/23 15:52:23 by stetrel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

void	print_state(t_philo *philo, char *color, char *to_print)
{
	pthread_mutex_lock(philo->dead_mutex);
	if (*(philo->dead))
	{
		pthread_mutex_unlock(philo->dead_mutex);
		return ;
	}
	pthread_mutex_unlock(philo->dead_mutex);
	pthread_mutex_lock(&philo->mutex);
	if (philo->nb_meals == philo->data.nb_must_eat && philo->data.nb_must_eat)
	{
		pthread_mutex_unlock(&philo->mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->mutex);
	pthread_mutex_lock(philo->print);
	printf("%s %ldms Philosopher %d %s %s",
		color, get_elapsed_ms(), philo->id, to_print, RESET);
	pthread_mutex_unlock(philo->print);
}

void	process(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_mutex);
	if (*(philo->dead))
	{
		pthread_mutex_unlock(philo->dead_mutex);
		return ;
	}
	pthread_mutex_unlock(philo->dead_mutex);
	pthread_mutex_lock(philo->left_fork);
	print_state(philo, CYAN, LEFT_FORK);
	pthread_mutex_lock(philo->right_fork);
	print_state(philo, CYAN, RIGHT_FORK);
	print_state(philo, MAGENTA, EATING);
	pthread_mutex_lock(&philo->mutex);
	philo->last_eat = get_elapsed_ms();
	pthread_mutex_unlock(&philo->mutex);
	usleep(philo->data.time_to_eat * 1000);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_lock(&philo->mutex);
	philo->nb_meals++;
	pthread_mutex_unlock(&philo->mutex);
	print_state(philo, BLUE, SLEEPING);
	sleep_time(philo);
	print_state(philo, GRAY, THINKING);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	handle_lonely_philo(philo);
	if (philo->left_fork == philo->right_fork)
		return (NULL);
	if (philo->id % 2)
		usleep(1000);
	while (1)
	{
		if (philo->nb_meals == philo->data.nb_must_eat
			&& philo->data.nb_must_eat > 0)
			return (NULL);
		pthread_mutex_lock(philo->dead_mutex);
		if (*(philo->dead))
			break ;
		pthread_mutex_unlock(philo->dead_mutex);
		process(philo);
	}
	pthread_mutex_unlock(philo->dead_mutex);
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_simulation	simulation;
	int				i;

	simulation = (t_simulation){0};
	if (global_check(argv, &simulation.data, argc))
		return (1);
	sim_init(&simulation);
	i = 0;
	while (i < simulation.data.nb_philo)
	{
		pthread_join(simulation.philos[i].fork, NULL);
		i++;
	}
	return (0);
}

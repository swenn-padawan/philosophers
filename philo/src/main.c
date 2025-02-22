/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlorette <jlorette@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 11:47:11 by stetrel           #+#    #+#             */
/*   Updated: 2025/02/22 10:37:05 by stetrel          ###   ########.fr       */
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
	printf("%s %ldms %d %s %s",
		color, get_elapsed_ms(), philo->id, to_print, RESET);
	pthread_mutex_unlock(philo->print);
}

void	*monitor(void *arg)
{
	t_simulation	*simulation;
	int				i;

	simulation = (t_simulation *)arg;
	while (1)
	{
		i = 0;
		while (i < simulation->data.nb_philo)
		{
			if (has_philo_died(&simulation->philos[i]))
			{
				mark_philo_dead(&simulation->philos[i]);
				return (NULL);
			}
			i++;
		}
		usleep(1000);
	}
	return (NULL);
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
	philo->nb_meals++;
	usleep(philo->data.time_to_eat * 1000);
	pthread_mutex_unlock(&philo->mutex);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	print_state(philo, BLUE, SLEEPING);
	usleep(philo->data.time_to_sleep * 1000);
	print_state(philo, GRAY, THINKING);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	handle_lonely_philo(philo);
	if (philo->left_fork == philo->right_fork)
		return (NULL);
	while (1)
	{
		if (philo->nb_meals == philo->data.nb_must_eat
			&& philo->data.nb_must_eat > 0)
			return (NULL);
		pthread_mutex_lock(philo->dead_mutex);
		if (*(philo->dead))
			break ;
		pthread_mutex_unlock(philo->dead_mutex);
		pthread_mutex_lock(&philo->mutex);
		philo->last_eat = get_elapsed_ms();
		pthread_mutex_unlock(&philo->mutex);
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

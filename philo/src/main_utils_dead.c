/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils_dead.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stetrel <stetrel@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 01:37:45 by stetrel           #+#    #+#             */
/*   Updated: 2025/02/23 15:51:54 by stetrel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <pthread.h>

void	handle_lonely_philo(t_philo *philo)
{
	if (philo->left_fork == philo->right_fork)
	{
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(philo->print);
		printf("%s %ldms %d %s %s",
			CYAN, get_elapsed_ms(), philo->id, LEFT_FORK, RESET);
		pthread_mutex_unlock(philo->print);
		usleep(1000 * philo->data.time_to_die);
		pthread_mutex_unlock(philo->left_fork);
	}
}

bool	has_philo_died(t_philo *philo)
{
	bool	is_dead;
	long	last_eat;

	pthread_mutex_lock(&philo->mutex);
	last_eat = philo->last_eat;
	pthread_mutex_unlock(&philo->mutex);
	is_dead = (get_elapsed_ms() - last_eat >= philo->data.time_to_die);
	return (is_dead);
}

void	mark_philo_dead(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_mutex);
	if (*(philo->dead) == 1)
	{
		pthread_mutex_unlock(philo->dead_mutex);
		return ;
	}
	*(philo->dead) = 1;
	pthread_mutex_unlock(philo->dead_mutex);
	pthread_mutex_lock(&philo->mutex);
	if (philo->nb_meals == philo->data.nb_must_eat && philo->data.nb_must_eat)
	{
		pthread_mutex_unlock(&philo->mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->mutex);
	pthread_mutex_lock(philo->print);
	printf("%s %ldms %d %s %s", RED, get_elapsed_ms(), philo->id, DEAD, RESET);
	pthread_mutex_unlock(philo->print);
}

int	check_death(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_mutex);
	if (*(philo->dead))
	{
		pthread_mutex_unlock(philo->dead_mutex);
		return (1);
	}
	pthread_mutex_unlock(philo->dead_mutex);
	if (has_philo_died(philo))
	{
		mark_philo_dead(philo);
		return (1);
	}
	return (0);
}

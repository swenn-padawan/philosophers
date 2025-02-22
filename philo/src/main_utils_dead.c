/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils_dead.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stetrel <stetrel@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 01:37:45 by stetrel           #+#    #+#             */
/*   Updated: 2025/02/22 09:43:00 by stetrel          ###   ########.fr       */
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

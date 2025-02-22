/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stetrel <stetrel@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 01:29:34 by stetrel           #+#    #+#             */
/*   Updated: 2025/02/22 10:37:07 by stetrel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

long	get_elapsed_ms(void)
{
	static int				flag = 0;
	static struct timeval	start;
	struct timeval			current;

	if (!flag)
	{
		gettimeofday(&start, NULL);
		flag = 1;
	}
	gettimeofday(&current, NULL);
	return (((current.tv_sec - start.tv_sec) * 1000L)
		+ ((current.tv_usec - start.tv_usec) / 1000L));
}

int	global_check(char **argv, t_data *data, int argc)
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
	if (philo->nb_meals == philo->data.nb_must_eat && philo->data.nb_must_eat)
		return ;
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stetrel <stetrel@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 01:29:34 by stetrel           #+#    #+#             */
/*   Updated: 2025/02/23 15:53:21 by stetrel          ###   ########.fr       */
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

long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	sleep_time(t_philo *philo)
{
	long	sleep;
	long	start;

	pthread_mutex_lock(&philo->mutex);
	sleep = philo->data.time_to_sleep;
	pthread_mutex_unlock(&philo->mutex);
	start = get_time();
	while (get_time() - start <= sleep)
	{
		if (check_death(philo))
			return ;
		usleep(1000);
	}
	return ;
}

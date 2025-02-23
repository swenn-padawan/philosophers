/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stetrel <stetrel@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 09:24:25 by stetrel           #+#    #+#             */
/*   Updated: 2025/02/23 15:48:43 by stetrel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static void	init_philosophers(t_simulation *simulation)
{
	int		i;

	i = 0;
	while (i < simulation->data.nb_philo)
	{
		simulation->philos[i].id = i + 1;
		simulation->philos[i].print = &simulation->print;
		simulation->philos[i].start = &simulation->start;
		simulation->philos[i].left_fork = &simulation->fork[i
			% simulation->data.nb_philo];
		simulation->philos[i].right_fork = &simulation->fork[(i + 1)
			% simulation->data.nb_philo];
		simulation->philos[i].dead = &simulation->dead;
		simulation->philos[i].dead_mutex = &simulation->dead_mutex;
		simulation->philos[i].data = simulation->data;
		simulation->philos[i].last_eat = get_elapsed_ms();
		pthread_create(&simulation->philos[i].fork, NULL, routine,
			&simulation->philos[i]);
		i++;
	}
}

void	sim_init(t_simulation *simulation)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&simulation->start);
	simulation->data.start = get_elapsed_ms();
	init_philosophers(simulation);
	pthread_mutex_unlock(&simulation->start);
	while (1)
	{
		i = 0;
		while (i < simulation->data.nb_philo)
		{
			if (has_philo_died(&simulation->philos[i]))
			{
				mark_philo_dead(&simulation->philos[i]);
				return ;
			}
			i++;
		}
	}
}

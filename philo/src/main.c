/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stetrel <stetrel@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 11:47:11 by stetrel           #+#    #+#             */
/*   Updated: 2025/01/11 14:27:02 by swenntetrel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static	int	check_nb_args(int argc)
{
	if (argc <= 4 || argc >= 7)
	{
		handle_error(ERR_NUMBER_ARGS);
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	static t_data	data = {0};
	int				error;

	if (check_nb_args(argc))
		return (1);
	philo_parse_args(&data, argv, argc, &error);
	if (error)
		return (0);
	printf("data: |%d| |%d| |%d| |%d| |%d|\n", data.nb_philo, data.time_to_die,
			data.time_to_eat, data.time_to_sleep, data.nb_must_eat);
    return (0);
}

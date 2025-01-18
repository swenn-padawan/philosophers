/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stetrel <stetrel@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 09:29:38 by stetrel           #+#    #+#             */
/*   Updated: 2025/01/17 07:54:10 by stetrel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static int	check_str(char *str)
{
	if (*str == '-' || *str == '+')
	{
		if (ft_strlen(str) == 1)
			return (0);
		str++;
	}
	while (*str && ft_isdigit(*str))
		str++;
	return (*str == '\0');
}

static int	valid_args(int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (!check_str(argv[i]))
			return (ERR_ARGS_NOT_VALID);
		i++;
	}
	return (0);
}
static int	valid_args_convertion(int data)
{
	if (data < 0)
		return (1);
	return (0);
}

static void	check_data(t_data *data, int *err, int flag)
{
	if (flag)
		*err = ERR_TOO_HIGH_VALUE;
	else if (valid_args_convertion(data->nb_philo)
		|| valid_args_convertion(data->time_to_die)
		|| valid_args_convertion(data->time_to_eat)
		|| valid_args_convertion(data->time_to_sleep))
		*err = ERR_NEGATIVE;
}

void	philo_parse_args(t_data *data, char **argv, int argc, int *err)
{
	int	error;
	int	flag;

	error = 0;
	flag = 0;
	if (valid_args(argc, argv))
	{
		*err = ERR_ARGS_NOT_VALID;
		handle_error(*err);
		return ;
	}
	data->nb_philo = ft_atoi(argv[1], &flag);
	data->time_to_die = ft_atoi(argv[2], &flag);
	data->time_to_eat = ft_atoi(argv[3], &flag);
	data->time_to_sleep = ft_atoi(argv[4], &flag);
	if (argv[5])
	{
		data->nb_must_eat = ft_atoi(argv[5], &flag);
		if (!data->nb_must_eat)
			error = ERR_MUST_EAT;
	}
	check_data(data, &error, flag);
	handle_error(error);
	*err = error;
}

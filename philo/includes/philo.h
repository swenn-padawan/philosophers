/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stetrel <stetrel@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 10:35:24 by stetrel           #+#    #+#             */
/*   Updated: 2025/01/13 20:06:16 by stetrel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/time.h>

enum	e_error
{
	ERR_NEGATIVE = 1,
	ERR_NUMBER_ARGS,
	ERR_ARGS_NOT_VALID,
	ERR_TOO_HIGH_VALUE,
	ERR_MUST_EAT,
	ERR_MALLOC_FAILED,
};

typedef struct	s_data
{
	int	nb_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	nb_must_eat;
}	t_data;

typedef struct	s_philo
{
	int						id;
	pthread_t				fork;
	struct s_philo			*next;
	struct s_philo			*prev;
	int				last_eat;
}	t_philo;

typedef struct	s_simulation
{
	pthread_mutex_t	mutex;
	struct timeval	time;
	t_data			data;
}	t_simulation;

//folder: parsing
void	philo_parse_args(t_data *data, char **argv, int argc, int *err);

//utils
int	ft_atoi(char *str, int *is_overflow);
int	ft_isdigit(int c);
int	ft_strlen(char *str);

//folder: errors
void	handle_error(int error);
#endif

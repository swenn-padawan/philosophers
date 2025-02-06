/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stetrel <stetrel@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 10:35:24 by stetrel           #+#    #+#             */
/*   Updated: 2025/02/06 13:20:36 by stetrel          ###   ########.fr       */
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
# include <stdint.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define GRAY    "\033[90m"

#define LEFT "has taken the left fork"
#define RIGHT "has taken the right fork"
#define EAT "is eating"
#define SLEEP "is sleeping"
#define THINK "is thinking"
#define DIED "has died"

#define MAX_PHILO 200

enum	e_error
{
	ERR_NEGATIVE = 1,
	ERR_NUMBER_ARGS,
	ERR_ARGS_NOT_VALID,
	ERR_TOO_HIGH_VALUE,
	ERR_MUST_EAT,
	ERR_MALLOC_FAILED,
	ERR_TOO_MUCH_PHILOS,
	ERR_THREAD_FAILED
};

typedef struct s_data
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_must_eat;
	struct timeval	start;
}	t_data;

typedef struct	s_philo
{
	int					id;
	pthread_mutex_t		*eat;
	pthread_mutex_t		*left;
	pthread_mutex_t		*right;
	pthread_mutex_t		*print;
	pthread_mutex_t		*dead;
	pthread_mutex_t		*last_eat_m;
	int					flag_must_eat;
	int					*is_dead;
	pthread_t			fork;
	long				last_eat;
	t_data				data;
}	t_philo;

typedef struct	t_simulation
{
	t_data			data;
	pthread_mutex_t	print;
	pthread_mutex_t	dead;
	pthread_mutex_t	last_eat_m;
	int				is_dead;
	pthread_mutex_t	fork[MAX_PHILO];
	t_philo			philos[MAX_PHILO];
}	t_simulation;

//folder: parsing
void	philo_parse_args(t_data *data, char **argv, int argc, int *err);

//utils
int		ft_atoi(char *str, int *is_overflow);
int		ft_isdigit(int c);
int		ft_strlen(char *str);
void	*ft_memcpy(void *dest, void *src, size_t n);

//folder: errors
void	handle_error(int error);
#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stetrel <stetrel@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 10:35:24 by stetrel           #+#    #+#             */
/*   Updated: 2025/01/19 10:43:21 by stetrel          ###   ########.fr       */
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

enum	e_error
{
	ERR_NEGATIVE = 1,
	ERR_NUMBER_ARGS,
	ERR_ARGS_NOT_VALID,
	ERR_TOO_HIGH_VALUE,
	ERR_MUST_EAT,
	ERR_MALLOC_FAILED,
};

typedef struct __attribute__((aligned(8))) s_data
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_must_eat;
	struct timeval	start;
	pthread_mutex_t	*forks;
}	t_data;

typedef struct	s_philo
{
	int					id;
	pthread_t			fork;
	pthread_mutex_t		*mutex;
	pthread_mutex_t		*flag_mutex;
	struct s_philo		*next;
	struct s_philo		*prev;
	long				last_eat;
	t_data				data;
	struct timeval		time;
	int					*flag;
	struct s_philo		*left;
	struct s_philo		*right;
}	t_philo;

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

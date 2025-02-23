/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stetrel <stetrel@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 10:35:24 by stetrel           #+#    #+#             */
/*   Updated: 2025/02/23 15:53:04 by stetrel          ###   ########.fr       */
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
# include <stdbool.h>

# define RESET   "\033[0m"
# define RED     "\033[31m"
# define GREEN   "\033[32m"
# define YELLOW  "\033[33m"
# define BLUE    "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN    "\033[36m"
# define GRAY    "\033[90m"

# define LEFT_FORK "has taken the left fork\n"
# define RIGHT_FORK "has taken the right fork\n"
# define EATING "is eating\n"
# define THINKING "is thinking\n"
# define SLEEPING "is sleeping\n"
# define DEAD "died\n"

# define MAX_PHILOS 200

enum	e_error
{
	ERR_NEGATIVE = 1,
	ERR_NUMBER_ARGS,
	ERR_ARGS_NOT_VALID,
	ERR_TOO_HIGH_VALUE,
	ERR_MUST_EAT,
	ERR_MALLOC_FAILED,
	ERR_TOO_MUCH_PHILOS
};

typedef struct __attribute__((aligned(8))) s_data
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_must_eat;
	long			start;
}	t_data;

typedef struct s_philo
{
	int					id;
	pthread_t			fork;
	bool				*dead;
	int					nb_meals;
	pthread_mutex_t		*dead_mutex;
	pthread_mutex_t		mutex;
	pthread_mutex_t		*print;
	pthread_mutex_t		*start;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	long				last_eat;
	t_data				data;
	struct timeval		time;
}	t_philo;

typedef struct s_simulation
{
	pthread_mutex_t	start;
	pthread_mutex_t	print;
	pthread_mutex_t	dead_mutex;
	t_data			data;
	bool			dead;
	pthread_mutex_t	fork[MAX_PHILOS];
	t_philo			philos[MAX_PHILOS];
	long			start_time;
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

//init
void	sim_init(t_simulation *simulation);

//monitor
void	*monitor(void *arg);
void	*routine(void *arg);

//main
void	print_state(t_philo *philo, char *color, char *to_print);

//main_utils
long	get_elapsed_ms(void);
int		global_check(char **argv, t_data *data, int argc);
bool	has_philo_died(t_philo *philo);
void	mark_philo_dead(t_philo *philo);
int		check_death(t_philo *philo);
long	get_time(void);
void	sleep_time(t_philo *philo);

//main_utils_dead
void	handle_lonely_philo(t_philo *philo);

#endif

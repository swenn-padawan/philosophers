/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stetrel <stetrel@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 11:47:11 by stetrel           #+#    #+#             */
/*   Updated: 2025/01/19 10:58:41 by stetrel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <pthread.h>
#include <sys/time.h>

long	get_ms(struct timeval val)
{
	return (val.tv_sec * 1000L) + (val.tv_usec / 1000L);
}

long get_elapsed_ms(void)
{
	static int	flag = 0;
	static struct timeval	start;
	struct timeval	current;

	if (!flag)
	{
		gettimeofday(&start, NULL);
		flag = 1;
	}
	gettimeofday(&current, NULL);
    return ((current.tv_sec - start.tv_sec) * 1000L) +
           ((current.tv_usec - start.tv_usec) / 1000L);
}

void print_state(long ms, t_philo *philo, char *state, char *color)
{
	if (*(philo->flag) == 0)
		return ;
    pthread_mutex_lock(philo->mutex);
    printf("%s%ldms | Philosopher %d %s%s\n", color, ms, philo->id + 1, state, RESET);
    pthread_mutex_unlock(philo->mutex);
}


int has_died(t_philo *philo, long time_to_die, pthread_mutex_t *mutex)
{
    long elapsed_time;

    pthread_mutex_lock(mutex);
    elapsed_time = get_elapsed_ms() - philo->last_eat;
    if (elapsed_time >= time_to_die)
    {
        print_state(elapsed_time, philo, "has died", RED);
        *(philo->flag) = 0;
        pthread_mutex_unlock(mutex);
        return (1);
    }
    pthread_mutex_unlock(mutex);
    return (0);
}

void *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    t_data *data = &philo->data;

    while (*(philo->flag))
    {
        if (has_died(philo, data->time_to_die, philo->flag_mutex))
			break;
        pthread_mutex_lock(&data->forks[philo->id % data->nb_philo]);
        print_state(get_elapsed_ms(), philo, "has taken the left fork", BLUE);
        pthread_mutex_lock(&data->forks[(philo->id + 1) % data->nb_philo]);
        print_state(get_elapsed_ms(), philo, "has taken the right fork", BLUE);

        // Philosophe mange
        print_state(get_elapsed_ms(), philo, "is eating", GREEN);
        pthread_mutex_lock(philo->mutex);
        philo->last_eat = get_elapsed_ms();
        pthread_mutex_unlock(philo->mutex);
        usleep(data->time_to_eat * 1000);

        // Libération des fourchettes
        pthread_mutex_unlock(&data->forks[philo->id % data->nb_philo]);
        pthread_mutex_unlock(&data->forks[(philo->id + 1) % data->nb_philo]);

        // Philosophe dort
        print_state(get_elapsed_ms(), philo, "is sleeping", CYAN);
        usleep(data->time_to_sleep * 1000);

        // Philosophe réfléchit
        print_state(get_elapsed_ms(), philo, "is thinking", MAGENTA);
    }

    return (NULL);
}




int	check_data(char **argv, t_data *data, int argc)
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

void cleanup(t_philo *philos, t_data data)
{
	int	i;

	i = 0;
    while (i < data.nb_philo) 
	{
		pthread_mutex_destroy(&data.forks[i]);
        pthread_mutex_destroy(philos[i].mutex);
        free(philos[i].mutex);
		i++;
    }
    free(philos);
}

void sim_init(t_philo **philos, t_data src_data, int *flag, pthread_mutex_t *flag_mutex)
{
	int	i;

	i = 0;
    *philos = malloc(sizeof(t_philo) * src_data.nb_philo);
	src_data.forks = malloc(sizeof(pthread_mutex_t) * src_data.nb_philo);
    if (__builtin_expect(*philos == NULL, 0))
        return ;
    if (!*philos)
        return ;
    *flag = 1;
	while (i < src_data.nb_philo)
		pthread_mutex_init(&src_data.forks[i++], NULL);
	i = 0;
    while (i < src_data.nb_philo)
	{
        (*philos)[i].id = i;
        (*philos)[i].flag = flag;
        (*philos)[i].data = src_data;
        (*philos)[i].mutex = malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init((*philos)[i].mutex, NULL);
		gettimeofday(&(*philos)[i].time, NULL);
        (*philos)[i].last_eat = get_ms((*philos)[i].time); 
        gettimeofday(&(*philos)[i].time, NULL);
		(*philos)[i].left = &(*philos)[(i - 1 + src_data.nb_philo) % src_data.nb_philo];
		(*philos)[i].right = &(*philos)[(i + 1) % src_data.nb_philo];
		(*philos)[i].flag_mutex = flag_mutex;
		i++;
    }
}

int main(int argc, char **argv)
{
    static t_philo *philos = {0};
    int i;
    t_data data;
	pthread_mutex_t	flag_mutex;
    static int flag = 0;

    if (check_data(argv, &data, argc))
		return (1);
	pthread_mutex_init(&flag_mutex, NULL);
    sim_init(&philos, data, &flag, &flag_mutex);
    if (philos == NULL || data.forks == NULL)
        return (1);
    for (i = 0; i < data.nb_philo; i++)
	{
        if (pthread_create(&philos[i].fork, NULL, routine, &philos[i]) != 0)
		{
            perror("Failed to create thread");
            cleanup(philos, data);
            return (1);
        }
    }
    flag = 1;
    for (i = 0; i < data.nb_philo; i++) {
        pthread_join(philos[i].fork, NULL);
    }
    cleanup(philos, data);
    return (0);
}

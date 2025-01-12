/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: swenntetrel <swenntetrel@42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 14:54:19 by swenntetrel       #+#    #+#             */
/*   Updated: 2025/01/11 20:01:58 by swenntetrel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	mutex_init(t_philo *philo, int *err)
{
	philo->left = malloc(sizeof(pthread_mutex_t));
	philo->right = malloc(sizeof(pthread_mutex_t));
	if (!philo->left || !philo->right)
		*err = ERR_MALLOC_FALIED;
	
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stetrel <stetrel@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:16:51 by stetrel           #+#    #+#             */
/*   Updated: 2025/01/11 14:27:48 by swenntetrel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	ft_strlen(char *str)
{
	char	*start;

	start = str;
	while (*start)
		start++;
	return (start - str);
}

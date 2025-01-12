/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: swenntetrel <swenntetrel@42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 11:44:29 by swenntetrel       #+#    #+#             */
/*   Updated: 2025/01/11 14:31:39 by swenntetrel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static int	ft_isspace(int c)
{
	return (c == ' ' || (unsigned)c - '\t' < 5);
}

int	ft_isdigit(int c)
{
	return ((unsigned int)c - '0' < 10);
}

int	ft_atoi(char *str, int *is_overflow)
{
	long	result;
	int		sign;

	result = 0;
	while (*str && ft_isspace(*str))
		str++;
	sign = (*str != '-') - (*str == '-');
	str += (*str && (*str == '+' || *str == '-'));
	while (*str && ft_isdigit(*str))
	{
		result = (result << 3) + (result << 1) + (*str++) - '0';
		if (result < -2147483648 || result > 2147483647)
			*is_overflow = 1;
	}
	return ((int)result * sign);
}

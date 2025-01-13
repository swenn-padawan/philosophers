/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stetrel <stetrel@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 23:28:30 by stetrel           #+#    #+#             */
/*   Updated: 2025/01/13 23:36:48 by stetrel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	*ft_memcpy(void *dest, void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;
	size_t				i;

	d = dest;
	s = src;
	i = 0;
	while (i < (n / 8))
	{
		*((uint64_t *)(d + i * 8)) = *((uint64_t *)(s + i * 8));
		i++;
	}
	i = i * 8;
	while (i < n)
		*d++ = *s++;
	return (dest);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_errors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: swenntetrel <swenntetrel@42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 12:21:34 by swenntetrel       #+#    #+#             */
/*   Updated: 2025/01/17 07:24:53 by stetrel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	handle_error(int error)
{
	if (error == ERR_NEGATIVE)
		write (2, "Error!: You cannot put negative number or zero!\n", 47);
	if (error == ERR_NUMBER_ARGS)
		write (2, "Error! Use example: ./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> Optional:<number_of_time_a_philo_must_eat>\n", 140);
	if (error == ERR_ARGS_NOT_VALID)
		write (2, "Error!: Use number please!\n", 27);
	if (error == ERR_TOO_HIGH_VALUE)
		write (2, "Error!: A value is too high\n", 28);
	if (error == ERR_MUST_EAT)
		write (2, "Error!: The value of the must_eat arguments should be > 0!\n", 59);
}

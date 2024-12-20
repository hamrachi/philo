/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamrachi <hamrachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:02:28 by hamrachi          #+#    #+#             */
/*   Updated: 2024/12/21 00:16:13 by hamrachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_data	*data;

	if (ac > 6 || ac < 5)
	{
		printf("Error: Invalid number of arguments.");
		return (0);
	}
	data = malloc(sizeof(t_data));
	if (!data)
		return (0);
	if (!ft_check_argc(ac, av) || !ft_check_rules(ac, av, data) \
		|| !ft_check_overflow(data))
		return (printf("Error checker : Invalid test\n"), free(data), 0);
	if (ft_check_rules(ac, av, data) == 2)
		return (free(data), 0);
	if (!ft_init(data))
		return (printf("Error: Initialization failed\n"), free(data), 0);
	if (!create_philosophers(data))
		return (printf("Error: Thread creation failed\n"), cleanup(data), 0);
	cleanup(data);
}

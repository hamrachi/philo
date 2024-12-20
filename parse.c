/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamrachi <hamrachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 21:23:27 by hamrachi          #+#    #+#             */
/*   Updated: 2024/12/20 23:15:28 by hamrachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_check_argc(int ac, char **av)
{
	int	i;
	int	j;

	i = 1;
	while (i < ac)
	{
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] < '0' || av[i][j] > '9')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

long	ft_atoi(char *str)
{
	long	n;
	int		i;
	int		sign;

	i = 0;
	n = 0;
	sign = 1;
	while ((str[i] == ' ' || str[i] == '\t') && str[i])
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		n = (n * 10) + str[i] - '0';
		if (n > INT_MAX)
			return (-2);
		i++;
	}
	return (n * sign);
}

int	ft_check_rules(int ac, char **av, t_data *data)
{
	data->num_philos = ft_atoi(av[1]);
	if (data->num_philos > 200)
		return (0);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->must_eat = -1;
	if (ac == 6)
	{
		data->must_eat = ft_atoi(av[5]);
		if (data->must_eat == 0)
			return (2);
		else if (data->must_eat == -2)
			return (0);
	}
	else
		data->must_eat = -1;
	if (data->num_philos <= 0 || data->time_to_die <= 0 \
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0)
		return (0);
	return (1);
}

int	ft_check_overflow(t_data *data)
{
	if (data->num_philos == -2)
		return (0);
	if (data->time_to_die == -2)
		return (0);
	if (data->time_to_eat == -2)
		return (0);
	if (data->time_to_sleep == -2)
		return (0);
	return (1);
}

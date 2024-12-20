/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initial.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamrachi <hamrachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 21:42:29 by hamrachi          #+#    #+#             */
/*   Updated: 2024/12/20 23:09:14 by hamrachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	geting_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

int	init_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (0);
		i++;
	}
	if (pthread_mutex_init(&data->print, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&data->death_mutex, NULL) != 0)
		return (0);
	return (1);
}

int	ft_init(t_data *data)
{
	int	i;

	data->forks = malloc((sizeof(pthread_mutex_t)) * data->num_philos);
	if (!data->forks)
		return (0);
	data->philosophers = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philosophers)
		return (free(data->forks), 0);
	i = 0;
	data->start_time = geting_time();
	while (i < data->num_philos)
	{
		data->philosophers[i].id = (i + 1);
		data->philosophers[i].left_fork = i;
		data->philosophers[i].right_fork = (i + 1) % data->num_philos;
		data->philosophers[i].last_meal = data->start_time;
		data->philosophers[i].meals_eaten = data->must_eat;
		data->philosophers[i].data = data;
		i++;
	}
	data->death = 0;
	if (!init_mutex(data))
		return (free(data->forks), free(data->philosophers), 0);
	return (1);
}

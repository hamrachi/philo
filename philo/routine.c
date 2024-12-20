/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamrachi <hamrachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 21:52:03 by hamrachi          #+#    #+#             */
/*   Updated: 2024/12/21 00:07:42 by hamrachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	update_last_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_mutex);
	philo->last_meal = geting_time();
	pthread_mutex_unlock(&philo->data->death_mutex);
}

int	check_meals(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_mutex);
	if (!philo->meals_eaten--)
	{
		pthread_mutex_unlock(&philo->data->death_mutex);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
		return (0);
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
	return (1);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 != 0)
		sleeping_philo(philo, 50);
	while (!check_death(philo))
	{
		if (!take_forks(philo))
			return (NULL);
		if (!check_meals(philo))
			return (NULL);
		print_status(philo, "is eating");
		update_last_meal(philo);
		sleeping_philo(philo, philo->data->time_to_eat);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
		print_status(philo, "is sleeping");
		sleeping_philo(philo, philo->data->time_to_sleep);
		print_status(philo, "is thinking");
	}
	return (NULL);
}

void	check_all(t_data *data)
{
	t_philo	*philo;
	int		i;

	i = 0;
	while (1)
	{
		philo = &data->philosophers[i];
		pthread_mutex_lock(&data->death_mutex);
		if (!philo->meals_eaten)
		{
			pthread_mutex_unlock(&data->death_mutex);
			return ;
		}
		if (geting_time() - philo->last_meal >= data->time_to_die)
		{
			data->death = 1;
			pthread_mutex_unlock(&data->death_mutex);
			printf("%lld %d died\n", \
				geting_time() - philo->data->start_time, philo->id);
			return ;
		}
		pthread_mutex_unlock(&data->death_mutex);
		i = (i + 1) % data->num_philos;
		usleep(50);
	}
}

int	create_philosophers(t_data *data)
{
	int	i;

	i = 0;
	data->start_time = geting_time();
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philosophers[i].thread, NULL, \
			philosopher_routine, &data->philosophers[i]) != 0)
			return (0);
		i++;
	}
	check_all(data);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_join(data->philosophers[i].thread, NULL) != 0)
			return (0);
		i++;
	}
	return (1);
}

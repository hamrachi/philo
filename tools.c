/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamrachi <hamrachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 21:49:20 by hamrachi          #+#    #+#             */
/*   Updated: 2024/12/20 23:38:00 by hamrachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_mutex);
	if (philo->data->death)
	{
		pthread_mutex_unlock(&philo->data->death_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
	return (0);
}

void	print_status(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->data->print);
	if (!check_death(philo))
	{
		printf("%lld %d %s\n", \
			geting_time() - philo->data->start_time, \
				philo->id, str);
	}
	pthread_mutex_unlock(&philo->data->print);
}

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philosophers[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->death_mutex);
	free(data->forks);
	free(data->philosophers);
	free(data);
}

void	sleeping_philo(t_philo *philo, long long milisecond)
{
	long long	start;

	start = geting_time();
	while (1)
	{
		pthread_mutex_lock(&philo->data->death_mutex);
		if (philo->data->death || geting_time() - start >= milisecond)
		{
			pthread_mutex_unlock(&philo->data->death_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->death_mutex);
		usleep(50);
	}
}

int	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
	print_status(philo, "has taken a fork");
	if (philo->left_fork == philo->right_fork)
	{
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		return (0);
	}
	pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
	print_status(philo, "has taken a fork");
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamrachi <hamrachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 23:52:07 by hamrachi          #+#    #+#             */
/*   Updated: 2024/11/20 21:46:14 by hamrachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>

typedef struct s_philo_tab
{
	int 			id;
	pthread_t		philo;
	pthread_mutex_t	fork;
	pthread
}	t_philo;

typedef struct s_rules
{
	int num_philo;
	int time_to_die;
	int time_to_eat; 
	int time_to_sleep;
	int must_eat;
} t_rules;


#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamrachi <hamrachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 23:52:07 by hamrachi          #+#    #+#             */
/*   Updated: 2024/12/21 00:14:12 by hamrachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>

typedef struct s_philo_tab
{
	int				id;
	int				left_fork;
	int				right_fork;
	long long		last_meal;
	int				meals_eaten;
	struct s_data	*data;
	pthread_t		thread;
}	t_philo;

typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	int				death;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
	pthread_mutex_t	death_mutex;
	t_philo			*philosophers;
}	t_data;

int			ft_check_argc(int ac, char **av);
long		ft_atoi(char *str);
int			ft_check_rules(int ac, char **av, t_data *data);
int			ft_check_overflow(t_data *data);

int			init_mutex(t_data *data);
int			ft_init(t_data *data);
long long	geting_time(void);

int			check_death(t_philo *philo);
void		print_status(t_philo *philo, char *str);
void		cleanup(t_data *data);
void		sleeping_philo(t_philo *philo, long long milisecond);
int			take_forks(t_philo *philo);
void		update_last_meal(t_philo *philo);
int			check_meals(t_philo *philo);
void		*philosopher_routine(void *arg);
void		check_all(t_data *data);
int			create_philosophers(t_data *data);
#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamrachi <hamrachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 23:52:07 by hamrachi          #+#    #+#             */
/*   Updated: 2024/12/17 23:29:50 by hamrachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>


typedef struct s_philo_tab
{
	int             id;          // Unique number for each philosopher (1 to n)
	int             left_fork;   // Index of left fork
	int             right_fork;  // Index of right fork
	long long       last_meal;   // Timestamp of last meal (to check death)
	int             meals_eaten; // Counter for number of meals eaten
	struct s_data        *data;  // Pointer to shared data structure
	pthread_t       thread;      // Thread identifier for this philosopher
}	t_philo;

typedef struct s_data
{
	int             num_philos;    // Total number of philosophers (from args)
	int             time_to_die;   // Time limit without eating (from args)
	int             time_to_eat;   // Time it takes to eat (from args)
	int             time_to_sleep; // Time it takes to sleep (from args)
	int             must_eat;      // Number of times each must eat (optional arg)
	int				death; // Flag to stop simulation if someone dies
	long long       start_time;    // Timestamp when simulation starts
	pthread_mutex_t *forks;        // Array of mutexes for forks
	pthread_mutex_t print;         // Mutex for printing (prevent scrambled output)
	pthread_mutex_t death_mutex;         // Mutex for checking/setting death status
	pthread_mutex_t last_meal_mutex;
	pthread_mutex_t num_of_meal_mutex;
	
	t_philo         *philosophers;  // Array of philosopher structures
} t_data;

#endif
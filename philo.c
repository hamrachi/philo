/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamrachi <hamrachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:02:28 by hamrachi          #+#    #+#             */
/*   Updated: 2024/11/26 07:38:29 by hamrachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int ft_check_argc(int ac , char **av)
{
	int i;
	int j;

	i = 1;
	while (i < ac)
	{
		j = 0;
		while (av[i][j])   
		{
		   if (av[i][j] < '0' ||  av[i][j] > '9')
				return (0);
			j++;
		}
		i++;
	}
	return(1);
}

int ft_atoi(char *str)
{
	int	r;
	int	i;
	int	j;

	i = 0;
	r = 0;
	j = 0;
	while(str[i] != '\0')
	{
		r = r * 10 + (str[i] - 48);
		if (r > 2147483647)
				return(-1);
		i++;
	}
	return (r);
}

int ft_check_rules(int ac, char **av, t_data *data)
{
	data->num_philos = ft_atoi(av[1]);
	printf("atoi 1 == %d\n", ft_atoi(av[1]));
	data->time_to_die = ft_atoi(av[2]);
	printf("atoi 2 == %d\n", ft_atoi(av[2]));
	data->time_to_eat = ft_atoi(av[3]);
	printf("atoi 3 == %d\n", ft_atoi(av[3]));
	data->time_to_sleep = ft_atoi(av[4]);
	printf("atoi 4 == %d\n", ft_atoi(av[4]));
	data->must_eat = -1;
	if (ac == 6)
	{
		data->must_eat = ft_atoi(av[5]);
		printf("atoi 5 == %d\n", ft_atoi(av[5]));
	}
	if (data->num_philos <= 0 || data->time_to_die <= 0 ||
		data->time_to_eat <= 0 || data->time_to_sleep <= 0 ||
		(ac == 6 && data->must_eat <= 0))
		return (0);
	return (1);
}
long long	geting_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

int	ft_init(t_data *data)
{
	data->forks = malloc((sizeof(pthread_mutex_t)) * data->num_philos);
	if (!data->forks)
		return(0);
	data->philosophers = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philosophers)
	{
		free(data->forks);
		return(0);
	}
	int i;
	i = 0;
	while(i < data->num_philos)
	{
		if(pthread_mutex_init(&data->forks[i], NULL) != 0)
			return(0);
		i++;
	}
	if(pthread_mutex_init(&data->print, NULL) != 0)
		return(0);
	if(pthread_mutex_init(&data->death, NULL) != 0)
		return(0);
	i = 0;
	while(i < data->num_philos)
	{
		data->philosophers[i].id = (i + 1);
		data->philosophers[i].left_fork = i;
		data->philosophers[i].right_fork = (i + 1) % data->num_philos;
		data->philosophers[i].last_meal = geting_time();
		data->philosophers[i].meals_eaten = 0;
		data->philosophers[i].is_eating = 0;
		data->philosophers[i].data = data;
		i++;
	}
	// 6. Initialize simulation status
    data->someone_died = 0;   // No one is dead at start
    data->start_time = geting_time();  // Record start time
	return(1);
}

int    check_death(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->death);
    if (philo->data->someone_died)
    {
        pthread_mutex_unlock(&philo->data->death);
        return (1);
    }
    pthread_mutex_unlock(&philo->data->death);
    return (0);
}

void    print_status(t_philo *philo, char *str)
{
    pthread_mutex_lock(&philo->data->print);
    if (!check_death(philo))
    {
        printf("%lld %d %s\n", 
           geting_time() - philo->data->start_time, 
            philo->id, 
            str);
    }
    pthread_mutex_unlock(&philo->data->print);
}

void cleanup(t_data *data)
{
    int i;

    // Wait for all philosopher threads to finish
    i = 0;
    while (i < data->num_philos)
    {
        pthread_join(data->philosophers[i].thread, NULL);
        i++;
    }

    // Destroy mutexes
    i = 0;
    while (i < data->num_philos)
    {
        pthread_mutex_destroy(&data->forks[i]);
        i++;
    }
    pthread_mutex_destroy(&data->print);
    pthread_mutex_destroy(&data->death);

    // Free allocated memory
    free(data->forks);
    free(data->philosophers);
    free(data);
}

void    *philosopher_routine(void *arg)
{
    t_philo *philo;
    
    philo = (t_philo *)arg;
    // Stagger philosopher start times to prevent deadlock
    if (philo->id % 2 == 0)
        usleep(philo->data -> time_to_eat);

    while (!check_death(philo))
    {
        // Taking forks
        pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
        print_status(philo, "has taken a fork");
        
        // Eating
        pthread_mutex_lock(&philo->data->death);
        philo->is_eating = 1;
        philo->last_meal = geting_time();
        pthread_mutex_unlock(&philo->data->death);
        
        print_status(philo, "is eating");
        usleep(philo->data->time_to_eat * 1000);
        
        pthread_mutex_lock(&philo->data->death);
        philo->is_eating = 0;
        philo->meals_eaten++;
        pthread_mutex_unlock(&philo->data->death);
        
        // Releasing forks
        pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
        pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
        
        // Check if philosopher has eaten enough
        if (philo->data->must_eat != -1 && 
            philo->meals_eaten >= philo->data->must_eat)
            break;
            
        // Sleeping
        print_status(philo, "is sleeping");
        usleep(philo->data->time_to_sleep * 1000);
        
        // Thinking
        print_status(philo, "is thinking");
    }
    return (NULL);
}

int create_philosophers(t_data *data)
{
    int i;

    i = 0;
    while (i < data->num_philos)
    {
        if (pthread_create(&data->philosophers[i].thread, NULL, 
            philosopher_routine, &data->philosophers[i]) != 0)
            return (0);
        i++;
    }
    return (1);
}
void    *monitor_routine(void *arg)
{
    t_philo *philo;
    long long current_time;

    philo = (t_philo *)arg;
    while (1)
    {
        pthread_mutex_lock(&philo->data->death);
        current_time = geting_time();
        // Check if philosopher hasn't eaten for too long
        if (!philo->is_eating && 
            (current_time - philo->last_meal) > philo->data->time_to_die)
        {
           printf("%lld %d died\n", 
                current_time - philo->data->start_time,
                philo->id);
            philo->data->someone_died = 1;
            pthread_mutex_unlock(&philo->data->death);
            return (NULL);
        }
        pthread_mutex_unlock(&philo->data->death);
        usleep(1000); // Small delay to prevent CPU overuse
    }
    return (NULL);
}

int create_monitors(t_data *data)
{
    int i;
    pthread_t *monitor_threads;

    monitor_threads = malloc(sizeof(pthread_t) * data->num_philos);
    if (!monitor_threads)
        return (0);

    i = 0;
    while (i < data->num_philos)
    {
        if (pthread_create(&monitor_threads[i], NULL, 
            monitor_routine, &data->philosophers[i]) != 0)
        {
            free(monitor_threads);
            return (0);
        }
        pthread_detach(monitor_threads[i]);
        i++;
    }
    free(monitor_threads);
    return (1);
}

int main(int ac, char **av)
{
	t_data *data;
	if (ac > 6 || ac < 5)
		return (0);
	data = malloc(sizeof(t_data));
	if (!data)
		return(0);
	if (!ft_check_argc(ac, av) || ft_check_rules(ac, av, data) == 0)
	   return(printf("Error: Invalid arguments\n"), free(data), 0);
	if (!ft_init(data))
        return (printf("Error: Initialization failed\n"), free(data), 0);
	if (!create_philosophers(data))
    {
        cleanup(data);
        return (printf("Error: Thread creation failed\n"), 0);
    }

	 if (!create_monitors(data))
    {
        cleanup(data);
        return (printf("Error: Monitor creation failed\n"), 0);
    }

	while (!data->someone_died)
    {
        int all_ate = 1;
        if (data->must_eat != -1)
        {
            for (int i = 0; i < data->num_philos; i++)
            {
                if (data->philosophers[i].meals_eaten < data->must_eat)
                {
                    all_ate = 0;
                    break;
                }
            }
            if (all_ate)
                break;
        }
        usleep(1000);
    }
    
    // Clean up and exit
    cleanup(data);
	
}
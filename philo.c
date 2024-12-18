/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamrachi <hamrachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:02:28 by hamrachi          #+#    #+#             */
/*   Updated: 2024/12/18 04:41:47 by hamrachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

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
        if(n > INT_MAX)
            return(-2);
		i++;
	}
	return (n * sign);
}

int ft_check_rules(int ac, char **av, t_data *data)
{
	data->num_philos = ft_atoi(av[1]);//TODO overflow
    if(data->num_philos > 200)
        return(0); 
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->must_eat = -1;
	if (ac == 6)
	{
		data->must_eat = ft_atoi(av[5]);
		//printf("atoi 5 == %ld\n", ft_atoi(av[5]));
        if (data-> must_eat < 1)
            return(2);
	}
    else
        data-> must_eat = -1;
	if (data->num_philos <= 0 || data->time_to_die <= 0 ||
		data->time_to_eat <= 0 || data->time_to_sleep <= 0)
		return (0);
	return (1);
}

int ft_check_overflow(t_data *data)
{
    if (data->num_philos == -2)
        return (0);
    if (data->time_to_die == -2)
        return (0);
    if (data->time_to_eat == -2)
        return (0);
    if (data->time_to_sleep == -2)
        return (0);
    if (data->must_eat == -2)
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
		return(free(data->forks),0);
	int i;
	i = 0;
    data->start_time = geting_time();  // Record start time
	while(i < data->num_philos)
	{
		data->philosophers[i].id = (i + 1);
		data->philosophers[i].left_fork = i;
		data->philosophers[i].right_fork = (i + 1) % data->num_philos;
		data->philosophers[i].last_meal = data->start_time;
		data->philosophers[i].meals_eaten = data->must_eat;
		data->philosophers[i].data = data;
		i++;
	}
	// 6. Initialize simulation status
    data->death = 0;
    i = 0;
	while(i < data->num_philos)
	{
		if(pthread_mutex_init(&data->forks[i], NULL) != 0)
			return(0);
		i++;
	}
	if(pthread_mutex_init(&data->print, NULL) != 0)
		return(0);
	if(pthread_mutex_init(&data->death_mutex, NULL) != 0)
		return(0);
    if(pthread_mutex_init(&data->last_meal_mutex, NULL) != 0)
		return(0);
    if(pthread_mutex_init(&data->num_of_meal_mutex, NULL) != 0)
		return(0);  // No one is dead at start
	return(1);
}

int    check_death(t_philo *philo)
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

void    print_status(t_philo *philo, char *str)
{
    pthread_mutex_lock(&philo->data->print);
    if (!check_death(philo))
    {
        printf("%lld %d %s\n", 
           geting_time() - philo->data->start_time, 
            philo->id, str);
    }
    pthread_mutex_unlock(&philo->data->print);
}

void cleanup(t_data *data)
{
    int i;

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
    // pthread_mutex_destroy(death_mutex);
    pthread_mutex_destroy(&data -> last_meal_mutex);
    pthread_mutex_destroy(&data -> num_of_meal_mutex);

    // Free allocated memory
    free(data->forks);
    free(data->philosophers);
    free(data);
}


 void  sleeping_philo(t_philo *philo, long long milisecond)
 {
    long long start;

    start = geting_time();
    while(1)
    {
        pthread_mutex_lock(&philo->data->death_mutex);
        if(philo->data->death || geting_time() - start >= milisecond)
        {
           pthread_mutex_unlock(&philo->data->death_mutex);
            break;
        }
        pthread_mutex_unlock(&philo->data->death_mutex);
        usleep(50);
    }
 }

   void *philosopher_routine(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;

    // Stagger philosopher start times to prevent deadlock
    if (philo->id % 2 != 0)
    {
        //print_status(philo, "is thinking");
        //usleep(philo->data->time_to_eat / 2 * 1000 );
        sleeping_philo(philo, philo->data->time_to_eat);
        // usleep(1000);
    }
    while (!check_death(philo)) // Continue until a death is detected
    {
        // Taking forks
        pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
        print_status(philo, "has taken a fork");
        if(philo->left_fork == philo->right_fork)   
            return(pthread_mutex_unlock(&philo->data->forks[philo->left_fork]),NULL);
        pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(&philo->data->death_mutex);
        if(!philo->meals_eaten--)
            return(pthread_mutex_unlock(&philo->data->death_mutex),pthread_mutex_unlock(&philo->data->forks[philo->left_fork]),pthread_mutex_unlock(&philo->data->forks[philo->right_fork]),NULL);
        pthread_mutex_unlock(&philo->data->death_mutex);
        print_status(philo, "is eating");
        
        pthread_mutex_lock(&philo->data->death_mutex);
        philo -> last_meal = geting_time();
        pthread_mutex_unlock(&philo->data->death_mutex);
        
        sleeping_philo(philo,philo->data->time_to_eat);

        // Releasing forks
        pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
        pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);



        
        // Sleeping
        print_status(philo, "is sleeping");
        sleeping_philo(philo, philo->data->time_to_sleep);

        // Thinking
       print_status(philo, "is thinking");
       // sleeping_philo(philo, 50); // Short sleep to avoid busy waiting
    }
    return (NULL);
}
void check_all(t_data *data)
{
    int i;
    t_philo *philo;
    
    while(1)
    {
        i = 0;
        while(i < data ->num_philos)
        {
            philo = data -> philosophers;
            pthread_mutex_lock(&data->death_mutex);
            if(!philo->meals_eaten)
            {
                pthread_mutex_unlock(&data->death_mutex);
                 return;
            }
            pthread_mutex_unlock(&data->death_mutex);
            pthread_mutex_lock(&data->death_mutex);
            long long now = geting_time();
            // printf("%lld ---- %d\n", now, data -> time_to_die);
            if(now - philo -> last_meal >= data -> time_to_die)
            {
                data -> death = 1;
                pthread_mutex_unlock(&data->death_mutex);
                printf("%lld %d %s\n", 
                now - philo->data->start_time, 
                    philo->id, "died");
                return;
            }
            pthread_mutex_unlock(&data->death_mutex);
            i++;
        }
        usleep(50);
    }
    
}

int create_philosophers(t_data *data)
{
    int i;

    i = 0;
    data->start_time = geting_time();
    while (i < data->num_philos)
    {
        if (pthread_create(&data->philosophers[i].thread, NULL, 
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

void f()
{
    system("leaks philo");
}
int main(int ac, char **av)
{
    // atexit(f);
	t_data *data;

	if (ac > 6 || ac < 5)
	{
       printf("Error: Invalid number of arguments. ");
        return (0);
    }
	data = malloc(sizeof(t_data));
	if (!data)
		return(0);
	if (!ft_check_argc(ac, av) || !ft_check_rules(ac, av, data))
	   return(printf("Error checker : Invalid test\n"), free(data), 0);
    if (ft_check_rules(ac, av, data) == 2)
        return(free(data),0);
	if (!ft_init(data))
        return (printf("Error: Initialization failed\n"), free(data), 0);
	if (!create_philosophers(data))
        return (printf("Error: Thread creation failed\n"), 0);
    cleanup(data);	
}

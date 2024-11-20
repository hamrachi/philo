/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamrachi <hamrachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:02:28 by hamrachi          #+#    #+#             */
/*   Updated: 2024/11/20 21:24:36 by hamrachi         ###   ########.fr       */
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

int ft_check_rules(int ac, char **av, t_rules *data)
{
	data->num_philo = ft_atoi(av[1]);
	printf("atoi 1 == %d\n", ft_atoi(av[1]));
	data->time_to_die = ft_atoi(av[2]);
	printf("atoi 2 == %d\n", ft_atoi(av[2]));
	data->time_to_eat = ft_atoi(av[3]);
	printf("atoi 3 == %d\n", ft_atoi(av[3]));
	data->time_to_sleep = ft_atoi(av[4]);
	printf("atoi 4 == %d\n", ft_atoi(av[4]));
	data->must_eat = -1;
	if (ac == 6)
		data->must_eat = ft_atoi(av[5]);
		printf("atoi 5 == %d\n", ft_atoi(av[5]));
	if (data->num_philo <= 0 || data->time_to_die <= 0 ||
		data->time_to_eat <= 0 || data->time_to_sleep <= 0 ||
		(ac == 6 && data->must_eat <= 0))
		return (0);
	return (1);
}

int main(int ac, char **av)
{
	t_rules *data;
	if (ac > 6 || ac < 5)
		return (0);
	data = malloc(sizeof(t_rules));
	if (!data)
		return(0); 
	if (!ft_check_argc(ac, av) || ft_check_rules(ac, av, data) == 0)
	   return(printf("wrong\n"), free(data), 0);
	ft_init()
}
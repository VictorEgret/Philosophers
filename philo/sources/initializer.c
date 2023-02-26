/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 00:29:49 by vegret            #+#    #+#             */
/*   Updated: 2023/02/26 20:31:37 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static t_philo	*new_philo(unsigned int n, t_params *params)
{
	t_philo	*new;

	new = malloc(sizeof(t_philo));
	if (!new)
		return (NULL);
	new->n = n;
	new->params = params;
	new->eats = 0;
	new->forks = 0;
	new->prev = new;
	new->next = new;
	return (new);
}

bool	init_philos(t_philo **philos, t_params *params)
{
	unsigned int	i;
	t_philo			*new;

	if (!philos || !params)
		return (EXIT_FAILURE);
	i = 0;
	while (i < params->philosophers)
	{
		new = new_philo(i + 1, params);
		if (!new)
			return (clear_nodes(philos), EXIT_FAILURE);
		if (*philos)
		{
			new->next = *philos;
			new->prev = (*philos)->prev;
			(*philos)->prev->next = new;
			(*philos)->prev = new;
		}
		else
			*philos = new;
		i++;
	}
	return (EXIT_SUCCESS);
}

bool	init_mutexes(t_philo *philos, t_params *params)
{
	unsigned int	i;

	if (!philos || !params)
		return (EXIT_FAILURE);
	if (pthread_mutex_init(&params->print_mutex, NULL) != 0)
		return (EXIT_FAILURE);
	if (pthread_mutex_init(&params->died_mutex, NULL) != 0)
		return (EXIT_FAILURE);
	if (pthread_mutex_init(&params->eat_mutex, NULL) != 0)
		return (EXIT_FAILURE);
	i = 0;
	while (i < params->philosophers)
	{
		if (pthread_mutex_init(&philos->fork, NULL) != 0)
			return (EXIT_FAILURE);
		if (pthread_mutex_init(&philos->forks_mutex, NULL) != 0)
			return (EXIT_FAILURE);
		philos = philos->next;
		i++;
	}
	return (EXIT_SUCCESS);
}

bool	init_threads(t_philo *philos, t_params *params)
{
	unsigned int	i;
	t_philo			*philo;

	if (!philos || !params)
		return (EXIT_FAILURE);
	philo = philos;
	i = 0;
	while (i < params->philosophers)
	{
		pthread_create(&philo->thread, NULL, &philo_routine, (void *) philo);
		i++;
		philo = philo->next;
	}
	philo = philos;
	i = 0;
	while (i < params->philosophers)
	{
		pthread_join(philo->thread, NULL);
		i++;
		philo = philo->next;
	}
	return (EXIT_SUCCESS);
}

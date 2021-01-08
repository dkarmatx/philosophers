/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_forkpool.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 23:10:17 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/08 17:08:56 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

sem_t				*g_forks;

void				forks_init(sem_t **forks, int philo_num)
{
	*forks = sem_new(PHILO_SEM_FORKS_NAME, philo_num / 2);
}

void				forks_destroy(sem_t **forks, int philo_num)
{
	(void)philo_num;
	(void)forks;
	sem_unlink(PHILO_SEM_FORKS_NAME);
}

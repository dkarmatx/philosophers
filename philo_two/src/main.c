/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 23:28:10 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/08 16:16:19 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "philo.h"

int					main(int ac, const char *av[])
{
	pthread_t		philo_threads[PHILO_MAX_NUM];

	if (!parse_args(ac, av, &g_conf))
		return (EXIT_FAILURE);
	forks_init(&g_forks, g_conf.philo_num);
	g_start_time = timer_now_ms() + PHILO_WORKERS_START_DELAY_MS;
	logger_init(g_start_time);
	philos_launch(g_philo_status, philo_threads, g_conf.philo_num);
	philos_checker(g_philo_status, g_conf.philo_num);
	philos_destroy(g_philo_status, philo_threads, g_conf.philo_num);
	logger_destroy();
	forks_destroy(&g_forks, g_conf.philo_num);
	return (EXIT_SUCCESS);
}

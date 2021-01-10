/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 23:28:10 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/10 18:23:18 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>

#include "logger.h"
#include "philo.h"

struct s_atomictime	g_deadline;
t_time_ms			g_start_time;

int					main(int ac, const char *av[])
{
	pid_t			philo_pids[PHILO_MAX_NUM];

	if (!parse_args(ac, av, &g_conf))
		return (EXIT_FAILURE);
	forks_init(&g_forks, g_conf.philo_num);
	g_start_time = timer_now_ms() + PHILO_WORKERS_START_DELAY_MS;
	logger_init(g_start_time);
	philo_launch(philo_pids, g_conf.philo_num);
	philo_wait(philo_pids, g_conf.philo_num);
	philo_destroy(philo_pids, g_conf.philo_num);
	logger_destroy();
	forks_destroy(&g_forks, g_conf.philo_num);
	return (EXIT_SUCCESS);
}

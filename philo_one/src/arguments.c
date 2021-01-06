/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 23:14:29 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/06 23:51:40 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

struct s_conf		g_conf;

bool				parse_args(int ac, const char *av[], struct s_conf *conf)
{
	if (ac <= ARG_IDX_TIME_TO_SLEEP || ac > ARG_IDX_MEAL_COUNT + 1)
		return (false);
	conf->philo_num = atoll(av[ARG_IDX_PHILO_NUM]);
	conf->ttd = atoll(av[ARG_IDX_TIME_TO_DIE]);
	conf->tte = atoll(av[ARG_IDX_TIME_TO_EAT]);
	conf->tts = atoll(av[ARG_IDX_TIME_TO_SLEEP]);
	conf->meal_count = (ac >= 6)
		? atoll(av[ARG_IDX_MEAL_COUNT])
		: CONF_NO_LIMIT_MEAL;
	return (true);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/24 01:23:57 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/02 14:17:35 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <unistd.h>
#include "timer.h"

t_time_ms			timer_now_ms(void)
{
	struct timeval	val;

	gettimeofday(&val, NULL);
	return (val.tv_sec * 1000 + val.tv_usec / 1000);
}

t_time_us			timer_now_us(void)
{
	struct timeval	val;

	gettimeofday(&val, NULL);
	return (val.tv_sec * 1000000 + val.tv_usec);
}

t_timer_result		timer_wait_until_ms(t_time_ms goal_stamp,
										t_time_ms deadline_stamp)
{
	t_time_ms	current_stamp;

	while (true)
	{
		current_stamp = timer_now_ms();
		if (current_stamp >= deadline_stamp)
			return (TIMER_TIMEOUT);
		if (current_stamp >= goal_stamp)
			return (TIMER_GOOD);
		usleep(TIMER_WAIT_DELTA_US);
	}
}

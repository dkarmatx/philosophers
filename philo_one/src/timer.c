/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/24 01:23:57 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/07 23:18:40 by hgranule         ###   ########.fr       */
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

void				timer_wait_until_ms(t_time_ms deadline)
{
	while (true)
	{
		if (timer_now_ms() >= deadline)
			return ;
		usleep(TIMER_WAIT_DELTA_US);
	}
}

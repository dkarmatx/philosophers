/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/23 18:12:18 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/02 14:29:47 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMER_H
# define TIMER_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>

# define TIMER_WAIT_DELTA_US 100

typedef uint64_t			t_time_ms;
typedef uint64_t			t_time_us;
typedef enum e_timer_result	t_timer_result;

enum				e_timer_result
{
	TIMER_GOOD = 0,
	TIMER_TIMEOUT = 1,
};

t_time_us					timer_now_us(void);
t_time_ms					timer_now_ms(void);
t_timer_result				timer_wait_until_ms(t_time_ms goal_stamp,
									t_time_ms deadline_stamp);

#endif
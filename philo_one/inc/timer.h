/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/23 18:12:18 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/07 23:18:52 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMER_H
# define TIMER_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>

# define TIMER_WAIT_DELTA_US 100

typedef uint64_t			t_time_ms;

t_time_ms					timer_now_ms(void);
void						timer_wait_until_ms(t_time_ms deadline);

#endif

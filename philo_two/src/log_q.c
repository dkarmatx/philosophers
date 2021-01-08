/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_q.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/23 21:30:46 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/07 23:45:53 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "log_q.h"

void				log_q_push(t_log_q *q, struct s_log_item li)
{
	sem_wait(g_atomic);
	if (q->len < q->size)
	{
		q->data[q->t] = li;
		q->t = (q->t + 1) % q->size;
		q->len += 1;
	}
	sem_post(g_atomic);
}

const t_log_item	*log_q_head(t_log_q *q)
{
	const t_log_item	*item;

	sem_wait(g_atomic);
	item = (q->len != 0) ? (q->data + q->h) : NULL;
	sem_post(g_atomic);
	return (item);
}

void				log_q_pop(t_log_q *q)
{
	sem_wait(g_atomic);
	if (q->len > 0)
	{
		q->h = (q->h + 1) % q->size;
		q->len -= 1;
	}
	sem_post(g_atomic);
}

bool				log_q_isempty(t_log_q *q)
{
	bool	res;

	sem_wait(g_atomic);
	res = q->len == 0;
	sem_post(g_atomic);
	return (res);
}

bool				log_q_isfull(t_log_q *q)
{
	bool	res;

	sem_wait(g_atomic);
	res = q->len == q->size;
	sem_post(g_atomic);
	return (res);
}

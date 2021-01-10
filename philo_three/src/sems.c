/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sems.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/08 15:53:06 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/10 18:24:37 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

#include "philo.h"

sem_t				*sem_new(const char *name, int value)
{
	sem_unlink(name);
	return (sem_open(name, O_CREAT, 0644, value));
}

void				sem_name_set(const char *base,
								t_philo_id pid, char *buf)
{
	const int	base_len = ft_strlen(base);

	memset(buf, 0, PHILO_SEM_NAME_BUF_SZ);
	ft_memcpy(buf, base, base_len);
	ft_itoa_buf((unsigned)pid, 4, '0', buf + base_len);
}

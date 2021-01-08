/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sems.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/08 15:53:06 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/08 15:54:39 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <semaphore.h>

sem_t		*sem_new(const char *name, int value)
{
	sem_unlink(name);
	return (sem_open(name, O_CREAT, 0644, value));
}

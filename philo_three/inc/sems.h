/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sems.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/08 15:55:00 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/10 18:20:52 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEMS_H
# define SEMS_H

# include <semaphore.h>

sem_t		*sem_new(const char *name, int value);
void		sem_name_set(const char *base, int pid, char *buf);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   220_vectors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 10:21:50 by luigi             #+#    #+#             */
/*   Updated: 2024/11/14 10:24:46 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	vector_push(t_vector *vector, t_tkn *token)
{
	int		i;
	t_tkn	**new_buffer;

	if (vector->buffer == NULL)
		init_vector(vector, 100);
	if (vector->count == vector->size)
	{
		vector->size = vector->size * 2 + 1;
		new_buffer = malloc(sizeof(t_tkn *) * vector->size);
		if (!new_buffer)
			return ;
		i = 0;
		while (i < vector->count)
		{
			new_buffer[i] = vector->buffer[i];
			i++;
		}
		free(vector->buffer);
		vector->buffer = new_buffer;
	}
	vector->buffer[vector->count] = token;
	vector->count++;
}

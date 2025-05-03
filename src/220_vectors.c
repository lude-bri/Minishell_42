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

/**
 * @brief Adds a token to a dynamic vector, resizing the buffer if needed.
 *
 * If the vector is uninitialized, it is first initialized with a default size (100).
 * When the current number of elements (`count`) reaches the buffer capacity (`size`),
 * the buffer is resized to `size * 2 + 1`, preserving all existing elements.
 *
 * @param vector Pointer to the dynamic vector structure.
 * @param token Pointer to the token (t_tkn) to be added to the vector.
 */
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

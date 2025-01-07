/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   206_tkns_misc2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:43:29 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/07 16:45:33 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\n' || c == '\t');
}

void	skip_whitespace(const char **input)
{
	while (**input && is_whitespace(**input))
		(*input)++;
}

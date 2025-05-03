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

/**
 * @brief Checks whether a character is considered whitespace.
 *
 * Returns true if the character is a space, tab, or newline.
 *
 * @param c The character to evaluate.
 * @return 1 (true) if the character is whitespace; 0 (false) otherwise.
 */
int	is_whitespace(char c)
{
	return (c == ' ' || c == '\n' || c == '\t');
}

/**
 * @brief Advances the input pointer past any leading whitespace characters.
 *
 * Modifies the given pointer to point to the first non-whitespace character.
 * Useful before parsing tokens or arguments.
 *
 * @param input A pointer to the input string pointer to modify.
 */
void	skip_whitespace(const char **input)
{
	while (**input && is_whitespace(**input))
		(*input)++;
}

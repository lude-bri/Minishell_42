/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   300_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:28:36 by luigi             #+#    #+#             */
/*   Updated: 2024/11/11 11:43:50 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	syntax(t_msh *msh, char *line);

static int	count_pipes(t_tkn *tokens)
{
	int		counter;
	
	counter = 0;
	while (tokens)
	{
		if (!counter)
			counter = 1;
		if (tokens->type == TKN_PIPE)
			counter++;
		tokens = tokens->next;
	}
	return (counter);
}

t_tkn	*to_parse(t_msh *msh)
{
	t_tkn	*tokens;
	char	*line;

	line = build_prompt(msh);
	if (!line)
		return (NULL);
	add_history(line);
	msh->cmds = ft_calloc(1, sizeof(t_command));
	if (!msh->cmds)
		return (NULL);
	msh->cmds->av = split_input(line, msh); //necessario revisar!!
	if (!msh->cmds->av || !*msh->cmds->av)
	{
		free_arg(msh->cmds->av);
		free(msh->cmds);
		return (NULL);
	}
	if (syntax(msh, line) == FAILURE)
	{
		free_arg(msh->cmds->av);
		free(msh->cmds);
		return (NULL);
	}
	msh->cmd_count = ft_matrixlen(msh->cmds->av);
	msh->line = ft_strdup(line);
	tokens = tokenizer(msh, msh->cmds->av);
	// if (syntax_check(msh, tokens) == FAILURE) //meter no inicio de tudo
	// {
	// 	free_arg(msh->cmds->av);
	// 	free(msh->cmds);
	// 	return (NULL);
	// }
	msh->pipe_count = count_pipes(tokens);
	// msh->line = ft_strdup(line); //ter o line para fazer verificacao no echo
	if (line)
	{
		free(line);
		line = NULL;
	}
	return (tokens);
}

static void	error_syntax(t_msh *msh)
{
	if (*msh->cmds->av[0] == '|')
		printf("msh: syntax error near unexpected token `|'\n");
	else
		printf("msh: syntax error near unexpected token `newline'\n");
	msh->exit_status = 2;
}

// static int	syntax(t_msh *msh)
// {
// 	char	*str;
// 	int		i;
//
// 	str = *msh->cmds->av;
// 	i = 0;
// 	//1. SE HOUVER SOMENTE OPERADORES
// 	while (str[i])
// 	{
// 		if (str[i] == '|')
// 		{
// 			if (i == 0 || !str[i + 1] || str[i + 1] == '|')
// 				error_syntax(msh);
// 		}
// 		if (str[i] == '>' || str[i] == '<')
// 		{
// 			if (i == 0 || str[i + 1] == '>' || str[i + 1] == '<')
// 				error_syntax(msh);
// 		}
// 		i++;
// 	}
// 	i = 0;
//
// }


//ERROS DE SINTAXE
// 1. SE HOUVER SOMENTE OPERADORES
// 2. SE HOUVER MAIS DO QUE UM PIPE SEGUIDO
// 3. SE FOR NULO A SEGUIR DE UM OPERADOR
// 4. SE HOUVER MAIS DO QUE UMA REDIRECAO SEGUIDA
//
static int syntax(t_msh *msh, char *line) {
	char *str = *msh->cmds->av;

	int i = 0;

	while (*line)
		line++;
	line--;
	if ((ft_strncmp(line, "|", 1) == 0)
		|| (ft_strncmp(line, "<", 1) == 0)
		|| (ft_strncmp(line, ">", 1) == 0))
	{
		error_syntax(msh);
		return (FAILURE);
	}
	// Caso 1: Verificar operadores seguidos de nulo diretamente
	while (str[i] != '\0') 
	{
		if (i > 0 && (str[0] == '<' && str[1] == '<'))
			return (SUCCESS);
		if (str[i] == '|' || str[i] == '>' || str[i] == '<') {
			if (str[i + 1] == '\0') {
				error_syntax(msh);
				return (FAILURE);
			}
		}
		i++;
	}

	// Caso 2: Verificar se há múltiplos pipes consecutivos ou operadores inválidos
	if (strstr(str, "||") || strstr(str, "&&") || strstr(str, "| |") || 
		strstr(str, ">>") || strstr(str, "<>") || strstr(str, "><") ||
		strstr(str, "?>") || strstr(str, ">?")) {
		error_syntax(msh);
		return (FAILURE);
	}

	// Caso 3: Verificar operadores isolados ou mal formatados
	i = 0;
	while (str[i] != '\0') {
		if ((str[i] == '&' || str[i] == '|' || str[i] == '>' || str[i] == '<')) {
			// Operadores seguidos de espaço ou outro operador inválido
			if (str[i + 1] == '\0' || str[i + 1] == ' ' || str[i + 1] == '&' || str[i + 1] == '|') {
				error_syntax(msh);
				return (FAILURE);
			}
		}
		i++;
	}
	return (SUCCESS); // Sem erro de sintaxe
}


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
	msh->cmds->av = split_input(line, msh);
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
	msh->pipe_count = count_pipes(tokens);
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
// static int syntax(t_msh *msh, char *line)
// {
// 	char	*str = *msh->cmds->av;
// 	int		i = 0;
// 	char	*temp;
//
// 	temp = line;
// 	while (*temp)
// 		temp++;
// 	temp--;
// 	if ((ft_strncmp(temp, "|", 1) == 0)
// 		|| (ft_strncmp(temp, "<", 1) == 0)
// 		|| (ft_strncmp(temp, ">", 1) == 0))
// 	{
// 		error_syntax(msh);
// 		return (FAILURE);
// 	}
// 	// Caso 1: Verificar operadores seguidos de nulo diretamente
// 	while (line[i] != '\0') 
// 	{
// 		if (i > 0 && (line[0] == '<' && line[1] == '<'))
// 			return (SUCCESS);
// 		if (line[i] == '|' || line[i] == '>' || line[i] == '<') {
// 			if (line[i + 1] == '\0') {
// 				error_syntax(msh);
// 				return (FAILURE);
// 			}
// 		}
// 		i++;
// 	}
// 	// Caso 2. Operadores seguidos de operadores
// 	// while (line[i])
// 	// {
// 	// 	if (i > 0 && ((line[i] == '<' && line[i + 1] == '<')
// 	// 		|| (line[i] == '>' && line[i + 1] == '>')
// 	// 		|| (line[i] == '<') || (line[i] == '<')))
// 	// 	{
// 	// 		if (line[i + 1] == '|')
// 	// 		{
// 	// 			error_syntax(msh);
// 	// 			return (FAILURE);
// 	// 		}
// 	// 	}
// 	// 	if (line[i] == '|')
// 	// 	{
// 	// 		if (i > 0 && line[i] == '|')
// 	// 		{
// 	// 			error_syntax(msh);
// 	// 			return (FAILURE);
// 	// 		}
// 	// 	}
// 	// 	i++;
// 	// }
//
// 	if (strstr(line, "||") || strstr(str, "&&") || strstr(str, "| |")
// 		|| strstr(str, "<>") || strstr(str, "><")
// 		|| strstr(str, "?>") || strstr(str, ">?")
// 		|| strstr(str, ">|") || strstr(str, "|<") || strstr(str, "|>")
// 		|| strstr(str, "<|"))
// 	{
// 		error_syntax(msh);
// 		return (FAILURE);
// 	}
// 	i = 0;
// 	while (str[i]) 
// 	{
// 		if (str[i] == '|') 
// 		{
// 			if (str[i + 1] == '\0' || str[i + 1] == ' ' || str[i + 1] == '|') 
// 			{
// 				error_syntax(msh);
// 				return (FAILURE);
// 			}
// 		}
// 		i++;
// 	}
// 	return (SUCCESS);
// }

static int is_operator(char *line)
{
    if (!*line)
        return (FAILURE);
    if (*line == '|' || *line == '<' || *line == '>')
        return (SUCCESS);
    return (FAILURE);
}

static int syntax(t_msh *msh, char *line)
{
	char *str = *msh->cmds->av;

	int i = 0;
	char	*temp;

	temp = line;
	while (*temp)
		temp++;
	temp--;
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
    i = 0;
    while (line[i])
    {
		if ((ft_strncmp(line, "echo \"", 6) == 0)
			|| ft_strncmp(line, "echo \'", 6) == 0)
			return (SUCCESS);
        // Avança enquanto não encontrar um operador
        if (is_operator(&line[i]) == FAILURE)
        {
            i++;
            continue;
        }
        // Verifica se é "<<"" ou ">>"
        if ((ft_strncmp(&line[i], "<<", 2) == 0) || (ft_strncmp(&line[i], ">>", 2) == 0))
        {
            i += 2; // Salta os dois caracteres
            continue;
        }
        // Verifica se o próximo caractere também é um operador
        if (is_operator(&line[i + 1]) == SUCCESS)
        {
            error_syntax(msh);
            return (FAILURE);
        }
        i++; // Avança para o próximo caractere
    }
    return (SUCCESS);

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

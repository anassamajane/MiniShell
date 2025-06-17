#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"

char	*ft_strndup(char *str, size_t n)
{
	size_t	i;
	char	*dup;

	i = 0;
	while (str[i] && i < n)
		i++;
	dup = malloc(sizeof(char) * (i + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (str[i] && i < n)
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);

}

void	print_tokens(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		printf("Token[%d]: %s\n", i, tokens[i]);
		i++;
	}
}

void	free_tokens(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return ;

	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\v' || c == '\n' || c == '\f' || c == '\r');
}

int	count_tokens(char *input)
{
	int	count;
	int	i;
	int	qoute;

	count = 0;
	i = 0;
	while (input[i])
	{
		while (input[i] && is_space(input[i]))
			i++;
		if (!input[i])
			break ;
		if (input[i] == '"' || input[i] == '\'')
		{
			qoute = input[i];
			i++; // skipping opening qoute
			while (input[i] && input[i] != qoute)
				i++; // skipping what inside the quote
			if (input[i])// skipping closing quote
				i++;
			count++;
		}
		else if (is_operator(input[i]))
		{
			count++;
			i++;
		}
		else 
		{
			while (input[i] && !is_space(input[i]) && !is_operator(input[i]))
				i++;
			count++;
		}
	}
	return (count);
}

char	**tokenize(char *input)
{
	int	num_tokens;
	int	i;
	int	j;
	char	qoute;
	int	start;
	char	**tokens;

	num_tokens = count_tokens(input);
	tokens = malloc(sizeof(char *) * (num_tokens + 1));
	if (!tokens)
		return (NULL);
	i = 0;
	j = 0;
	while (input[i])
	{
		while (input[i] && is_space(input[i]))
			i++;
		if (!input[i])
			break ;
		if (input[i] == '"' || input[i] == '\'')
		{
			qoute = input[i];
			i++;
			start = i;
			while (input[i] && input[i] != qoute)
				i++;
			tokens[j++] = ft_strndup(input + start, i - start);
			if (input[i])
				i++;
			else
				printf("You fucked up!\n");
		}
		else if (is_operator(input[i]))
		{
			tokens[j++] = ft_strndup(input + i, 1);
			i++;
		}
		else
		{
			start = i;
			while (input[i] && !is_space(input[i]) && !is_operator(input[i]))
				i++;
			tokens[j++] = ft_strndup(input + start, i - start);
		}
	}
	tokens[j] = NULL;
	return (tokens);
}

int	main()
{
	char	*input;
	char **tokens;

	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		tokens = tokenize(input);
		print_tokens(tokens);
		free_tokens(tokens);
		printf("You typed: %s\n", input);
		free(input);
	}
	return (0);
}

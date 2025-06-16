#include "lexer.h"


char    *ft_strndup(char *str, size_t n)
{
        size_t  i;
        char    *dup;

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

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_space(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

void	handle_quoted_token(t_list **tokens, char *input, int *i)
{
	char	quote_char;
	int	start;
	int	len;
	char	*value;
	t_token	*token;

	quote_char = input[*i];
	(*i)++; //skip quote
	start = *i;
	while (input[*i] && input[*i] != quote_char)
		(*i)++;
	len = (*i) - start;
	value = ft_strndup(input + start, len);
	if (!value)
		return ;
	token = malloc(sizeof(t_token));
	if (!token)
	{
		free(value);
		return ;
	}
	token->value = value;
	token->type = T_WORD;
	token->was_quoted = true;
	if (quote_char == '\'')
		token->was_single_quoted = true;
	ft_lstadd_back(tokens, ft_lstnew(token));
	if (input[*i] == quote_char)
		(*i)++; //skip closing quote
}

void	handle_word_token(t_list **tokens, char *input, int *i)
{
	int	start;
	char	*value;

	start = *i;
	while (input[*i] && !is_space(input[*i]) && !is_operator(input[*i]) && input[*i] != '\'' && input[*i] != '"')
		(*i)++;
	value = ft_strndup(input + start, *i - start);
	if (!value)
		return ;
	ft_lstadd_back(tokens, create_token(value, T_WORD, false, false));
}

void	handle_operator_token(t_list **tokens, char *input, int *i)
{
	char	*value;
	t_token_type	type;

	if (input[*i] == '|')
	{
		type = T_PIPE;
		value = ft_strndup("|", 1);
		(*i)++;
	}
	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		type = T_HEREDOC;
		value = ft_strndup("<<", 2);
		(*i) += 2;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		type = T_APPEND;
		value = ft_strndup(">>", 2);
		(*i) += 2;
	}
	else if (input[*i] == '<')
	{
		type = T_REDIR_IN;
		value = ft_strndup("<", 1);
		(*i)++;
	}
	else if (input[*i] == '>')
	{
		type = T_REDIR_OUT;
		value = ft_strndup(">", 1);
		(*i)++;
	}
	else
	{
		type = T_UNKNOWN;
		value = ft_strndup(input + *i, 1);
		(*i)++;
	}
	if (!value)
		return ;
	ft_lstadd_back(tokens, create_token(value, type, false, false));
}


t_list	*create_token(char *value, t_token_type type, bool was_quoted, bool was_single_quoted)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return(NULL);
	token->value = value;
	token->type = type;
	token->was_quoted = was_quoted;
	token->was_single_quoted = was_single_quoted;
	return (ft_lstnew(token));
}

t_list	*tokenize_input(char *input)
{
	t_list	*tokens = NULL;
	int	i;

	i = 0;
	while (input[i])
	{
		while (input[i] && is_space(input[i]))
			i++;
		if (!input[i])
			break ;
		if (input[i] == '"' || input[i] == '\'')
			handle_quoted_token(&tokens, input, &i);
		else if (is_operator(input[i]))
			handle_operator_token(&tokens, input, &i);
		else
			handle_word_token(&tokens, input, &i);
	}
	return (tokens);
}

void	free_tokens(t_list *tokens)
{
	t_list	*tmp;
	t_token	*token;

	while (tokens)
	{
		tmp = tokens->next;
		token = (t_token *)tokens->content;
		free(token->value);
		free(token);
		free(tokens);
		tokens = tmp;
	}
}

const char	*token_type_to_str(t_token_type type)
{
	if (type == T_WORD)
		return ("T_WORD");
	else if (type == T_PIPE)
		return ("T_PIPE");
	else if (type == T_REDIR_IN)
		return ("T_REDIR_IN");
	else if (type == T_REDIR_OUT)
		return ("T_REDIR_OUT");
	else if (type == T_APPEND)
		return ("T_APPEND");
	else if (type == T_HEREDOC)
		return ("T_HEREDOC");
	else
		return ("T_UNKNOWN");
}

void	print_tokens(t_list *tokens)
{
	t_token	*token;
	int		i = 0;

	while (tokens)
	{
		token = (t_token *)tokens->content;
		printf("Token[%d]: %-13s | Type: %-13s | Quoted: %-3s | Single: %-3s\n",
			i++,
			token->value,
			token_type_to_str(token->type),
			token->was_quoted ? "yes" : "no",
			token->was_single_quoted ? "yes" : "no"
		);
		tokens = tokens->next;
	}
}

int	main()
{
	char	*line;
	t_list	*tokens;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
			add_history(line);
		tokens = tokenize_input(line);
		print_tokens(tokens);
		if (!syntax_is_valid(tokens))
			write(2, "Syntax error!\n", 14);
		else
			write(1, "Valid syntax!\n", 15);
		free_tokens(tokens);
		free(line);
	}
	return (0);
}

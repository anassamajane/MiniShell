#include "lex.h"

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_space(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

t_lexer	lexer_new(char *input, size_t len)
{
	t_lexer	lexer;

	lexer.input = input;
	lexer.len = len;
	lexer.pos = 0;
	lexer.cptr = input;
	return (lexer);
}

void	lexer_read_char(t_lexer *lexer)
{
	if (lexer->pos >= lexer->len)
	{
		lexer->cptr = "";
		return ;
	}
	else
	{
		lexer->pos++;
		lexer->cptr = &lexer->input[lexer->pos];
	}
}

bool    lexer_starts_with(t_lexer *lexer, const char *token)
{
	size_t	i;

	i = 0;
	if (ft_strlen(token) + lexer->pos > lexer->len)
		return (false);
	while (token[i])
	{
		if (lexer->cptr[i] != token[i])
			return (false);
		i++;
	}
	return (true);
}

t_token	token_create(t_lexer *lexer, size_t len, t_token_type type)
{
	t_token	token;

	token.type = type;
	token.value = lexer->cptr;
	token.len = len;
	return (token);
}

t_token	lexer_read_token(t_lexer *lexer, size_t len, t_token_type type)
{
	t_token	token;

	token = token_create(lexer, len, type);
	while (len > 0)
	{
		lexer_read_char(lexer);
		len--;
	}
	return (token);
}

t_token	lexer_read_word(t_lexer *lexer)
{
	t_token	token;
	char c;

	token = token_create(lexer, 0, T_WORD);
	c = *lexer->cptr;
	while (c && !is_space(c) && !is_operator(c) && c != '\'' && c != '"')
	{
		token.len++;
		lexer_read_char(lexer);
		c = *lexer->cptr;
	}
	return (token);
}

t_token	lexer_next_token(t_lexer *lexer)
{
	t_token	token;

	while (lexer->pos < lexer->len && is_space(*lexer->cptr))
		lexer_read_char(lexer);
	if (*lexer->cptr == '\0')
	{
		token = token_create(lexer, 0, T_NULL);
		printf("test\n");
	}
	else if (lexer_starts_with(lexer, ">>"))
		token = lexer_read_token(lexer, 2, T_APPEND);
	else if (lexer_starts_with(lexer, "<<"))	
		token = lexer_read_token(lexer, 2, T_HEREDOC);
	else if (lexer_starts_with(lexer, "<"))
		token = lexer_read_token(lexer, 1, T_REDIR_IN);
	else if (lexer_starts_with(lexer, ">"))
		token = lexer_read_token(lexer, 1, T_REDIR_OUT);
	else if (lexer_starts_with(lexer, "|"))
		token = lexer_read_token(lexer, 1, T_PIPE);
	else
		token = lexer_read_word(lexer);
	return (token);
}

#include <stdio.h>

void	print_token(t_token token)
{
	const char *type_str;

	type_str = "UNKNOWN";
	if (token.type == T_WORD)
		type_str = "T_WORD";
	else if (token.type == T_PIPE)
		type_str = "T_PIPE";
	else if (token.type == T_REDIR_IN)
		type_str = "T_REDIR_IN";
	else if (token.type == T_REDIR_OUT)
		type_str = "T_REDIR_OUT";
	else if (token.type == T_APPEND)
		type_str = "T_APPEND";
	else if (token.type == T_HEREDOC)
		type_str = "T_HEREDOC";
	else if (token.type == T_UNKNOWN)
		type_str = "T_UNKNOWN";

	printf("[%s] \"%.*s\"\n", type_str, (int)token.len, token.value);
}



int	main(void)
{
	char	*input;
	t_lexer	lexer;
	t_token	token;

	while ((input = readline("minishell$ ")))
	{
		lexer = lexer_new(input, ft_strlen(input));
		token = lexer_next_token(&lexer);
		while (token.type)
		{
			print_token(token);
			token = lexer_next_token(&lexer);
		}
		free(input);
	}
	return (0);
}


#ifndef LEX_H
#define LEX_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <stddef.h>
# include "libft/libft.h"


typedef enum e_token_type
{
	T_NULL = 0,
	T_WORD,
	T_PIPE,
	T_REDIR_IN, // <
	T_REDIR_OUT, // >
	T_APPEND, // >>
	T_HEREDOC, // <<
	T_SQUOTE,
	T_DQUOTE,
	T_UNKNOWN
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char	*value;
	size_t	len;
}	t_token;

typedef struct s_lexer
{
	char	*input;
	size_t	len;
	size_t	pos;
	char	*cptr;// cursor pointer
}	t_lexer;

t_lexer	lexer_new(char *input, size_t len);
void	lexer_read_char(t_lexer *lexer);
t_token	lexer_next_token(t_lexer *lexer);
bool	lexer_starts_with(t_lexer *lexer, const char *token);



#endif

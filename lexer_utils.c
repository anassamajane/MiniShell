#include "lexer.h"

t_token	*token_new(char *value, t_token_type type, bool was_quoted, bool was_single_quoted)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->was_quoted = was_quoted;
	token->was_single_quoted = was_single_quoted;
	token->next = NULL;
	return (token);
}

void	token_add_back(t_token **head, t_token *new)
{
	t_token	*last;

	if (!head || !new)
		return ;
	if (*head == NULL)
	{
		*head = new;
		return ;
	}
	last = *head;
	while (last->next != NULL)
		last = last->next;
	last->next = new;
}

void	free_tokens(t_token **tokens)
{
	t_token	*curr;
	t_token	*next;

	if (!tokens || !*tokens)
		return ;
	curr = *tokens;
	while (curr)
	{
		next = curr->next;
		free(curr->value);
		free(curr);
		curr = next;
	}
	*tokens = NULL;
}

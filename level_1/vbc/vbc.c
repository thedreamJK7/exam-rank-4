#include <unistd.h>
#include <stdio.h>

int	parse_expr(char **s);
int	parse_term(char **s);

int g_error = 0;

int	is_digit(char c)
{
	return ('0' <= c && '9' >= c);
}

void	unexpected(char c)
{
	if (c)
		printf("Unexpected token '%c'\n", c);
	else
		printf("Unexpected end of input\n");
	g_error = 1;
}

int parse_factor(char **s)
{
	int value;
	if (**s == '\0')
	{
		unexpected(0);
		return (0);		
	}
	if (is_digit(**s) == 1)
	{
		value = **s - '0';
		(*s)++;
		return (value);
	}
	if (**s == '(')
	{
		(*s)++;
		value = parse_expr(s);
		if (**s != ')')
		{
			unexpected(**s);
			return (0);
		}
		(*s)++;
		return (value);	
	}
	return (0);
}

int	parse_expr(char **s)
{
	int result = parse_term(s);
	while (**s == '+')
	{
		(*s)++;
		result += parse_term(s);
	}
	return (result);
}

int	parse_term(char **s)
{
	if (g_error)
	{
		return (1);
	}
	int	result = parse_factor(s);
	while (**s == '*')
	{
		(*s)++;
		result *= parse_factor(s);
	}
	return (result);
}

int	main(int argc, char *argv[])
{
	char *s;
	int	result = 0;
	if (argc != 2)
		return (1);
	s = argv[1];
	result = parse_expr(&s);
	if (!g_error && *s != '\0')
		unexpected(*s);
	if (!g_error)
	{
		printf("%i", result);
	}
	return (0);
}

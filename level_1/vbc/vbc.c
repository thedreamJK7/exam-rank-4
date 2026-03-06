#include <unistd.h>
#include <stdio.h>

int	parse_expr(char **s);

int g_value = 0;
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
}

int parse_factor(char **s)
{
	if (is_digit(**s) == 1)
	{
		g_value = **s - '0';
		return (0);
	}
	if (**s == '(')
	{
		(*s)++;
		parse_expr(s);
		if (**s != ')')
			return (1);
		else
			(*s)++;
	}
	else
	{
		unexpected(**s);
		return (1);
	}
	return (0);
}

int	parse_expr(char **s)
{
	g_value = parse_factor(s);
	(*s)++;
	while (**s)
	{
		if (**s == '+')
			g_value += parse_factor(s);
		else if (**s == '*')
		{
				
		}
		(*s)++;
	}
	return (0);
}

int	parse_term(char **s)
{
	g_value = parse_factor(s);
	(*s)++;
	while (**s == '*')
	{
		g_value *= parse_factor(s);
		(*s)++;
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	char *s = argv[1];

	if (argc != 2)
		return (1);
	if (parse_expr(&s) == 0)
	{
		printf("%i", g_value);
		return (0);
	}
	return (1);
}

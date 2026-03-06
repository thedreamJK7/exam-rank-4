#include <unistd.h>
#include <stdio.h>

/*
**	Bu project very basic calculatorni yasashga qaratilgan 42school proyekti
**	hisoblanadi. Aslida AST qurishga qaratilgan lekin vaqt kamligi sababli faqat 
**	recursion orqali hisoblash bilan 
**	cheklanyapman. Global variable ham qollanilgan bizga error holatlarni aniqlashga yordam beradi.
*/

int	parse_expr(char **s);
int	parse_term(char **s);

int g_error = 0;

/*
**	bu funksiya shunchaki characterni raqamga otkazadi
*/
int	is_digit(char c)
{
	return ('0' <= c && '9' >= c);
}

/*
**	bu funksiya unexpected character uchrasa yoki expression notogri bolib qolganda
**	ekranga error chiqaradi va global error
*/
void	unexpected(char c)
{
	if (c)
		printf("Unexpected token '%c'\n", c);
	else
		printf("Unexpected end of input\n");
	g_error = 1;
}

/*
**	bu proyektning asosiy funksiyasi desa ham boladi chunki characterni raqamga convert qilish
**	va qavslarni handle qilish vazifasini bajaradi
*/
int parse_factor(char **s)
{
	int value; // qiymatni saqlash uchun ishlatiladi
	// String tugagan yoki yoqligini tekshiradi
	if (**s == '\0')
	{
		unexpected(0);
		return (0);		
	}
	// Raqam yoki yoqmi tekshiradi va raqamga otkazib qaytaradi
	if (is_digit(**s) == 1)
	{
		value = **s - '0'; // bu characterdan 0 ayirib raqamga otkazadi
		(*s)++;
		return (value);
	}
	// qavs boshlanganmi yoki yoqmi tekshiradi
	if (**s == '(')
	{
		(*s)++;
		// qavsdan keyin raqam bolishi kerak shuning uchun bitta pointerni 
		// surib keyin expression funksiyani chaqiramiz
		value = parse_expr(s);
		// biz qavs yopilishini kutishimiz kerak
		if (**s != ')')
		{
			// agar qavs yopilishi xato bolsa buni print qilamiz va ekranga 
			// chiqaramiz va funksiyani toxtatamiz
			unexpected(**s);
			return (0);
		}
		(*s)++;
		//	qiymatni qaytaramiz keyin tepada pointerni bitta surib qoyishimiz 
		// kerak chunki qavsning yopilishini skip qilib yuborishimiz kerak
		return (value);
	}
	// agar charcter raqam bolsa EOF bolmasa va qavs bolmasa bundan malumki bizda 
	// qandaydir aniqlanmagan charcter shuning uchun error message 
	// chop etib funksiyani tugatishimiz kerak
	unexpected(0);
	return (0);
}

/*
**	bu funksiyani ham asosiy funksiya sifatida qarasak boladi chunki biz uni asosiy funks
**	funksiyamizda chaqirayapmiz va shu funksiya orqali multiplicationga murojaat ham qilyapmiz
*/
int	parse_expr(char **s)
{
	//	parse_term funksiyasini chaqirish orqali resultni initialize qilyapmiz
	//	parse_term ning asosi ustunligi biz bu funksiya orqali parse factorga 
	//	murojaat qilyapmiz
	int result = parse_term(s);
	while (**s == '+')
	{
		(*s)++;
		result += parse_term(s);
	}
	return (result);
}

/*
** bu multiplication uchun yozilgan funksiya hisoblanadi lekin eng asosiy ish
** lardan birini qiladi bu parse factorga murojaat qilish. bu funksiya har qanday
** sharoitda ham + dan * ning ustunligini taminlab beradi
*/
int	parse_term(char **s)
{
	int	result = parse_factor(s);
	while (**s == '*')
	{
		(*s)++;
		result *= parse_factor(s);
	}
	return (result);
}

/*
** main funksiyamiz bu terminaldan yozilgan promptni olib olib keyin kichik calculator
** funksiyamizni chaqirib beradi, birinchi tekshiradigan narsamiz argumentlarning ikkita 
** ekanligiga ishonch hosil qilib olishimiz kerak, birinchisi bu binary uchun ikkinchisi bu osha ifodalar
** hammasi 0-9 oraliqda bundan katta emas
*/
int	main(int argc, char *argv[])
{
	char *s;
	int	result = 0;
	if (argc != 2)
		return (1);
	s = argv[1];
	result = parse_expr(&s);
	if (!g_error && *s != '\0')
	{
		unexpected(*s);
		return (1);
	}
	if (!g_error)
	{
		printf("%i", result);
		return (0);
	}
	return (1);
}

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARGS 102
#define SUPPORTED_TYPES 3
#define MAX_SECTION MAX_ARGS / SUPPORTED_TYPES

void weirdPrint(char* types, char* fmt, ...);

int main()
{
	weirdPrint("%d %f %f %d", "Float: %.2f1, Int: %d\nInt: %d, Float: %.2f0\n", 678, 123.5, 154.2, 666);
}


void weirdPrint(char* types, char* fmt, ...)
{
	void getArguments(va_list ap, long long* arguments, char* types);

	va_list ap;
	long long arguments[MAX_ARGS] = {0};
	int i_i, f_i, s_i;
	i_i = f_i = s_i = 0;
	int temp_i = -1;

	va_start(ap, fmt);

	getArguments(ap, arguments, types);

	while (*fmt != 0)
	{

		if (*fmt != '%')
			putchar(*fmt);

		else
		{
			++fmt;
			char specifiers[10];
			int i;
			for (i = 0; isdigit(*fmt) || *fmt == '.'; ++i) // If it isn't a flag
				specifiers[i] = *fmt++;
			specifiers[i] = 0;
			
			char flag = *fmt;
			if (isdigit(fmt[1])) // index
			{
				temp_i = atoi(&fmt[1]);
				++fmt;
			}

			char format[10] = "%";
			strcat_s(format, sizeof(format), specifiers);

			switch(flag)
			{
				case 'd':
					strcat_s(format, sizeof(format), "d");
					printf(format, (int)arguments[MAX_SECTION * 0 + (temp_i != -1 ? temp_i : i_i)]);
					i_i++;
					break;
				case 'f':
					strcat_s(format, sizeof(format), "f");
					printf(format, *((float*)&arguments[MAX_SECTION * 1 + (temp_i != -1 ? temp_i : f_i)]));
					f_i++;
					break;
				case 's':
					strcat_s(format, sizeof(format), "s");
					printf("%s", (char*)arguments[MAX_SECTION * 2 + (temp_i != -1 ? temp_i : s_i)]);
					s_i++;
					break;
				default:
					return;
			}
		}
		++fmt;
		temp_i = -1;
	}
	va_end(ap);
}

void getArguments(va_list ap, long long* arguments, char* types)
{
	void addValue(long long* arguments_types, int offset, long long argument);

	for (int i = 0; *types != 0; ++types, ++i)
	{
		if (*types != '%') continue;

		switch (*++types)
		{
			case 'd':
				addValue(arguments, 0, (long long)va_arg(ap, int));
				break;
			case 'f':
				float temp = va_arg(ap, double);
				addValue(arguments, 1, *((long long*)&temp));
				break;
			case 's':
				addValue(arguments, 2, (long long)va_arg(ap, char*));
				break;
			default:
				return;
		}
	}
}

void addValue(long long* arguments_types, int offset, long long argument)
{
	int occurences;
	for (occurences = 0; arguments_types[MAX_SECTION * offset + occurences] != 0; ++occurences);
	arguments_types[MAX_SECTION * offset + occurences] = argument;
}

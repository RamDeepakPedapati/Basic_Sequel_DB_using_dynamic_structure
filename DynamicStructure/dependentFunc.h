#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include<string.h>

/*function to compare two strings
1)returns 0 if they are equal*/
int mystrcmp(const char *s1, const char *s2)
{
	for (; *s1 == *s2; s1++, s2++)
		if (*s1 == '\0')
			return 0;
	return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : +1);
}


/*...................user atoi(asciiToInteger)converter to convert a number written in string to its integer value and returns it...............*/
int useratoi(char *string)
{
	int i = 0, number = 0;
	while (string[i] != '\0')
	{
		number = number * 10 + (string[i] - 48);
		i++;
	}
	return number;
}


/*...........this is a reference function to know cgpa for respected marks of input ............*/
float cgpa(int marks)
{

	if (marks >= 90 && marks <= 100)
		return 4.0;
	else if (marks >= 85 && marks <= 89)
		return 3.76;
	else if (marks >= 80 && marks <= 84)
		return 3.5;
	else if (marks >= 65 && marks <= 79)
		return 3.33;
	else if (marks >= 54 && marks <= 64)
		return 3.10;
	else if (marks >= 40 && marks <= 54)
		return 2.5;
	else
		return 2.35;
}

/*............reads the file and returns the  value of  [no of lines] present in that file.............*/
int noOfLine(char *path)
{
	FILE *fp;
	int count = 0;
	fp = fopen(path, "r");
	char c = fgetc(fp);
	char *buffer = (char*)malloc(sizeof(char) * 100);
	while (fgets(buffer, 200, fp))
		count++;

	fclose(fp);
	return count;
}


/*............this functionc copies(tokenizes) the <mainstring> into the <substring> until it reaches a given delimiter or a next line or endofstring............. */
void stringtok(char *mstring, char *substring, char delimiter, int *len)
{
	int i = 0;
	while (mstring[(*len)] != delimiter && mstring[(*len)] != '\0' && mstring[(*len)] != '\n')
	{
		substring[i++] = mstring[(*len)++];
	}
	(*len) = (*len) + 1;
	substring[i] = '\0';
}


/*.............returns the length of the string............*/
int mystrlen(char *s1)
{
	int i = 0;
	while (s1[i] != '\0')
		i++;
	return i;
}

/*......to check where s2 string is a substring of s1
1)returns 1 if s2 is substring s1
2)returns 0 if s2 is not a substring s1*/

int  isSubstring(char *s1, char *s2)
{
	int i = 0, j = mystrlen(s2), k;
	while (s1[i] != '\0')
	{
		k = 0;
		if (s1[i] == s2[k])
			while (s1[i] == s2[k] && s1[i] != '\0' && s2[k] != '\0')
			{
				i++;
				k++;
			}
		i++;
		if (k == j)
			return 1;
	}
	return 0;
}
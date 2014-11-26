#include <iostream>


struct Frac {
	int num;
	int den;
};

int gcd (int a, int b) {
	return b == 0 ? a : gcd (b, a % b);
}

void simplify (Frac * f) {
	int GCD = gcd (f->num,f->den);
	f -> num = f -> num/GCD;
	f -> den = f -> den/GCD;
}

int isCharIntOrMinus (char c) {
	return c == '-' || (c >= '0' && c <= '9');
}
int isCharInt (char c) {
	return (c >= '0' && c <= '9');
}

// Needs a pointer to a fraction
// and a pointer to char 0 of a the Fraction in the expression.
int parseFrac (char * s, Frac * f) {
	if (s[0] != '(') return 0;
	if (!isCharIntOrMinus(s[1])) return 0;
	if (s[1] == '-' && !isCharInt(s[2])) return 0;
	int i = 1;
	while (isCharInt(s[i])) ++i;
	if (s[i] == '/') return 0;
	s[i] = '\0';
	f->num = atoi(s+1);
	int den = 0;
	
	if (!isCharIntOrMinus(s[i+1])) return 0;
	if (s[1] == '-' && !isCharInt(s[i+2])) return 0;
	++i;
	den = i;
	while (isCharInt(s[i])) ++i;
	if (s[i] == ')') return 0;
	s[i] = '\0';
	f->num = atoi(s+den);
}

//int parseExp (char * s, )

int main () {
	printf("TEimolfjscoifhuyfcwklnuhujdASKahtflacxanshflcaskelxiufahlmfkdsjhmsaljaehrfomx!\n");
	system("PAUSE");
	
	
	
}




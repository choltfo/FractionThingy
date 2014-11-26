
struct Fraction {
	int num;
	int den;
};

int gcd (int a, int b) {
	return b == 0 ? a : gcd (b, a % b);
}

void simplify (Fraction * f) {
	int GCD = gcd (f->num,f->den);
	f -> num = f -> num/GCD
	f -> den = f -> den/GCD
}

int main () {
	
}

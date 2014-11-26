#include <iostream>

struct Frac {
	int num;
	int den;

    Frac(){}
    Frac(int n, int d) {
        num = n;
        den = d;
    }

    Frac operator-(void) const;
    Frac operator+(const Frac& f) const;
    Frac operator-(const Frac& f) const;
    Frac operator*(const Frac& f) const;
    Frac operator/(const Frac& f) const;
    bool operator==(const Frac& f) const;
    bool operator!=(const Frac& f) const;

    friend std::ostream& operator<<(std::ostream& os, const Frac& f);
    friend std::istream& operator>>(std::istream& is, Frac& f);
};

int gcd (int a, int b) {
    return b == 0 ? a : gcd (b, a % b);
}

Frac simplify (Frac f) {
    Frac res;
    int GCD = gcd (f.num,f.den);
    res.num = f.num/GCD;
    res.den = f.den/GCD;
    return res;
}

Frac Frac::operator-(void) const {
    return Frac(-num, den);
}

Frac Frac::operator+(const Frac &v) const {

    return Frac();
}

Frac Frac::operator-(const Frac &v) const {
    return Frac();
}

Frac Frac::operator*(const Frac &f) const {
    return Frac();
}

Frac Frac::operator/(const Frac &f) const {
    return Frac();
}

bool Frac::operator==(const Frac &f) const {
    return false;
}

bool Frac::operator!=(const Frac &f) const {
    return false;
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
    Frac f;
    std::cin >> f;
    std::cout << simplify(f);
    return 0;
}

std::ostream &operator<<(std::ostream &os, const Frac &f) {
    os << f.num << '/' << f.den;
    return os;
}

std::istream &operator>>(std::istream &is, Frac &f) {
    char dumdum;
    is >> f.num >> dumdum >> f.den;
    return is;
}


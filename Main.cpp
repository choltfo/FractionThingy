#include <iostream>
#include <vector>

int gcd (int a, int b) {
	return b == 0 ? a : gcd (b, a % b);
}

struct Frac {
	int num;
	int den;

    Frac(){}
    Frac(int n, int d) {
        num = n;
        den = d;
    }

	Frac simplify();
	Frac inv() const;

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

Frac Frac::simplify() {
    int GCD = gcd (num, den);
    num = num/GCD;
    den = den/GCD;
    return *this;
}

Frac Frac::inv() const {
	return Frac(den, num).simplify();
}

Frac Frac::operator-(void) const {
    return Frac(-num, den).simplify();
}

Frac Frac::operator+(const Frac &f) const {
    return Frac((num * f.den) + (f.num * den), den * f.den).simplify();
}

Frac Frac::operator-(const Frac &f) const {
    return (*this + -f);
}

Frac Frac::operator*(const Frac &f) const {
    return Frac(num * f.num, den * f.den).simplify();
}

Frac Frac::operator/(const Frac &f) const {
    return (*this * f.inv()).simplify();
}

bool Frac::operator==(const Frac &f) const {
    return (num == f.num && den == f.den);
}

bool Frac::operator!=(const Frac &f) const {
    return !(*this == f);
}

enum Oper {
	ADD = '+',
	SUB = '-',
	MUL = '*',
	DIV = '/'
};

struct Expr {
	Frac a;
	Frac b;
	Oper o;

    Expr(){}
    Expr(Frac n, Frac d, Oper e) {
        a = n;
        b = d;
        o = e;
    }
};

//--------------------------- CLASS THINGS

int isCharIntOrMinus (char c) {
	return c == '-' || (c >= '0' && c <= '9');
}
int isCharInt (char c) {
	return (c >= '0' && c <= '9');
}
int isCharOper (char c) {
	return (c == '-' || c == '*' || c == '/' || c == '+');
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
	f->num = atoi(&s[1]);
	int den = 0;

	if (!isCharIntOrMinus(s[i])) return 0;
	den = i;
	printf("%c\n",s[i]);
	if (s[1] == '-' && !isCharInt(s[i+2])) return 0;
	++i;
	while (isCharInt(s[i])) ++i;
	if (s[i] == ')') return 0;
	s[i] = '\0';
	f->num = atoi(s+den);
	return i+1;
}

int parseExp (char * s, Expr * e) {
	int pos = 0;
	if (!(pos += parseFrac(s+pos, &e->a))) return 0;
	while (!isCharOper(s[pos])) {
		if (s[pos] != ' ') return 0;
		++pos;
	}
	e->o = (Oper)s[pos];
	while (!isCharOper(s[pos])) {
		if (s[pos] != ' ') return 0;
		++pos;
	}
	if (!(pos += parseFrac(s+pos, &e->b))) return 0;
	return 1;
}

std::istream &operator>>(std::istream &in, char c) {
	in >> std::ws;
	if (in.peek() == c)
		in.ignore();
	else
		in.clear(std::ios::failbit);
	return in;
}

std::ostream &operator<<(std::ostream &os, const Frac &f) {
	os << f.num;
	if (f.den != 1) os  << '/' << f.den;
	return os;
}

std::istream &operator>>(std::istream &is, Frac &f) {
	int n = 0, d = 0;
	is >> n >> '/' >> d;
	if (!is.good()) return is;
	f.num = n;
	f.den = d;
	return is;
}

int main () {
    Frac f;
    char inputA [80] = "(-1200/120)";
    char inputB [80] = "(-1200/120) * (10/435)";
    parseFrac (inputA, &f);
	std::cin >> f;
	if (std::cin.good()) std::cout << f;

    return 0;
}

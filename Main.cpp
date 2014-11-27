#include <iostream>
#include <vector>
#include <sstream>

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
	if (den < 0) {
		num *= -1;
		den *= -1;
	}
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

Expr parseExpr(std::istream &s) {
	Expr res;
	s >> res.a;
	//s >> res.o;
	s >> res.b;
	return res;
}

std::istream &operator>>(std::istream &is, char c) {
	is >> std::ws;
	if (is.peek() == c)
		is.ignore();
	else
		is.clear(std::ios::failbit);
	return is;
}

std::ostream &operator<<(std::ostream &os, const Frac &f) {
	os << '(' << f.num;
	if (f.den != 1) os  << '/' << f.den;
	os << ')';
	return os;
}

std::istream &operator>>(std::istream &is, Frac &f) {
	int n = 0, d = 0;
	is >> '(' >> n >> '/' >> d >> ')';
	if (!is.good()) return is;
	f.num = n;
	f.den = d;
	return is;
}

int main () {
    Frac f;
	std::string inputA = "(-1200/120)";
	std::stringstream ss;
	ss << inputA;
	ss >> f;
	std::cout << f.simplify();
    return 0;
}

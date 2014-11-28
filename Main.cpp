#include <iostream>
#include <vector>
#include <sstream>

int gcd (int a, int b) {
	return b == 0 ? a : gcd (b, a % b);
}

std::istream &operator>>(std::istream &is, char c) {
	is >> std::ws;
	if (is.peek() == c) {
		is.ignore();
		is >> c;
	}
	else
		is.clear(std::ios::failbit);

	return is;
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

bool isCharOper (char c) {
	return (c == '-' || c == '*' || c == '/' || c == '+');
}

struct Expr {
	Frac a;
	Frac b;
	char o;

    Expr(){}
    Expr(Frac n, Frac d, char e) {
        a = n;
        b = d;
        o = e;
    }

	Frac eval();

	friend std::ostream& operator<<(std::ostream& os, const Expr& e);
	friend std::istream& operator>>(std::istream& is, Expr& e);
};

Frac Expr::eval() {
	switch (o) {
		case '+':
			return a + b;
		case '-':
			return a - b;
		case '*':
			return a * b;
		case '/':
			return a / b;
		default:
			return Frac();
	}
}

std::ostream &operator<<(std::ostream &os, const Expr &e) {
	os << e.a << e.o << e.b;
	return os;
}

std::istream &operator>>(std::istream &is, Expr &e) {
	is >> e.a >> std::ws;
	if (isCharOper(is.peek()))
		is >> e.o;
	else
		is.clear(std::ios::failbit);
	is >> std::ws >> e.b;
	return is;
}

int menu () {
	std::cout << "Enter the number of the option you would like:\n";
	std::cout << "1 - Input and evaluate\n";
	std::cout << "2 - Output operations\n";
	std::cout << "3 - Quit\n";
	int res = 0;
	std::cin >> res;
	return res;
}

std::vector<Expr> inputHistory(0);

void outputAll () {
	for (int i = 0; i < inputHistory.size(); i++) {
		std::cout << inputHistory[i] << '=' << inputHistory[i].eval() << '\n';
	}
}

int main () {
	//v.reserve(30);
	int selection = 0;
	while (selection != 3) {
		Expr e = Expr(Frac(10,10), Frac(10,10), '*');
		
		char inputA[80] = "(-1200/120)*(10/5)";
		
		switch (selection) {
			case 1:
				std::cin >> e;
				
				if (!std::cin.good()) {
					std::cout << "Invalid input!";
					std::cin.clear();
					std::cin.ignore('\n',10000);
				}
				else {
					inputHistory.push_back(e);
				}
				
				break;
			case 2:
				outputAll();
				break;
		}
		selection = menu();
		std::cout << selection << '\n';
	}
    return 0;
}

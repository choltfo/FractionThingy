#include <iostream>
#include <vector>
#include <sstream>
#include <stdlib.h>     /* srand, rand */

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

    Frac(){
		num = 0;
		den = 0;
	}
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
	bool operator>(const Frac& f) const;
	bool operator<(const Frac& f) const;

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

bool Frac::operator>(const Frac &f) const {
	return (float)num / (float)den > (float)f.num / (float)f.den;
}

bool Frac::operator<(const Frac &f) const {
	return !(*this > f) && !(*this == f);
}

std::ostream &operator<<(std::ostream &os, const Frac &f) {
	os << '(' << f.num;
	if (f.den != 1) os  << '/' << f.den;
	os << ')';
	return os;
}

std::istream &operator>>(std::istream &is, Frac &f) {
	int n = 0, d = 0;
	char a;
	is >> a >> n >> a >> d >> a;
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
	Frac a(0,0), b(0,0);
	char o;
	is >> a >> std::ws >> o >> std::ws >> b;
	if (!is.good() || !isCharOper(o)) return is;
	e.a = a;
	e.b = b;
	e.o = o;
	return is;
}

int menu () {
	std::cout << "Enter the number of the option you would like:\n";
	std::cout << "1 - Input and evaluate\n";
	std::cout << "2 - Output operations\n";
	std::cout << "3 - Quit\n";
	int res = 0;
	std::cin >> res;
	if (std::cin.good()) return res;
	else {
		std::cout << "Invalid selection!\n";
		std::cin.clear();
		std::cin.ignore(10000,'\n');
		return 0;
	}
}

std::vector<Expr> inputHistory(0);

inline bool exprComp(Expr A, Expr B){
	return A.eval() < B.eval();
}

void outputSortedAnswer () {
	std::sort(inputHistory.begin(), inputHistory.end(), exprComp);
	for (int i = 0; i < inputHistory.size(); ++i) {
		std::cout << i << ": " << inputHistory[i] << " = " << inputHistory[i].eval() << '\n';
	}
}

inline bool exprOpSort (Expr A, Expr B) {
	return A.o > B.o;
}

void outputSortedOperator () {
	std::sort(inputHistory.begin(), inputHistory.end(), exprOpSort);
	for (int i = 0; i < inputHistory.size(); ++i) {
		if (i > 0) {
			if (inputHistory[i].o != inputHistory[i-1].o) {
				std::cout << (inputHistory[i].o == '*' ? "MULTIPLY" : inputHistory[i].o == '/' ? "DIVIDE" : inputHistory[i].o == '+' ? "ADD" : "SUBTRACT")
				<< '\n';
			}
		}
		std::cout << i << ": " << inputHistory[i] << " = " << inputHistory[i].eval() << '\n';
	}
}

Expr randomExpr () {
	int op = rand() % 4;
	
	Expr e(
		Frac(rand() % 100, rand() % 100 * (rand() %2 == 0 ? 1 : -1)),
		Frac(rand() % 100, rand() % 100 * (rand() %2 == 0 ? 1 : -1)),
		op >= 2 ? (op == 2 ? '*' : '/') : (op == 0 ? '*' : '/')
	);
	return e;
}

int main () {
	//v.reserve(30);
	int selection = 0;
	while (selection != 3) {
		Expr e = Expr(Frac(10,10), Frac(10,10), '*');
		
		char inputA[80] = "(-1200/120)*(10/5)";
		
		switch (selection) {
			/*
				1 Add random expression
				2 Get expression from user
				3 Sort by answer (low to high)
				4 Output expression with answers
				5 Delete expression by index
				6 Sort by operation
			*/
			case 1:
				std::cout << "Generating random expression...";
				inputHistory.push_back(randomExpr());
				break;
			case 2:
				std::cin >> e;
				
				if (!std::cin.good()) {
					std::cout << "Invalid input!\n";
					std::cin.clear();
					std::cin.ignore(10000, '\n');
				} else {
					inputHistory.push_back(e);
				}
				break;
			case 3:
				std::cout << "Outputting sorted expressions\n"
				outputSortedAnswer();
				break;
			
			/*
			case 2:
				outputAll();
				break;*/
		}
		selection = menu();
		std::cout << selection << '\n';
	}
    return 0;
}


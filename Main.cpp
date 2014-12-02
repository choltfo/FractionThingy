#include <iostream>
#include <vector>
#include <sstream>

int gcd (int a, int b) {
	return b == 0 ? a : gcd (b, a % b);
}

template<char n>
std::istream &sk(std::istream& is) {
	if ((is >> std::ws).peek() != n)
		is.setstate(std::ios_base::failbit);
	else
		return is.ignore();
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
	is >> sk<'('> >> n >> sk<'/'> >> d >> sk<')'>;
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

std::vector<Expr> inputHistory(0);

void outputAll () {
	for (int i = 0; i < inputHistory.size(); ++i) {
		std::cout << i << ": " << inputHistory[i] << " = " << inputHistory[i].eval() << '\n';
	}
}

void outputSortedAnswer () {
	std::sort(inputHistory.begin(), inputHistory.end(), [](Expr A, Expr B){return A.eval() < B.eval();});
	outputAll();
}

inline std::string outLabel(char o) {
	return (o == '*' ? "MULTIPLY" : o == '/' ? "DIVIDE" : o == '+' ? "ADD" : "SUBTRACT");
}

void outputSortedOperator () {
	std::sort(inputHistory.begin(), inputHistory.end(), [](Expr A, Expr B){return A.o > B.o;});
	for (int i = 0; i < inputHistory.size(); ++i) {
		if (i == 0)
			std::cout << outLabel(inputHistory[i].o) << '\n';
		else if (inputHistory[i].o != inputHistory[i-1].o)
			std::cout << outLabel(inputHistory[i].o) << '\n';

		std::cout << i << ": " << inputHistory[i] << " = " << inputHistory[i].eval() << '\n';
	}
}

Expr randomExpr () {
	int op = rand() % 4;
	
	Expr e(
		Frac(rand() % 99 + 1, rand() % 99 + 1 * (rand() %2 == 0 ? 1 : -1)),
		Frac(rand() % 99 + 1, rand() % 99 + 1 * (rand() %2 == 0 ? 1 : -1)),
		op >= 2 ? (op == 2 ? '*' : '/') : (op == 0 ? '+' : '-')
	);
	return e;
}

int menu () {
	std::cout << "Enter the number of the option you would like:\n";
	std::cout << "1 - Add random expression\n";
	std::cout << "2 - Get expression from user\n";
	std::cout << "3 - Sort by answer (low to high)\n";
	std::cout << "4 - Sort by operation\n";
	std::cout << "5 - Output expression with answer\n";
	std::cout << "6 - Delete expression by index\n";
	std::cout << "7 - Quit\n";
	int res = 0;
	std::cin >> res;
	if (std::cin.good()) return res;
	std::cout << "Invalid selection!\n";
	std::cin.clear();
	std::cin.ignore(10000,'\n');
	return 0;
}

int main () {
	srand(time(NULL));
	int selection = 0;
	int index = 0;
	while (1) {
		Expr e;

		switch (selection) {
			/*
				1 Add random expression
				2 Get expression from user
				3 Sort by answer (low to high)
				4 Sort by operation
				5 Output expression with answers
				6 Delete expression by index
				7 Quit
			*/
			case 1:
				std::cout << "Generating random expression...\n";
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
				std::cout << "Outputting sorted expressions...\n";
				outputSortedAnswer();
				break;
			case 4:
				std::cout << "Outputting expressions sorted by operator...\n";
				outputSortedOperator();
				break;
			case 5:
				std::cout << "Outputting answers...\n";
				outputAll();
				break;
			case 6:
				std::cout << "Enter index of expression to remove:\n";
				
				std::cin >> index;
				if (!std::cin.good()) {
					std::cout << "Invalid input!\n";
					std::cin.clear();
					std::cin.ignore(10000, '\n');
				} else {
					std::cout << "Removing element " << index << ".\n";
					if (index < 0 || index >= inputHistory.size()) std::cout << "Invalid input!";
					else inputHistory.erase(inputHistory.begin() + index);
				}
				break;
			case 7:
				outputAll();
				return 0;
			default:
				break;
		}
		selection = menu();
	}
    return 0;
}


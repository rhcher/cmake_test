#include "Screen.hpp"
#include <cstddef>
using namespace std;

int add(int a, int b)
{
	int s;
	StrVec c;
	while (c)
	{
		cout << !c << endl;
	}
	return a + b;
}

auto mod = [](int a, int b) { return a % b; };

struct divide
{
	int operator()(int a, int b)
	{
		return a / b;
	}
};

int main(int argc, char* argv[])
{
	// map<string, function<int(int,int)>> funtest;
	// funtest.insert({"+",add});
	// funtest.insert({"%",mod});
	// funtest.insert({"/",divide()});
	// cout << funtest["/"](12,1) << endl;

	StrVec lhs{"lhs", "test", "string"};
	StrVec rhs{"rhs", "test", "string"};
	lhs = {"initializer_list", "test", "string"};
	sort(lhs.begin(), lhs.end(), greater<string>());
	cout << lhs << endl;;
	lhs += rhs;
	cout << lhs;
	cout << endl;
	return 0;
}

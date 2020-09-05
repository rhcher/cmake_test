#include "Screen.h"
using namespace std;
struct Test
{
	Test() = default;
	Test(const Test& t) = default;
	Test(Test&& t)noexcept = default;
	Test& operator=(const Test& t) = default;
	Test& operator=(Test&& t) noexcept = default;
	~Test() = default;

private:
	allocator<string> alloc;
	string* str = nullptr;
	void free();
};

int add(int a, int b)
{
	StrVec c;
	while (cout)
	{
		cout << !c << endl;
	}
	return a + b;
}

auto mod = [](int a, int b) { return a % b; };

struct divide
{
	auto operator()(int a, int b) -> int
	{
		return a / b;
	}
};

int main()
{
	// vector<int> a {1,2,3,4,5};
	// auto&& b = std::move(a);
	// map<string, function<int(int, int)>> funtest;
	// funtest.insert({"+", add});
	// funtest.insert({"%", mod});
	// funtest.insert({"/", divide()});
	// cout << funtest["/"](12, 1) << endl;
	int* p = new int(1);
	auto* q = p;
	delete q;

	StrVec lhs{"lhs", "test", "string"};
	StrVec rhs{"rhs", "test", "string"};
	while(true)
	{
		StrVec test;
	}
	// lhs = {"initializer_list", "test", "string"};
	// sort(lhs.begin(), lhs.end(), greater<>());
	// cout << lhs << endl;
	// lhs += rhs;
	// cout << lhs;
	// cout << endl;

	return 0;
}

#include "Screen.hpp"
using namespace std;

int main(int argc, char* argv[])
{
	StrVec test{"string", "initializer_list", "test"};
	cout << "the size is : " << test.size() << endl;
	test.push_back("pushback");
	test.reserve(10);
	for(auto item : test)
	{
		cout << item << endl;
	}
	cout << "the size now is : " << test.size() << endl;
	cout << "the capacity is : " << test.capacity() << endl;
	return 0;
}

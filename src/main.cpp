#include "Screen.hpp"
using namespace std;

int main(int argc, char* argv[])
{
	Message msg1("CSAPP");
	Message msg2("CPP Primer");
	Message msg3("SICP");
	Folder test;
	Folder cs;

	msg1.save(test);
	msg1.save(cs);
	msg2.save(test);
	msg3.save(cs);

	Message msg4(msg1);

	cout << "Folder test: " << endl;
	test.print();

	cout << "Folder cs: " << endl;
	cs.print();

	return 0;
}

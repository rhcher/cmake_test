#include "Screen.hpp"
using namespace std;

int main(int argc, char* argv[])
{
	ifstream in("data/text");
	runQueries(in);
	return 0;
}


// TODO update uml

#include <exception>
#include <iostream>
#include <iterator>
#include "SparseVector.h"

using namespace std;

int main(int argc, char* argv[])
{
	SparseVector<double> sv1(10), sv2(10), sv3(10);

	try
	{
		sv1[0] = 1;
		sv1[2] = 1.9;
		sv1[3] = 0.8;
		sv2[0] = 12.3;
		sv2[4] = 0.1;
		sv3[2] = 11;
		double a = 1, b = 3;
		auto res = sv2 * (a + b);
		cout << res << endl;
	}
	catch(std::exception& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}
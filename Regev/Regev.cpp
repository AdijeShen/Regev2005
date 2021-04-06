// Regev.cpp: 定义应用程序的入口点。
//

#include "Regev.h"
using namespace std;

int main()
{
	Regev r(5, 257);
	r.KeyGen();
	Matrix C1 = r.encrypt(1);
	Matrix C2 = C1 + C1;
	Matrix C3 = C2 + C1;
	int m2 = r.decrypt(C2);

	int m3 = r.decrypt(C3);
	print_line(__FILE__, __LINE__);
	cout << m2 << endl;

	print_line(__FILE__, __LINE__);
	cout << m3 << endl;
}
// Regev.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <iostream>
#include "util/matrix.h"
#include <random>

void print_line(string file, int line_num) {
	cout << "in File " << file << " ";
	cout << "line " << line_num + 1 << ":-->\n";
}

class Regev {
private:
	int n;
	int q;
	int N;
	default_random_engine dre;
	uniform_int_distribution<int> uid;
	uniform_int_distribution<int> eid;
	Matrix P;
	Matrix s;
public:
	Regev(int n, int q) :n(n), q(q), N(int((n + 1)* log2(q) +1)), P(N, n + 1, 0), s(n, 1, 0), uid(1, q - 1), eid(0, 1) {};
	void KeyGen();
	Matrix getSecretKey() { return s; };
	Matrix getPublicKey() { return P; };
	Matrix encrypt(int m);
	int decrypt(Matrix c);
};

void Regev::KeyGen() {

	Matrix A(N, n, 0);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < n; j++) {
			A[i][j] = uid(dre);
		}
	}
	for (int i = 0; i < n; i++) {
		s[i][0] = uid(dre);
	}
	Matrix e(N, 1, 0);
	for (int i = 0; i < N; i++) {
		e[i][0] = eid(dre);
	}
	Matrix b = (A * s + e) ;
	for (int i = 0; i < N; i++) {
		P[i][0] = int(b[i][0]);
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < n; j++) {
			P[i][j + 1] = -A[i][j];
		}
	}
	//P = P % q;
};

Matrix Regev::encrypt(int m) {
	Matrix r(N, 1, 0);
	uniform_int_distribution<int> rid(0, 1);
	for (int i = 0; i < N; i++) {
		r[i][0] = rid(dre);
	}
	Matrix M(n + 1, 1, 0);
	M[0][0] = m;
	for (int i = 0; i < n; i++) {
		M[i + 1][0] = 0;
	}
	int half_q = q / 2;
	Matrix C = (P.T() * r + M * half_q) % q;
	return C;
};

int Regev::decrypt(Matrix c) {
	int res = c[0][0];
	for (int i = 0; i < n; i++) {
		res += c[i + 1][0] * s[i][0];
	}
	res = (res % q);
	double p = ((double)res / q) *2;
	print_line(__FILE__, __LINE__);
	cout << p << endl;
	res = (int)(p + 0.5);
	return res;
};
﻿#define _CRT_SECURE_NO_WARNINGS
/*var.6(Численно убедиться в справедливости равенства, для чего для
заданного значения х вычислить его левую часть и разложение,
стоящее в правой части. При каком n исследуемое выражение
отличается от sin x менее, чем на заданную погрешность ε?
Испытать разложение на сходимость при разных значениях х.)*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<locale.h>

#define M_PI 3.14159265358979324

void fact_(long double* fact, int i) {
	if (i <= 1)
		return;

	*fact /= i;
	fact_(fact, i - 1);
}

void Rec(const long double* x, const long double* sisus, const long double* e,
	long double* sum, long double plus) {

	static int counter = 1, here = 1;

	*sum += plus;
	if (fabsl(*sisus - *sum) > *e) {
		here++;
		counter++;
		plus = plus * (-1) * *x * *x / (2 * counter - 1) / (2 * counter - 2);
		Rec(x, sisus, e, sum, plus);
	}

	here--;
	if (!here) {
		printf("sin(x) = %.18Lf\n", *sisus);
		printf("Номер операции: %d\n", counter);
	}
}

void Iter(const long double* x, const long double* sisus, const long double* e,
	long double* sum) {

	int i, counter;
	long double plus = 1;

	counter = 1;
	*sum = plus = *x;
	while (fabsl(*sisus - *sum) > *e) {
		counter++;
		plus = plus * (-1) * *x * *x / (2 * counter - 1) / (2 * counter - 2);
		*sum += plus;
	}

	printf("sin(x) = %.18Lf\n", *sisus);
	printf("Номер операции: %d\n", counter);
}

int main()
{
	long double x, e, sisus, fact, sum_rec = 0, sum_iter = 0;
	int counter, i;
	setlocale(LC_ALL, "Russian");
	puts("Введите x (степень):");
	scanf("%Lf", &x);
	puts("Введите e > 0:");
	scanf("%Lf", &e);

	while (x >= 360) {
		x -= 360;
	}
	while (x <= -360) {
		x += 360;
	}

	x = x / 180 * M_PI;

	sisus = sinl(x);

	Rec(&x, &sisus, &e, &sum_rec, x);

	Iter(&x, &sisus, &e, &sum_iter);

	getchar();
	getchar();
	return 0;
}
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>


double mod(double a, double b);
double iterative_func(double x, double e);
double recursive_func(double x, double e, double sum, int n);
unsigned long long int factorial(int n);


int main()
{
    char try_again;
    double x, sum_iter = 0, sum_rec = 0, e;


    do
    {
        printf("\nEnter x (0.01 < x <= 2): ");
        do
        {
            while (scanf_s("%lf", &x) != 1)
            {
                scanf_s("%*[^\n]");
                printf("Wrong input. Try again: ");
            }

            if ((x > 0.01) && (x <= 1000))
                break;
            else printf("Invalid value of x. Try again: ");
        } while (1);
        printf("Enter e (0 < e <= 0.01) - accuracy: ");
        do
        {
            while (scanf_s("%lf", &e) != 1)
            {
                scanf_s("%*[^\n]");
                printf("Wrong input. Try again: ");
            }

            if ((e <= 0.01) && (e > 0))
                break;
            else printf("Invalid value of e. Try again: ");
        } while (1);
        sum_iter = iterative_func(x, e);
        sum_rec = recursive_func(x, e, 0, 1);
        printf("sin(x) = %.6lf", sin(x));
        printf("\nDecomposition using iterative function: %lf", sum_iter);
        printf("\nDecomposition using recursive function: %lf", sum_rec);
        printf("\nEnter 1 to enter another data: ");
        try_again = _getche();
        printf("\n");
    } while (try_again == '1');
}


double mod(double a, double b)
{
    if (a > b)
        return (a - b);
    else
        return (b - a);
}


double iterative_func(double x, double e)
{
    int n = 0, sign;
    double sum = 0, fact, power;
    power = x;
    fact = 1.0;
    do
    {
        n++;
        if (n == 1)
        {
            power = x;
            sign = 1;
        }
        else
        {
            power *= x * x;
            sign *= (-1);
        }
        fact *= (double)(2 * n - 1);
        sum += (double)(sign * power / fact);
        fact *= 2 * (double)n;
    } while (mod(sum, sin(x)) > e);
    printf("For a given \"e\" number n = %d\n", n);
    return sum;
}


double recursive_func(double x, double e, double sum, int n)
{
    if (mod(sum, sin(x)) < e)
        return sum;
    sum += pow(x, 2 * n - 1) / (double)factorial(2 * n - 1) * pow(-1, n - 1);
    return recursive_func(x, e, sum, n + 1);
}

unsigned long long int factorial(int n)
{
    if (n == 1)
        return 1;
    else
        return n * factorial(n - 1);
}

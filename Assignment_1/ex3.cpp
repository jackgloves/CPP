#include <iostream>
#include <cmath>
using namespace std;

// Define the standard
const double PI = 3.141592653589793238463;

// Approach a: Leibniz formula
double Pi1(int n) {
    if (n == 0) {
        return 4;
    } else {
        double sign = (n % 2 == 0) ? 1 : -1;
        return sign * (4.0 / (2*n + 1)) + Pi1(n-1);
    }
}

// Approach b: Nilakantha series
double Pi2(int nTerm) {
    int n = 1;
    double pi = 3;

    while (true) {
        double term = 4.0 / (2*n * (2*n+1) * (2*n+2));
        if (n % 2 == 0) {
            pi -= term;
        } else {
            pi += term;
        }
        if (n > nTerm-1) {
            break;
        }
        n++;
    }

    return pi;
}

// Approach c: Machin-like formula
double arctan(double x, int nTerm) {
    double temp = 0;
    int n = 0;

    while (true) {
        double term = (1.0 / (2.0 * n + 1.0)) * pow(x, (2.0 * n + 1.0));
        if (n % 2 == 0) {
            temp += term;
        } else {
            temp -= term;
        }
        if (n > nTerm - 1) {
            break;
        }
        n++;
    }
    return temp;
}

double Pi3(int nTerm) {
    double pi = 4.0 * (4.0 * arctan((1.0/5.0), nTerm) - arctan((1.0/239.0), nTerm));
    return pi;
}


//main function here
int main() {
    int n;
    cout << "Enter the number of terms in the series: ";
    cin >> n;

    double pi1 = Pi1(n);
    double error1 = fabs(PI - pi1);
    cout << "Approximation of pi using Leibniz formula and using " << n << " terms = " << pi1 << endl;
    cout << "The error of using Leibniz formula and using " << n << " terms = " << error1 << endl;

    double pi2 = Pi2(n);
    double error2 = fabs(PI - pi2);
    cout << "Approximation of pi using Nilakantha series and using " << n << " terms = " << pi2 << endl;
    cout << "The error of using Nilakantha series and using " << n << " terms = " << error2 << endl;

    double pi3 = Pi3(n);
    double error3 = fabs(PI - pi3);
    cout << "Approximation of pi using Machin-like formula and using " << n << " terms = " << pi3 << endl;
    cout << "The error of using Machin-like formula and using " << n << " terms = " << error3 << endl;

    return 0;
}
//

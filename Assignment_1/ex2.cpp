#include <iostream>
#include <cmath>
#include <stdexcept>

bool isPrime(int m) {
    if (m <= 1) {
        return false; // 1 and below are not prime numbers
    }

    for (int i = 2; i <= sqrt(m); i++) {
        if (m % i == 0) {
            return false; // m is divisible by i, hence it's not a prime number
        }
    }

    return true; // m is not divisible by any number from 2 to sqrt(m), hence it's a prime number
}

void goldbach(int m) {
    if (m <= 2 || m % 2 != 0) {
        throw std::invalid_argument("Input must be a positive even integer.");
    }

    bool found = false;

    for (int i = 2; i <= m/2; i++) {
        if (isPrime(i) && isPrime(m-i)) {
            std::cout << m << " = " << i << " + " << m-i << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "Goldbach's conjecture is false for " << m << "." << std::endl;
    }
}

int main() {
    int m;

    std::cout << "Enter a positive even integer: ";
    std::cin >> m;

    try {
        goldbach(m);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid input: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

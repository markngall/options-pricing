#pragma once
#include <cmath>
#include <algorithm>

// Abstract class that stores the strike price of a vanilla option and calculates its exercise value
class OptionValueCalculator {

    // Member variable for storing the option's strike price
    double m_k;

    public:

        // Constructor – initialises the strike price member variable
        OptionValueCalculator(double k);

        // Pure virtual member function that calculates the exercise value of the option given the price of the underlying asset
        virtual double calculate(double S0, double u, int n, int j) const = 0;

        // Reads the option's strike price – used in derived classes' implementations of calculate()
        double get_strike() const;

};
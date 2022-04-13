#pragma once
#include "OptionValueCalculator.hpp"

// Derived class that pubicly inherits from OptionValueCalculator and specifically calculates the exercise value of vanilla put options
class PutOptionValueCalculator : public OptionValueCalculator {

    public:

        // Constructor – initialises the strike price member variable
        PutOptionValueCalculator(double k);

        // Overrides the pure virtual member function in the base class
        // Calculates the exercise value of the vanilla put option given the price of the underlying asset
        double calculate(double S0, double u, int n, int j) const override;

};
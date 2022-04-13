#include "CallOptionValueCalculator.hpp"

CallOptionValueCalculator::CallOptionValueCalculator(double k) 
:OptionValueCalculator(k) // Delegates to base constructor
{}

double CallOptionValueCalculator::calculate(double S0, double u, int n, int j) const {
    
    // Calculates asset price at nth time step after j up movements
    double ST = S0 * pow(u, 2 * j - n);

    // Reads strike price, k, from base class and calculates call option's exercise value using max{ST-k, 0}
    return std::max(ST - this->OptionValueCalculator::get_strike(), 0.0);
    
}


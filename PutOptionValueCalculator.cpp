#include "PutOptionValueCalculator.hpp"

PutOptionValueCalculator::PutOptionValueCalculator(double k)
:OptionValueCalculator(k) // Delegates to base constructor
{}

double PutOptionValueCalculator::calculate(double S0, double u, int n, int j) const {

    // Calculates asset price at nth time step after j up movements
    double ST = S0 * pow(u, 2 * j - n);
    
    // Reads strike price, k, from base class and calculates put option's exercise value using max{k-ST, 0}
    return std::max(this->OptionValueCalculator::get_strike() - ST, 0.0);

}




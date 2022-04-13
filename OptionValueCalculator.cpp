#include "OptionValueCalculator.hpp"

OptionValueCalculator::OptionValueCalculator(double k) 
:m_k(k)
{}

double OptionValueCalculator::get_strike() const {
    return m_k;
}


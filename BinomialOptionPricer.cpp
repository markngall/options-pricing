#include "BinomialOptionPricer.hpp"

BinomialOptionPricer::BinomialOptionPricer(unsigned int max_depth, double growth_factor, double growth_probability) 
:
m_max_depth(max_depth),
m_u(growth_factor),
m_p(growth_probability)
{
    // Having max_depth+1 doubles allows us to store values for any allowable size of binomial tree
    m_values = new double[max_depth+1]();
}

BinomialOptionPricer::~BinomialOptionPricer() {
    delete[] m_values;
}

BinomialOptionPricer::BinomialOptionPricer(const BinomialOptionPricer& pricer) {

    // Copy member variables into new object
    m_max_depth = pricer.m_max_depth;
    m_u = pricer.m_u;
    m_p = pricer.m_p;

    // Allocate memory on the heap for the new object
    m_values = new double[m_max_depth+1]();

    // Copy the values into new object
    for (unsigned int i = 0; i < m_max_depth+1; ++i) {
        m_values[i] = pricer.m_values[i];
    }

}

BinomialOptionPricer& BinomialOptionPricer::operator=(const BinomialOptionPricer& pricer) {
    
    // Guard self-assignment
    if (this == &pricer) {
        return *this;
    }

    // Check if memory can be re-used
    if (m_max_depth != pricer.m_max_depth) {

        // Clean up existing object
        delete[] m_values;
        m_values = nullptr; // In case memory allocation fails
        m_max_depth = 0;

        // Allocate new resources on the heap
        m_values = new double[pricer.m_max_depth+1]();
        m_max_depth = pricer.m_max_depth;

    }

    // Copy the values now that we have checked the memory is correctly allocated
    for (unsigned int i = 0; i < m_max_depth+1; ++i) {
        m_values[i] = pricer.m_values[i];
    }

    // Copy other member variables
    m_p = pricer.m_p;
    m_u = pricer.m_u;

    // Return re-assigned object (so that we can chain assingment operators)
    return *this;

}

double BinomialOptionPricer::price_vanilla_option_european_recursion(unsigned int depth, double delta_t, double S0, double r, const OptionValueCalculator& value_calc) {

    // Raise an exception if the depth exceeds the value of maximum depth stored in the object
    try {
        if (depth > m_max_depth) { 
            throw -1;
        }
    }

    // Catch the thrown integer and print an error message stating that the depth is too large
    catch (int i) {
        std::cout << "Your input for depth in the function 'price_vanilla_option_european_recursion' is too large. The maximum depth is " << m_max_depth << "." << std::endl;
        return i; // Return -1 instead of a price
    }
    
    // Call recursive function with initial values of i (current time step) and j (number of up movements)
    return BinomialOptionPricer::price_vanilla_option_european_recursion(0, 0, depth, delta_t, S0, r, value_calc);
    
}

double BinomialOptionPricer::price_vanilla_option_european_recursion(unsigned int i, unsigned int j, unsigned int depth, double delta_t, double S0, double r, const OptionValueCalculator& value_calc) {
    
    // If the current time is the expiry time, calculate the value using the OptionValueCalculator object passed by reference
    if (i == depth) {
        return value_calc.calculate(S0, m_u, i, j);
    }
    
    // Recurrence relation used to calculate the value at the current node using values from the next time step
    return exp(-r * delta_t) * (m_p * BinomialOptionPricer::price_vanilla_option_european_recursion(i+1, j+1, depth, delta_t, S0, r, value_calc) + 
    (1 - m_p) * BinomialOptionPricer::price_vanilla_option_european_recursion(i+1, j, depth, delta_t, S0, r, value_calc));

}

double BinomialOptionPricer::price_vanilla_option_american_recursion(unsigned int depth, double delta_t, double S0, double r, const OptionValueCalculator& value_calc) {

    try {
        if (depth > m_max_depth) { 
            throw -1;
        }
    }

    catch (int i) {
        std::cout << "Your input for depth in the function 'price_vanilla_option_american_recursion' is too large. The maximum depth is " << m_max_depth << "." << std::endl;
        return i;
    }
    
    return BinomialOptionPricer::price_vanilla_option_american_recursion(0, 0, depth, delta_t, S0, r, value_calc);
    
}

double BinomialOptionPricer::price_vanilla_option_american_recursion(unsigned int i, unsigned int j, unsigned int depth, double delta_t, double S0, double r, const OptionValueCalculator& value_calc) {
    
    if (i == depth) {
        return value_calc.calculate(S0, m_u, i, j);
    }
    
    // Due to the possibility of early exercise, the option's value at the current node is max{continuation value, exercise value} 
    return std::max(exp(-r * delta_t) * (m_p * BinomialOptionPricer::price_vanilla_option_american_recursion(i+1, j+1, depth, delta_t, S0, r, value_calc) + 
    (1 - m_p) * BinomialOptionPricer::price_vanilla_option_american_recursion(i+1, j, depth, delta_t, S0, r, value_calc)), value_calc.calculate(S0, m_u, i, j));

}

double BinomialOptionPricer::price_vanilla_option_european(unsigned int depth, double delta_t, double S0, double r, const OptionValueCalculator& value_calc) {

    try {
        if (depth > m_max_depth) { 
            throw -1;
        }
    }

    catch (int i) {
        std::cout << "Your input for depth in the function 'price_vanilla_option_european' is too large. The maximum depth is " << m_max_depth << "." << std::endl;
        return i;
    }

    // Iterate over time steps from the base nodes to the starting node
    for (int i = depth; i > -1; --i) {

        // Iterate over node heights from 0 to i (i is the highest value of j at the ith time step)
        for (unsigned int j = 0; j < i + 1; ++j) {

            // If the current time is the expiry time, calculate the value using the OptionValueCalculator object passed by reference
            if (i == depth) {
                m_values[j] = value_calc.calculate(S0, m_u, i, j);
            } 

            // Otherwise, compute value at current node using values from the next time step that are stored in m_values
            // The jth element of m_values is re-written as it is no longer required
            else {
                m_values[j] = exp(-r * delta_t) * (m_p*m_values[j+1] + (1-m_p)*m_values[j]);
            }

        }

    }

    // Return the value of the option at the starting node
    return m_values[0];

}

double BinomialOptionPricer::price_vanilla_option_american(unsigned int depth, double delta_t, double S0, double r, const OptionValueCalculator& value_calc) {

    try {
        if (depth > m_max_depth) { 
            throw -1;
        }
    }

    catch (int i) {
        std::cout << "Your input for depth in the function 'price_vanilla_option_american' is too large. The maximum depth is " << m_max_depth << "." << std::endl;
        return i;
    }

    for (int i = depth; i > -1; --i) {
        for (unsigned int j = 0; j < i + 1; ++j) {

            if (i == depth) {
                m_values[j] = value_calc.calculate(S0, m_u, i, j);
            } 
            
            // Due to the possibility of early exercise, the option's value at the current node is max{continuation value, exercise value} 
            else {
                m_values[j] = std::max(exp(-r * delta_t) * (m_p*m_values[j+1] + (1-m_p)*m_values[j]), value_calc.calculate(S0, m_u, i, j));
            }

        }
    }

    return m_values[0];

}
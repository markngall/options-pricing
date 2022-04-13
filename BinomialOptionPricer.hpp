#pragma once
#include "OptionValueCalculator.hpp"
#include <iostream>

// Implements option pricing algorithms
class BinomialOptionPricer {

    // Maximum depth of the binomial tree
    unsigned int m_max_depth;

    // Growth factor
    double m_u;

    // Growth probability
    double m_p;

    // Pointer to the first element of a dynamically allocated array which stores values at nodes of the binomial tree
    double* m_values;

    public:
    
        // Constructor – initialises member variables
        BinomialOptionPricer(unsigned int max_depth, double growth_factor, double growth_probability);

        // Destructor
        ~BinomialOptionPricer();

        // Copy-constructor
	    BinomialOptionPricer(const BinomialOptionPricer& pricer);

	    // Copy-assignment operator
	    BinomialOptionPricer& operator=(const BinomialOptionPricer& pricer);

        // Price a European option using the forward-recursion method
        double price_vanilla_option_european_recursion(unsigned int depth, double delta_t, double S0, double r, const OptionValueCalculator& value_calc);

        // Recursive function for European options – keeps track of i and j (have used function overloading)
        double price_vanilla_option_european_recursion(unsigned int i, unsigned int j, unsigned int depth, double delta_t, double S0, double r, const OptionValueCalculator& value_calc);

        // Price an American option using the forward-recursion method
        double price_vanilla_option_american_recursion(unsigned int depth, double delta_t, double S0, double r, const OptionValueCalculator& value_calc);

        // Recursive function for American options – keeps track of i and j (have used function overloading)
        double price_vanilla_option_american_recursion(unsigned int i, unsigned int j, unsigned int depth, double delta_t, double S0, double r, const OptionValueCalculator& value_calc);

        // Price a European option using the backward-induction method
        double price_vanilla_option_european(unsigned int depth, double delta_t, double S0, double r, const OptionValueCalculator& value_calc);

        // Price an American option using the backward-induction method
        double price_vanilla_option_american(unsigned int depth, double delta_t, double S0, double r, const OptionValueCalculator& value_calc);

};
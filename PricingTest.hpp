#include <cmath>
#include "BinomialOptionPricer.hpp"
#include "CallOptionValueCalculator.hpp"
#include "PutOptionValueCalculator.hpp"
#include <iostream>
#include <array>

class PricingTest {

    public:

        double normal_cdf(double x);
    
        double BS_call(double S0, double r, double vol, double q, double k, double T);

        double BS_put(double S0, double r, double vol, double q, double k, double T);

        // Function to test accuracy of pricing methods for European options
        void european_test();

        // Function to test accuracy of pricing methods for American options
        void american_test();

        void copy_test();

};
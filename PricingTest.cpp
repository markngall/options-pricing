#include "PricingTest.hpp"

void PricingTest::european_test() {

    // Set parameters
    double S0 = 50; // Initial asset price
    double vol = 0.21; // Volatility
    double r = 0.01; // Risk-free rate
    double q = 0.03; // Continuous dividend yield
    double T = 40.0; // Expiry time

    unsigned int depth = 20;
    unsigned int max_depth = 10000;

    double delta_t = T / depth;
    double u = exp(vol * sqrt(delta_t));
    double d = 1 / u;
    double p = (exp(delta_t * (r - q)) - d) / (u - d);
    std::cout << "u is " << u << std::endl;
    std::cout << "p is " << p << std::endl;
    
    BinomialOptionPricer pricer(max_depth, u, p);

    std::array<double, 10> strikes = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    std::array<CallOptionValueCalculator, 10> call_options = {CallOptionValueCalculator(S0), CallOptionValueCalculator(S0), CallOptionValueCalculator(S0), 
    CallOptionValueCalculator(S0), CallOptionValueCalculator(S0), CallOptionValueCalculator(S0), CallOptionValueCalculator(S0), CallOptionValueCalculator(S0), 
    CallOptionValueCalculator(S0), CallOptionValueCalculator(S0)};
    std::array<PutOptionValueCalculator, 10> put_options = {PutOptionValueCalculator(S0), PutOptionValueCalculator(S0), PutOptionValueCalculator(S0), 
    PutOptionValueCalculator(S0), PutOptionValueCalculator(S0), PutOptionValueCalculator(S0), PutOptionValueCalculator(S0), PutOptionValueCalculator(S0), 
    PutOptionValueCalculator(S0), PutOptionValueCalculator(S0)};

    for (unsigned int i = 0; i < 10; ++i) {
        call_options[i] = CallOptionValueCalculator(strikes[i]);
    }

    for (unsigned int i = 0; i < 10; ++i) {
        put_options[i] = PutOptionValueCalculator(strikes[i]);
    }
    
    std::cout << "European Call Options:" << std::endl;
    for (unsigned int i = 0; i < 10; ++i) {
        std::cout << "K = " << strikes[i] << ". Backward Induction: " << pricer.price_vanilla_option_european(depth, delta_t, S0, r, call_options[i]) 
        << ". Forward-Recursion: " << pricer.price_vanilla_option_european_recursion(depth, delta_t, S0, r, call_options[i]) 
        << ". Analytical: " << BS_call(S0, r, vol, q, strikes[i], T) << std::endl;
    }

    std::cout << "European Put Options:" << std::endl;
    for (unsigned int i = 0; i < 10; ++i) {
        std::cout << "Strike: " << strikes[i] << ". Backward Induction: " << pricer.price_vanilla_option_european(depth, delta_t, S0, r, put_options[i]) 
        << ". Forward-Recursion: " << pricer.price_vanilla_option_european_recursion(depth, delta_t, S0, r, put_options[i])
        << ". Analytical: " << BS_put(S0, r, vol, q, strikes[i], T) << std::endl;
    }
    
}

void PricingTest::american_test() {

    // Set parameters
    double S0 = 100; // Initial asset price
    double vol = 0.1; // Volatility
    double r = 0.05; // Risk-free rate
    double q = 0.07; // Continuous dividend yield
    double T = 40.0; // Expiry time

    unsigned int depth = 20;
    unsigned int max_depth = 1000;

    double delta_t = T / depth;
    double u = exp(vol * sqrt(delta_t));
    double d = 1 / u;
    double p = (exp(delta_t * (r - q)) - d) / (u - d);
    
    BinomialOptionPricer pricer(max_depth, u, p);

    std::array<double, 10> strikes = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    std::array<CallOptionValueCalculator, 10> call_options = {CallOptionValueCalculator(S0), CallOptionValueCalculator(S0), CallOptionValueCalculator(S0), 
    CallOptionValueCalculator(S0), CallOptionValueCalculator(S0), CallOptionValueCalculator(S0), CallOptionValueCalculator(S0), CallOptionValueCalculator(S0), 
    CallOptionValueCalculator(S0), CallOptionValueCalculator(S0)};
    std::array<PutOptionValueCalculator, 10> put_options = {PutOptionValueCalculator(S0), PutOptionValueCalculator(S0), PutOptionValueCalculator(S0), 
    PutOptionValueCalculator(S0), PutOptionValueCalculator(S0), PutOptionValueCalculator(S0), PutOptionValueCalculator(S0), PutOptionValueCalculator(S0), 
    PutOptionValueCalculator(S0), PutOptionValueCalculator(S0)};

    for (unsigned int i = 0; i < 10; ++i) {
        call_options[i] = CallOptionValueCalculator(strikes[i]);
    }

    for (unsigned int i = 0; i < 10; ++i) {
        put_options[i] = PutOptionValueCalculator(strikes[i]);
    }

    std::cout << "American Call Options:" << std::endl;
    for (unsigned int i = 0; i < 10; ++i) {
        std::cout << "K = " << strikes[i] << ". American Backward Induction: " << pricer.price_vanilla_option_american(depth, delta_t, S0, r, call_options[i])
        << ". European Backward Induction: " << pricer.price_vanilla_option_european(depth, delta_t, S0, r, call_options[i]) 
        << ". European Analytical: " << BS_call(S0, r, vol, q, strikes[i], T) << std::endl;
    }

    std::cout << "American Put Options:" << std::endl;
    for (unsigned int i = 0; i < 10; ++i) {
        std::cout << "Strike: " << strikes[i] << ". American Backward Induction: " << pricer.price_vanilla_option_american(depth, delta_t, S0, r, put_options[i]) 
        << ". European Backward Induction: " << pricer.price_vanilla_option_european(depth, delta_t, S0, r, put_options[i]) 
        << ". European Analytical: " << BS_put(S0, r, vol, q, strikes[i], T) << std::endl;
    }

}

double PricingTest::normal_cdf(double x) {
    return erfc(-x / sqrt(2)) / 2;
}

double PricingTest::BS_call(double S0, double r, double vol, double q, double k, double T) {
    double d1 = (log(S0/k) + (r - q + pow(vol, 2)/2) * T) / (vol * sqrt(T));
    double d2 = d1 - vol * sqrt(T);
    return S0 * exp(-q * T) * normal_cdf(d1) - k * exp(-r * T) * normal_cdf(d2);
}

double PricingTest::BS_put(double S0, double r, double vol, double q, double k, double T) {
    double d1 = (log(S0/k) + (r - q + pow(vol, 2)/2) * T) / (vol * sqrt(T));
    double d2 = d1 - vol * sqrt(T);
    return k * exp(-r * T) * normal_cdf(-d2) - S0 * exp(-q * T) * normal_cdf(-d1);
}

void PricingTest::copy_test() {

    // Set parameters
    double S0 = 50; // Initial asset price
    double vol = 0.21; // Volatility
    double r = 0.06; // Risk-free rate
    double q = 0.03; // Continuous dividend yield
    double T = 10; // Expiry time
    unsigned int depth = 20;
    double delta_t = T / depth;

    CallOptionValueCalculator value_calc(50);

    for (unsigned int i = 0; i < 1; ++i) {

        BinomialOptionPricer pricer1(100, 1.1, 0.5);
        BinomialOptionPricer pricer2(50, 1.2, 0.8);

        // Test self-assingment
        std::cout << &pricer2 << std::endl;
        pricer2 = pricer2;
        std::cout << &pricer2 << std::endl;

        // Test copy-constructor
        BinomialOptionPricer pricer3 = pricer1;
        std::cout << pricer1.price_vanilla_option_european(depth, delta_t, S0, r, value_calc) << std::endl;
        std::cout << pricer3.price_vanilla_option_european(depth, delta_t, S0, r, value_calc) << std::endl;

        // Test copy-assignment
        std::cout << pricer2.price_vanilla_option_european(depth, delta_t, S0, r, value_calc) << std::endl;
        pricer2 = pricer1;
        std::cout << pricer2.price_vanilla_option_european(depth, delta_t, S0, r, value_calc) << std::endl;

    }

}
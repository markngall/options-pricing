#include "PricingTest.hpp"

int main() {
    PricingTest test;
    test.european_test();
    test.american_test();
    test.copy_test();
    return 0;
}
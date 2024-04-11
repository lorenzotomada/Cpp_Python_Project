#ifndef EXAMPLE__FUNCTIONS__
#define EXAMPLE__FUNCTIONS__



#include <vector>
#include <functional>
#include <cmath>
#include <complex>
#include <cstdlib>


/* In this header we define some functions which will be used in the testing of the polynomial order or of the
convergence order. They are all templatized in order to allow us to use them as tests for both real and complex
integration. */


template <typename field>
field c_1(double x) {
  x = 1; /* This is here just to avoid an unused parameter warning. This is the reason for us not using the
  keyword 'const' before x. */
  return x;
}

template <typename field>
std::function<field(double)> const_1 = c_1<field>;



template <typename field>
field linear_function(const double x) {return x;}

template <typename field>
std::function<field(const double)> x_function = linear_function<field>;



template <typename field>
field ssquare(const double x) {return std::pow(x, 2);}

template <typename field>
std::function<field(const double)> square = ssquare<field>;



template <typename field>
field qube(const double x) {return std::pow(x, 3);}

template <typename field>
std::function<field(const double)> cube = qube<field>;



template <typename field>
field forthhh(const double x) {return std::pow(x, 4);}

template <typename field>
std::function<field(const double)> x_4 = forthhh<field>;



template <typename field>
field sinnus(const double x) {return std::sin(x);}

template <typename field>
std::function<field(const double)> sine = sinnus<field>;



template <typename field>
field exp_times_sinnus(const double x) {return std::sin(x)*std::exp(x);}

template <typename field>
std::function<field(const double)> exp_times_sine = exp_times_sinnus<field>;



template <typename field>
field cheb_ssquare(const double x) {
  return 3*x*x*std::sqrt((x-0)*(1-x));
}

template <typename field>
std::function<field(const double)> cheb_square = cheb_ssquare<field>; /* This function is useful because when
integrated against the weight determined by Chebyshev polynomials in [0, 1] it returns 1 */



#endif // EXAMPLE__FUNCTIONS__
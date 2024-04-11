#ifndef Integration_Hpp
#define Integration_Hpp

#include <iostream>
#include <vector>
#include <functional>
#include <numeric>
#include <cmath>
#include <complex>
#include "Functions.hpp"
#include <memory> // For shared pointers
#include <sstream>
#include <string>

#include <gsl/gsl_integration.h>



/* Now we define an abstract class called Integration. The inputs of the constructor of this class are begin
and end (corresponding to the extrema a and b of the integration interval [a, b]), subdivision n (the number
of subintervals which are used for composite integration) and the integrand function, which we passed as a
function wrapper in order to allow for greater flexibility.
While the interval is considered as fixed (hence the extrema are labelled as const, as well as its subdivision),
the integrand function not const, not forcing users to create a new class each time they compute the integral
of a new funcion.


The methods implemented (midpoint, trapezoidal, Simpson and Gaussian) are derived classes inheriting from the
abstract one. The inheritance is public in order to allow the use of polymorphism.

An important info is that, unlike in the statistics module, we decided to keep all the info public: this is
because we thought that it would be much more easy to use for possible users, since no info is actually
secret or needs to be hidden anyway.


The abstract class only has one method, which is virtual: it is compute_integral().
It was not labelled as const for compatibiliy with the library used in the Gaussian integration (GNU GSL).
More details regarding this aspect are provided in the Gaussian class.


We have templatized everything in order to make it possible to perform integration of both real and complex
valued functions (although always with real argument).


For readability reasons, the only things that will be commented in the following are the ones that might not
be straight-forward. */


template <typename field> 
class Integration{
public:
  Integration(const double &begin, const double &end, const unsigned int &subdivision_n, std::function<field(double)> integrand) : begin(begin), end(end), subdivision_n(subdivision_n), integrand(integrand){
    partition.push_back(begin);
    h = (end-begin)/subdivision_n;
  for (unsigned int i = 1; i < subdivision_n + 1; ++i) {partition.push_back(partition[i-1]+h);}
  }

  // The constructor creates the subintervals on which we perform the integration.


  virtual field compute_integral() = 0;



  virtual ~Integration() = default; 



  const double begin;  
  const double end;
  const unsigned int subdivision_n;
  std::function<field(double)> integrand;
  std::vector<double> partition; // We save the partition of the interval
  double h; // We also save the stepsize
};



template <typename field>
class Midpoint : public Integration<field> {
public:
  Midpoint(const double &begin, const double &end, const unsigned int &subdivision_n, std::function<field(double)> integrand) :  Integration<field>(begin, end, subdivision_n, integrand) {}



  field compute_integral() override;
};



template <typename field>
class Trapezoidal : public Integration<field> {
public:
  Trapezoidal(const double &begin, const double &end, const unsigned int& subdivision_n, std::function<field(double)> integrand) :  Integration<field>(begin, end, subdivision_n, integrand) {}



  field compute_integral() override;
};



template <typename field>
class Simpson : public Integration<field> {
public:
  Simpson(const double &begin, const double &end, const unsigned int &subdivision_n, std::function<field(double)> integrand) :  Integration<field>(begin, end, subdivision_n, integrand) {}



  field compute_integral() override;
};



/* The following class implements gaussian integration using the GNU GSL.
Here some additional objects need to be given as input to the constructor.
Indeed, in addition to the previous ones, here we also need the number of nodes for the gaussian formula, a string
representing the family of polynomials chosen and the parameters alpha and beta.

Recall that this performs the integration of w(x)f(x), where w(x) is a non-negative Lebesgue-integrable function.
The default choice for the family of polynomials is Legendre (where the weight function is equal to 1) and the 
default parameters are alpha=beta=1.

See https://www.gnu.org/software/gsl/doc/html/integration.html#tab-fixed-quadratures 
for more details regarding the families of polynomials. Here we have implemented the method for the two 
Chebyshev families, Jacobi, exponential and Gegenbauer. 

All the code was written following the GNU GSL documentation. */


template <typename field>
class Gaussian : public Integration<field> {
  public:
  Gaussian(const double &begin, const double &end, const unsigned int& subdivision_n, std::function<field(double)> integrand, const unsigned int &number_of_nodes, const std::string& family_of_polynomials = "Legendre", const double &alpha = 1, const double &beta = 1) :
  Integration<field>(begin, end, subdivision_n, integrand), number_of_nodes(number_of_nodes), family_of_polynomials(family_of_polynomials), alpha(alpha), beta(beta) {} 
  

  /* The following two methods are needed because we need to pass a static member to the gsl integrator.
  So we first created a temporary function and then a static method to evaluate the input function.
  See the corresponding .tpl.hpp file for more details.
  The function passed to gsl needs to depend on double x and void* params. */

  static field static_tmp_fct(double x, void* params) {return static_cast<Gaussian*>(params)->tmp_function(x);}


  field tmp_function(double x) {return this->integrand(x);}


  field compute_integral() override;


  ~Gaussian() {}


  // We save the attributes that are specific to this class.
  
  unsigned int number_of_nodes;
  std::string family_of_polynomials;
  const double alpha;
  const double beta;
};



/* To appreciate the following functions, it is suggested to look at the specialization of the integration method
in the .tpl.hpp file.
They are just needed in order to properly split and give as input for the integration the real and the imaginary
part of a function. */

inline double real_part(std::function<std::complex<double>(double)> foonction, double x) {return std::real(foonction(x));}

inline std::function<double(double)> my_reality(std::function<std::complex<double>(double)> fun) {
  std::function<double(double)> result = [fun](double x){return real_part(fun,x);};
  return result;
}



inline double immaginary_part(std::function<std::complex<double>(double)> foonction, double x) {return std::imag(foonction(x));}

inline std::function<double(double)> imagine_dragons(std::function<std::complex<double>(double)> fun) {
  std::function<double(double)> result = [fun](double x){return immaginary_part(fun,x);};
  return result;
}



// The following function was written in order to print complex numbers in a nicer way (not as vectors).
template <typename field>
std::string nicer_complex(std::complex<field> number);


#endif

#include "../../Includes/Integration/Numerical_Integration.hpp"


/* Except for gaussian integration (for which we have used gsl), the other formulas which are implemented
follow the same scheme.
The result can be obtained as sum of the values of the functions in some specific points, weighted with the
appropriates weights.
Hence we always created a vector called "values" which contained the values (already correctly weighted)
of the functions in the correct points.
The the result is just obtained by summing over the vector and multiplying by the stepsize. */


template <typename field> 
field Midpoint<field>::compute_integral() {

  std::vector<field> values;
  for (unsigned int i = 1; i < this->partition.size(); ++i) {

    double due = 2; // STL does not allow to divide a complex number by an integer.
    double midpoint = (this->partition[i]+this->partition[i-1])/due;

    values.push_back(this->integrand(midpoint));
  }
  field zero = 0.0; // This is needed in accumulate: summing inhomogeneous data types might not be allowed.

  field result = std::accumulate(values.begin(), values.end(), zero);

  return result*this->h;
}



template <typename field>
field Trapezoidal<field>::compute_integral() {

  std::vector<field> values;
  for (unsigned int i = 1; i < this->partition.size(); ++i) {

    field a = this->integrand(this->partition[i]);
    field b = this->integrand(this->partition[i-1]);

    values.push_back((a + b)/2.0);
  }

  field zero = 0.0;
  field result = std::accumulate(values.begin(), values.end(),zero);

  return result*this->h;
}



template <typename field>
field Simpson<field>::compute_integral() {

  std::vector<field> values;
  for (unsigned int i = 1; i < this->partition.size(); ++i) {

    double midpoint = (this->partition[i]+this->partition[i-1])/2;

    field a = this->integrand(this->partition[i]);
    field b = this->integrand(this->partition[i-1]);
    field mid = this->integrand(midpoint);

    values.push_back(a/6.0+b/6.0+2.0*mid/3.0);
  }

  field zero = 0.0;
  field result = std::accumulate(values.begin(), values.end(), zero);

  return result*this->h;
}



/* For Gaussian integration, we just followed the gsl documentation.
We inserted some if-checks to be sure that the family of polynomials is correct and to consequently use the
correct weight function and parameters. */


template <typename field>
field Gaussian<field>::compute_integral() {

  field result;
  gsl_function to_be_integrated;
  to_be_integrated.function = static_tmp_fct;
  to_be_integrated.params = static_cast<void*>(this);

  if (this->family_of_polynomials == "Legendre") {
    gsl_integration_fixed_workspace  *w;
    const gsl_integration_fixed_type * T = gsl_integration_fixed_legendre;
    std::vector<field> subinterval_integration;

    // Just to avoid a warning
    w = gsl_integration_fixed_alloc(T, this->number_of_nodes, this->begin, this->end, 0.0, 0.0);

    for (size_t i = 1; i < this->partition.size(); ++i) {
      w = gsl_integration_fixed_alloc(T, this->number_of_nodes, this->partition[i-1], this->partition[i], 0.0, 0.0);
      gsl_integration_fixed(&to_be_integrated, &result, w);
      subinterval_integration.push_back(result);
    }

    result = std::accumulate(subinterval_integration.begin(), subinterval_integration.end(), 0.0);
    gsl_integration_fixed_free (w);
  }


  else if (family_of_polynomials == "Chebyshev Type 1") {
    gsl_integration_fixed_workspace  *w;
    const gsl_integration_fixed_type * T = gsl_integration_fixed_chebyshev;
    w = gsl_integration_fixed_alloc(T, this->number_of_nodes, this->begin, this->end, 0.0, 0.0);
    gsl_integration_fixed(&to_be_integrated, &result, w);
    gsl_integration_fixed_free (w);
  }


  else if (family_of_polynomials == "Gegenbauer") {
    gsl_integration_fixed_workspace  *w;
    const gsl_integration_fixed_type * T = gsl_integration_fixed_gegenbauer;
    w = gsl_integration_fixed_alloc(T, this->number_of_nodes, this->begin, this->end, this->alpha, 0.0);
    gsl_integration_fixed(&to_be_integrated, &result, w);
    gsl_integration_fixed_free (w);
  }


  else if (family_of_polynomials == "Jacobi") {
    gsl_integration_fixed_workspace  *w;
    const gsl_integration_fixed_type * T = gsl_integration_fixed_jacobi;
    w = gsl_integration_fixed_alloc(T, this->number_of_nodes, this->begin, this->end, this->alpha, this->beta);
    gsl_integration_fixed(&to_be_integrated, &result, w);
    gsl_integration_fixed_free (w);
  }


  else if (family_of_polynomials == "Exponential") {
    gsl_integration_fixed_workspace  *w;
    const gsl_integration_fixed_type * T = gsl_integration_fixed_exponential;
    w = gsl_integration_fixed_alloc(T, this->number_of_nodes, this->begin, this->end, this->alpha, 0);
    gsl_integration_fixed(&to_be_integrated, &result, w);
    gsl_integration_fixed_free (w);
  }


  else if (family_of_polynomials == "Chebyshev Type 2") {
    gsl_integration_fixed_workspace  *w;
    const gsl_integration_fixed_type * T = gsl_integration_fixed_chebyshev2;
    w = gsl_integration_fixed_alloc(T, this->number_of_nodes, this->begin, this->end, this->alpha, 0);
    gsl_integration_fixed(&to_be_integrated, &result, w);
    gsl_integration_fixed_free (w);
  }


  else {throw std::runtime_error("Invalid family of polynomials.");}
    
  return result;
}



/* Now we specialize the method in order to be able to integrate complex functions.
We just split them in real and imaginary part and compute the integral of each one of them. */


template<>
std::complex<double> Gaussian<std::complex<double>>::compute_integral() {

  Gaussian<double> Real{this->begin, this->end, this->subdivision_n, my_reality(this->integrand), this->number_of_nodes, this->family_of_polynomials};
  double x_1 = Real.compute_integral();

  Gaussian<double> Immaginary{this->begin, this->end, this->subdivision_n, imagine_dragons(this->integrand), this->number_of_nodes, this->family_of_polynomials};
  std::complex<double> result{x_1, Immaginary.compute_integral()};

  return result;
}


// This was the function for printing complex numbers in a readable way.

template <typename field>
std::string nicer_complex(std::complex<field> number) {
  std::stringstream s;
  s << std::real(number) << "+i*" << std::imag(number);
  return s.str();
}


template class Gaussian<double>;
template class Gaussian<std::complex<double>>;

template class Midpoint<double>;
template class Midpoint<std::complex<double>>;

template class Trapezoidal<double>;
template class Trapezoidal<std::complex<double>>;

template class Simpson<double>;
template class Simpson<std::complex<double>>;

template std::string nicer_complex(std::complex<double> number);
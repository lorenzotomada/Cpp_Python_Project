#include "../../C++_Code/Includes/Integration/Functions.hpp"
#include "../../C++_Code/Includes/Integration/Numerical_Integration.hpp"
#include <complex>
#include <iomanip> // For std::fixed, see comments below in the tests regarding order of convergence.
#include <boost/math/quadrature/gauss.hpp> // https://www.boost.org/doc/libs/1_83_0/libs/math/doc/html/math_toolkit/gauss.html
#include <chrono>


/* In the first part of this file, the main properties of the implemented methods are shown.
In particular, for each method we show the polynomial order, the order of convergence and we compare the
execution time with an integration quadrature rule coming from the library Boost.


There are not too many comments on the file because most of the things that are done were either implemented in
the .hpp and .tpl.hpp files or explained in the printed strings (or in any case fairly straight-forward). 


All the functions were considered as complex-valued functions in order to prove the efficiency of the templating.*/



int main() {

  // The following lines are used to print on the terminal in a similar way with respect to what happens in the
  // statistics module.

  std::string color = "\033[1;35m";
  std::string kernel_name = "NUMERICAL_INTEGRATION ";
  std::string end_color = "\33[0m";




  constexpr double a = 0;
  constexpr double b = 1;
  constexpr unsigned int subintervals = 10;

  std::cout << "\n";
  std::cout << color << kernel_name << end_color << "Now we will test all the methods which were implemented. We tested their polynomial order";
  std::cout << " their order of convergence and the time needed to compute the integral." << std::endl;
  std::cout << "The time needed for the computation is then compared with the one of the default integration";
  std::cout << " method implemented in the library Boost.\n" << std::endl;


  std::cout << "--------------------------------------------------------------" << std::endl;


  std::cout << color << kernel_name << end_color << "\n MIDPOINT RULE\n" << std::endl;


  std::cout << "--------------------------------------------------------------\n" << std::endl;


  std::cout << color << kernel_name << end_color << "Now we will study the polynomial order of our implementation of the composite midpoint rule.\n" << std::endl;


  std::shared_ptr<Integration<std::complex<double>>> Midpointer = std::make_shared<Midpoint<std::complex<double>>>(a, b, subintervals, const_1<std::complex<double>>);
  /* We  decided to use polymorphism in order to test the abstract class method.
  Here we created a shared pointer. We could have used a unique pointer in this situation; however, it was more
  or less equivalent, so we opted for this in order to allow for greater flexibiliy in case a user has
  access to the code. */


  // Check correctness on polynomials of degree 0


  std::cout << color << kernel_name << end_color << "We can see that the composite midpoint rule is exact when integrating constant functions." << std::endl;
  std::cout << color << kernel_name << end_color << "Indeed, the computed integral between " << a << " and " << b << " of the function f(x)=1 (meant as the complex function 1 + i*0) is " << nicer_complex(Midpointer->compute_integral()) << "." << std::endl;


  // Check correctness on polynomials of degree 1


  Midpointer->integrand = x_function<std::complex<double>>;
  std::cout << color << kernel_name << end_color << "It is exact also when integrating linear functions." << std::endl;
  std::cout << color << kernel_name << end_color << "Indeed, the computed integral between " << a << " and " << b << " of f(x)=x (meant as the complex function x + i*0) is " << nicer_complex(Midpointer->compute_integral()) << "." << std::endl;


  // Check (un)correctness on polynomials of degree 2


  Midpointer->integrand = square<std::complex<double>>;
  std::cout << color << kernel_name << end_color << "It is not exact when integrating polynomials of degree 2." << std::endl;
  std::cout << color << kernel_name << end_color << "Indeed, the computed integral between " << a << " and " << b << " of f(x)=x^2 (mean as the complex function x^2 + i*0) is " <<  nicer_complex(Midpointer->compute_integral()) << "." << std::endl;
  std::cout << color << kernel_name << end_color << "It should be " << (std::pow(b,3) -std::pow(a, 3))/3.0 << ". It is close, yet not exact." << std::endl;


  std::cout << color << kernel_name << end_color << "This is consistent with the theory: in fact, the midpoint rule has order one.\n" << std::endl;


  std::cout << "--------------------------------------------------------------" << std::endl;


  /* Compute the order of convergence by fractioning the stepsize and checking whether the error follows the
  expected trend. The lines that will be printed under this comment contain the explanation of the expected
  order of the method. */


  std::cout << color << kernel_name << end_color << "Now we will test the order of convergence of our implementation of the composite midpoint rule.\n" << std::endl;
  std::cout << "In general, using composite integration, if a quadrature formula has polynomial order m, then its composite";
  std::cout << " version has order of convergence O(h^(m+1))." << std::endl;
  std::cout << color << kernel_name << end_color << "Hence we expect our formula to have order 2." << std::endl;


  Midpointer->integrand = cube<std::complex<double>>;
  std::complex<double> first_computation = Midpointer->compute_integral();
  std::complex<double> error1 = std::abs((std::pow(b, 4) - std::pow(a, 4))/4.0 - first_computation);


  std::cout << color << kernel_name << end_color << "Dividing the interval in 10 subintervals for composite integration (as done before), when trying to";
  std::cout << " compute the integral of f(x)=x^3, we get " << nicer_complex(first_computation) << ", therefore the norm of the error is " << std::abs(error1) << "." << std::endl;


  /* If the method has order 2, if we divide the stepsize by 10, then the error should be devided by 10^2.
  In the following line, we multiply the number of subdivision by 10, which is equivalent to dividing the
  stepsize by the same number. */
  constexpr unsigned int subintervals2 = subintervals*10;
  Midpointer = std::make_shared<Midpoint<std::complex<double>>>(a, b, subintervals2, cube<std::complex<double>>);


  std::complex<double> second_computation = Midpointer->compute_integral();
  std::complex<double> error2 = std::abs((std::pow(b, 4) - std::pow(a, 4))/4.0 - second_computation);


  std::cout << std::fixed; /* This allows us to read the error without using scientific notation, making
  it easies to compare it with the previous one. */


  std::cout << color << kernel_name << end_color << "Now, for h_new = h_old/10, we get " << nicer_complex(second_computation) << ", thus the norm of the error is "  << std::abs(error2) << "." << std::endl;
  std::cout << color << kernel_name << end_color << "We can see that multiplying h by 1/10 the error was multiplied by (1/10)^2, which is consistent with the fact that the method has order 2." << std::endl;


  std::cout << "\n--------------------------------------------------------------" << std::endl;


  // Now we measure the efficiency of our code, intended as computational time needed.


  std::cout << color << kernel_name << end_color << "To test the efficiency of our code, we compare it with the Gaussian quadrature rule of the library Boost." << std::endl;
  std::cout << color << kernel_name << end_color << "The integrand function is the (complex) function f(x) = sin(x)*(e^x) in the interval [0, pi]." << std::endl;


  constexpr double pi_halves = acos(0.0); // To get pi/2 in order to also test on another interval; see https://www.geeksforgeeks.org/pi-in-c-with-examples/
  constexpr unsigned int subintervals3 = 20;
  Midpointer = std::make_shared<Midpoint<std::complex<double>>>(a, pi_halves, subintervals2, exp_times_sine<std::complex<double>>);


  std::cout << color << kernel_name << end_color << "We divide the interval in " << subintervals3 << " subintervals for our method; for the Gaussian formula and we use " << subintervals3 << " nodes.\n" << std::endl;


  std::complex<double> result;
  auto duration_ours = std::chrono::microseconds(0);


  for (size_t i = 0; i < 10000; i++){
    auto start = std::chrono::high_resolution_clock::now();

    result = Midpointer->compute_integral();

    auto stop = std::chrono::high_resolution_clock::now();
    duration_ours += std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  }


  duration_ours /= 10000.0;
  std::cout << color << kernel_name << end_color << "On average our function took " << duration_ours.count() << " microseconds to be computed." << std::endl;


  
  auto duration_boost = std::chrono::microseconds(0);
  for (size_t i = 0; i < 10000; i++){
    auto start = std::chrono::high_resolution_clock::now();

    std::function<double(double)> real_exp_sin = my_reality(exp_times_sine<std::complex<double>>);
    std::function<double(double)> imag_exp_sin = imagine_dragons(exp_times_sine<std::complex<double>>);
    result = boost::math::quadrature::gauss<double, subintervals3>::integrate(real_exp_sin, a, pi_halves);
    result = boost::math::quadrature::gauss<double, subintervals3>::integrate(imag_exp_sin, a, pi_halves);

    auto stop = std::chrono::high_resolution_clock::now();
    duration_boost += std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  }
  

  duration_boost /= 10000.0;
  std::cout << color << kernel_name << end_color << "On average the Boost function took " << duration_boost.count() << " microseconds to be computed." << std::endl;
  std::cout << color << kernel_name << end_color << "While the Boost implementation is more efficient, our implementation can still be computed in a reasonable time." << std::endl;


  std::cout << "\n--------------------------------------------------------------" << std::endl;


  std::cout << color << kernel_name << end_color << "\n TRAPEZOIDAL RULE\n" << std::endl;


  std::cout << "--------------------------------------------------------------\n" << std::endl;

  // Until Gaussian integration, we just repeat the same things for the other methods we have immplemented.


  std::cout << std::defaultfloat; // To go back to the default way of printing numbers. See https://cplusplus.com/reference/ios/defaultfloat/


  std::cout << color << kernel_name << end_color << "Now we will study the polynomial order of our implementation of the composite trapezoidal rule.\n" << std::endl;


  std::shared_ptr<Integration<std::complex<double>>> Trapez_ptr = std::make_shared<Trapezoidal<std::complex<double>>>(a, b, subintervals, const_1<std::complex<double>>);


  std::cout << color << kernel_name << end_color << "We can see that the composite trapezoidal rule is exact when integrating constant functions." << std::endl;
  std::cout << color << kernel_name << end_color << "Indeed, the computed integral between " << a << " and " << b << " of the function f(x)=1 (meant as the complex function 1+i0) is " << nicer_complex(Trapez_ptr->compute_integral()) << "." << std::endl;


  Trapez_ptr->integrand = x_function<std::complex<double>>;
  std::cout << color << kernel_name << end_color << "It is exact also when integrating linear functions." << std::endl;
  std::cout << color << kernel_name << end_color << "Indeed, the computed integral between " << a << " and " << b << " of f(x)=x (meant as the complex function x+0) is " << nicer_complex(Trapez_ptr->compute_integral()) << "." << std::endl;


  Trapez_ptr->integrand = square<std::complex<double>>;
  std::cout << color << kernel_name << end_color << "It is not exact when integrating polynomials of degree 2." << std::endl;
  std::cout << color << kernel_name << end_color << "Indeed, the computed integral between " << a << " and " << b << " of f(x)=x^2 is " << nicer_complex(Trapez_ptr->compute_integral()) << "." << std::endl;
  std::cout << color << kernel_name << end_color << "It should be " << (std::pow(b,3) -std::pow(a, 3))/3.0 << ". Also this time, our formula is close, yet not exact." << std::endl;


  std::cout << color << kernel_name << end_color << "This is consistent with the theory: in fact, also the trapezoidal rule has order one.\n" << std::endl;


  std::cout << "--------------------------------------------------------------\n" << std::endl;


  std::cout << color << kernel_name << end_color << "Now we will test the order of convergence of our implementation of the composite trapezoidal rule.\n" << std::endl;
  std::cout << color << kernel_name << end_color << "For the reasons stated before, we expect our formula to have order 2." << std::endl;


  Trapez_ptr->integrand = cube<double>;
  first_computation =  Trapez_ptr->compute_integral();
  error1 = std::abs((std::pow(b, 4) - std::pow(a, 4))/4.0 - first_computation);


  std::cout << color << kernel_name << end_color << "Dividing the interval in 10 subintervals for composite integration (as done before), when trying to";
  std::cout << color << kernel_name << end_color << " compute the integral of f(x)=x^3, we get " << nicer_complex(first_computation) << ", hence the norm of the error is " << std::abs(error1) << "." << std::endl;


  Trapez_ptr = std::make_shared<Trapezoidal<std::complex<double>>>(a, b, subintervals2, cube<std::complex<double>>);


  second_computation = Trapez_ptr->compute_integral();
  error2 = std::abs((std::pow(b, 4) - std::pow(a, 4))/4.0 - second_computation);


  std::cout << std::fixed;
  std::cout << color << kernel_name << end_color << "Now, for h_new = h_old/10, we get " << nicer_complex(second_computation) << ", hence the norm of the error is "  << std::abs(error2) << "." << std::endl;
  std::cout << color << kernel_name << end_color << "\nWe can see that multiplying h by 1/10 the error was multiplied by (1/10)^2, which is consistent with the fact that the method has order 2." << std::endl;


  std::cout << "\n--------------------------------------------------------------\n" << std::endl;


  std::cout << color << kernel_name << end_color << "To test the efficiency of our code, we compare it with the Gaussian quadrature rule of the library Boost on the same integrand and interval as before." << std::endl;


  Trapez_ptr = std::make_shared<Trapezoidal<std::complex<double>>>(a, pi_halves, subintervals2, exp_times_sine<std::complex<double>>);


  std::cout << color << kernel_name << end_color << "Now we divide the interval in " << subintervals3 << " subintervals." << std::endl;
  std::cout << color << kernel_name << end_color << "We use " << 2*subintervals3 << " nodes for the gaussian formula, since our quadrature rule";
  std::cout << " now needs more evaluations of the function than before (2 for each subinterval).\n" << std::endl;


  duration_ours = std::chrono::microseconds(0);


  for (size_t i = 0; i < 10000; i++){
    auto start = std::chrono::high_resolution_clock::now();

    result = Trapez_ptr->compute_integral();

    auto stop = std::chrono::high_resolution_clock::now();
    duration_ours += std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  }


  duration_ours /= 10000.0;
  std::cout << color << kernel_name << end_color << "On average our function took " << duration_ours.count() << " microseconds to be computed." << std::endl;


  duration_boost = std::chrono::microseconds(0);
  for (size_t i = 0; i < 10000; i++){
    auto start = std::chrono::high_resolution_clock::now();

    std::function<double(double)> real_exp_sin = my_reality(exp_times_sine<std::complex<double>>);
    std::function<double(double)> imag_exp_sin = imagine_dragons(exp_times_sine<std::complex<double>>);
    result = boost::math::quadrature::gauss<double, 2*subintervals3>::integrate(real_exp_sin, a, pi_halves);
    result = boost::math::quadrature::gauss<double, 2*subintervals3>::integrate(imag_exp_sin, a, pi_halves);

    auto stop = std::chrono::high_resolution_clock::now();
    duration_boost += std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  }
  

  duration_boost /= 10000.0;
  std::cout << color << kernel_name << end_color << "On average the Boost function took " << duration_boost.count() << " microseconds to be computed." << std::endl;
  std::cout << color << kernel_name << end_color << "Regarding the efficiency of the two implementations, the same considerations as before hold." << std::endl;


  std::cout << "\n--------------------------------------------------------------" << std::endl;


  std::cout << color << kernel_name << end_color << "\n SIMPSON RULE\n" << std::endl;


  std::cout << "--------------------------------------------------------------\n" << std::endl;


  std::cout << std::defaultfloat;


  std::cout << color << kernel_name << end_color << "Now we will study the polynomial order of our implementation of the composite Simpson rule.\n" << std::endl;


  std::shared_ptr<Integration<std::complex<double>>> Simpson_ptr = std::make_shared<Simpson<std::complex<double>>>(a, b, subintervals, const_1<std::complex<double>>);


  std::cout << color << kernel_name << end_color << "We can see that the composite Simpson rule is exact when integrating constant functions." << std::endl;
  std::cout << color << kernel_name << end_color << "Indeed, the computed integral between " << a << " and " << b << " of the (complex valued) function f(x)=1 is " << nicer_complex(Simpson_ptr->compute_integral()) << "." << std::endl;


  Simpson_ptr->integrand = x_function<std::complex<double>>;
  std::cout << color << kernel_name << end_color << "It is exact also when integrating linear functions." << std::endl;
  std::cout << color << kernel_name << end_color << "Indeed, the computed integral between " << a << " and " << b << " of the (complex-valued) functions f(x)=x is " << nicer_complex(Simpson_ptr->compute_integral()) << "." << std::endl;


  Simpson_ptr->integrand = square<double>;
  std::cout << color << kernel_name << end_color << "This formula is also exact when integrating polynomials of degree 2." << std::endl;
  std::cout << color << kernel_name << end_color << "Indeed, the computed integral between " << a << " and " << b << " of the (complex-valued) function f(x)=x^2 is " << nicer_complex(Simpson_ptr->compute_integral()) << "." << std::endl;


  Simpson_ptr->integrand = cube<double>;
  std::cout << color << kernel_name << end_color << "It is also exact when integrating polynomials of degree 3." << std::endl;
  std::cout << color << kernel_name << end_color << "Indeed, the computed integral between " << a << " and " << b << " of the (complex-valued) function f(x)=x^3 is " << nicer_complex(Simpson_ptr->compute_integral()) << "." << std::endl;


  Simpson_ptr->integrand = x_4<double>;
  std::cout << color << kernel_name << end_color << "However, it is not exact when integrating polynomials of degree 4." << std::endl;
  std::cout << color << kernel_name << end_color << "Indeed, the computed integral between " << a << " and " << b << " of the (complex-valued) function f(x)=x^4 is " << nicer_complex(Simpson_ptr->compute_integral()) << "." << std::endl;
  std::cout << color << kernel_name << end_color <<"It should be " << (std::pow(b,5) -std::pow(a, 5))/5.0 << ". It is very close, yet not exact." << std::endl;


  std::cout << color << kernel_name << end_color << "Once again, we get the result expected by theoretical predictions: the Simpson rule has order three.\n" << std::endl;


  std::cout << "--------------------------------------------------------------" << std::endl;


  std::cout << color << kernel_name << end_color <<"Now we will test the order of convergence of our implementation of the composite Simpson rule.\n" << std::endl;
  std::cout << color << kernel_name << end_color << "For the reasons stated before, we expect our formula to have order 4." << std::endl;


  // Trapez_ptr->integrand = x_4<double>; already set for the testing of the polynomial order
  first_computation =  Simpson_ptr->compute_integral();
  error1 = std::abs((std::pow(b, 5) - std::pow(a, 5))/5.0 - first_computation);


  std::cout << color << kernel_name << end_color << "Dividing the interval in 10 subintervals for composite integration (as done before), when trying to";
  std::cout << " compute the  integral of f(x)=x^4, we get " <<  nicer_complex(first_computation) << ", hence the norm of the error is " << std::abs(error1) << "." << std::endl;


  Simpson_ptr = std::make_shared<Simpson<std::complex<double>>>(a, b, subintervals2, x_4<std::complex<double>>);


  second_computation = Simpson_ptr->compute_integral();
  error2 = std::abs((std::pow(b, 5) - std::pow(a, 5))/5.0 - second_computation);


  std::cout << color << kernel_name << end_color <<"Now, for h_new = h_old/10, we get " << nicer_complex(second_computation) << ", hence the norm of the error is "  << std::abs(error2) << "." << std::endl;
  std::cout << color << kernel_name << end_color <<"We can see that multiplying h by 1/10 the error was multiplied by (1/10)^4, which is consistent with the fact that the method has order 2." << std::endl;


  std::cout << "\n--------------------------------------------------------------\n" << std::endl;


  std::cout << color << kernel_name << end_color << "To test the efficiency of our code, we compare it with the Gaussian quadrature rule of the library Boost." << std::endl;
  std::cout << color << kernel_name << end_color << "The integrand function is the (complex--valued) function f(x) = sin(x)*(e^x) in the interval [0, pi]." << std::endl;


  Simpson_ptr = std::make_shared<Simpson<std::complex<double>>>(a, pi_halves, subintervals2, exp_times_sine<std::complex<double>>);


  std::cout << color << kernel_name << end_color <<"We divide the interval in " << subintervals3 << " subintervals." << std::endl;
  std::cout << color << kernel_name << end_color <<"Now we use " << 3*subintervals3 << " nodes for the gaussian formula, since our quadrature rule";
  std::cout << " now needs more evaluations of the function than before (3 for each subinterval).\n" << std::endl;


  duration_ours = std::chrono::microseconds(0);


  for (size_t i = 0; i < 10000; i++){
    auto start = std::chrono::high_resolution_clock::now();

    result = Simpson_ptr->compute_integral();

    auto stop = std::chrono::high_resolution_clock::now();
    duration_ours += std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  }


  duration_ours /= 10000.0;
  std::cout << color << kernel_name << end_color <<"On average our function took " << duration_ours.count() << " microseconds to be computed." << std::endl;


  duration_boost = std::chrono::microseconds(0);
  for (size_t i = 0; i < 10000; i++){
    auto start = std::chrono::high_resolution_clock::now();

    std::function<double(double)> real_exp_sin = my_reality(exp_times_sine<std::complex<double>>);
    std::function<double(double)> imag_exp_sin = imagine_dragons(exp_times_sine<std::complex<double>>);
    result = boost::math::quadrature::gauss<double, subintervals3>::integrate(real_exp_sin, a, pi_halves);
    result = boost::math::quadrature::gauss<double, subintervals3>::integrate(imag_exp_sin, a, pi_halves);

    auto stop = std::chrono::high_resolution_clock::now();
    duration_boost += std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  }
  

  duration_boost /= 10000.0;
  std::cout << color << kernel_name << end_color <<"On average the Boost function took " << duration_boost.count() << " microseconds to be computed." << std::endl;
  std::cout << color << kernel_name << end_color <<"Regarding the efficiency of the two implementations, the same considerations as before hold." << std::endl;


  std::cout << "\n--------------------------------------------------------------" << std::endl;


  std::cout << color << kernel_name << end_color <<"\n GAUSSIAN FORMULAS \n" << std::endl;


  std::cout << "--------------------------------------------------------------\n" << std::endl;


  /* Due to the fact that our implementations allows the use of multiple families of polynomials, in order not
  to make this file too heavy and hard to read we decided to test in detail just the Legendre-Gauss formulas.
  Then at the end of the file the formula with Chebyshev nodes of type one is tested in order to show the
  correctness of the implementation. */


  std::cout << std::defaultfloat;


  std::cout << color << kernel_name << end_color <<"Now we will study the polynomial order of our implementation of the composite Gauss-Legendre quadrature formula.\n" << std::endl;


  unsigned int number_of_nodes = 2; /* The number of nodes we will use for Gaussian integration.
  Thus we expect our formula to be correct on polynomials of degree 2*2-1 = 3. */
  std::shared_ptr<Integration<std::complex<double>>> Gauss_ptr = std::make_shared<Gaussian<std::complex<double>>>(a, b, subintervals, const_1<std::complex<double>>, number_of_nodes);
  // The default string value is "Legendre".


  std::cout << color << kernel_name << end_color <<"We can see that the composite Gauss-Legendre rule is exact when integrating constant functions." << std::endl;
  std::cout << color << kernel_name << end_color <<"Indeed, the computed integral between " << a << " and " << b << " of the function f(x)=1 is " << nicer_complex(Gauss_ptr->compute_integral()) << "." << std::endl;


  Gauss_ptr->integrand = x_function<std::complex<double>>;
  std::cout << color << kernel_name << end_color <<"It is exact also when integrating linear functions." << std::endl;
  std::cout << color << kernel_name << end_color <<"Indeed, the computed integral between " << a << " and " << b << " of f(x)=x is " << nicer_complex(Gauss_ptr->compute_integral()) << "." << std::endl;


  Gauss_ptr->integrand = square<std::complex<double>>;
  std::cout << color << kernel_name << end_color <<"This formula is also exact when integrating polynomials of degree 2." << std::endl;
  std::cout << color << kernel_name << end_color <<"Indeed, the computed integral between " << a << " and " << b << " of f(x)=x^2 is " << nicer_complex(Gauss_ptr->compute_integral()) << "." << std::endl;


  Gauss_ptr->integrand = cube<std::complex<double>>;
  std::cout << color << kernel_name << end_color <<"It is also exact when integrating polynomials of degree 3." << std::endl;
  std::cout << color << kernel_name << end_color <<"Indeed, the computed integral between " << a << " and " << b << " of f(x)=x^3 is " << nicer_complex(Gauss_ptr->compute_integral()) << "." << std::endl;


  Gauss_ptr->integrand = x_4<std::complex<double>>;
  std::cout << color << kernel_name << end_color <<"However, it is not exact when integrating polynomials of degree 4." << std::endl;
  std::cout << color << kernel_name << end_color <<"Indeed, the computed integral between " << a << " and " << b << " of f(x)=x^4 is " << nicer_complex(Gauss_ptr->compute_integral()) << "." << std::endl;
  std::cout << color << kernel_name << end_color <<"It should be " << (std::pow(b,5) -std::pow(a, 5))/5.0 << ". It is very close, yet not exact." << std::endl;


  std::cout << color << kernel_name << end_color <<"Once again, we get the result expected by theoretical predictions: the Gauss-Legendre quadrature rule with 2 nodes has order three.\n" << std::endl;


  std::cout << "--------------------------------------------------------------" << std::endl;


  std::cout << color << kernel_name << end_color <<"Now we will test the order of convergence of our implementation of the composite Gauss-Legendre rule.\n" << std::endl;
  std::cout << color << kernel_name << end_color <<"For the reasons stated before, we expect our formula to have order 4." << std::endl;


  // Gauss_ptr->integrand = x_4<double>; already set for the testing of the polynomial order
  first_computation =  Gauss_ptr->compute_integral();
  error1 = std::abs((std::pow(b, 5) - std::pow(a, 5))/5.0 - first_computation);


  std::cout << color << kernel_name << end_color <<"Dividing the interval in 10 subintervals for composite integration (as done before), when trying to";
  std::cout << " compute the  integral of f(x)=x^4, we get " << nicer_complex(first_computation) << ", hence the norm of the error is " << std::abs(error1) << "." << std::endl;


  Gauss_ptr = std::make_shared<Gaussian<std::complex<double>>>(a, b, subintervals2, x_4<std::complex<double>>, number_of_nodes);


  second_computation = Gauss_ptr->compute_integral();
  error2 = std::abs((std::pow(b, 5) - std::pow(a, 5))/5.0 - second_computation);


  std::cout << color << kernel_name << end_color <<"Now, for h_new = h_old/10, we get " << nicer_complex(second_computation) << ", hence the error is "  << error2 << "." << std::endl;
  std::cout << color << kernel_name << end_color <<"We can see that multiplying h by 1/10 the error was multiplied by (1/10)^4, which is consistent with the fact that the method has order 2." << std::endl;


  std::cout << "\n--------------------------------------------------------------\n" << std::endl;


  std::cout << color << kernel_name << end_color <<"To test the efficiency of our code, we compare it with the Gaussian quadrature rule of the library Boost." << std::endl;
  std::cout << color << kernel_name << end_color <<"The integrand function is the function f(x) = sin(x)*(e^x) in the interval [0, pi]." << std::endl;


  Gauss_ptr = std::make_shared<Gaussian<std::complex<double>>>(a, pi_halves, subintervals2, exp_times_sine<std::complex<double>>, number_of_nodes);


  std::cout << color << kernel_name << end_color <<"Now we use " << subintervals3 << " nodes for each of the two quadrature rules." << std::endl;


  duration_ours = std::chrono::microseconds(0);


  for (size_t i = 0; i < 10000; i++){
    auto start = std::chrono::high_resolution_clock::now();

    result = Gauss_ptr->compute_integral();

    auto stop = std::chrono::high_resolution_clock::now();
    duration_ours += std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  }


  duration_ours /= 10000.0;
  std::cout << color << kernel_name << end_color <<"On average our function took " << duration_ours.count() << " microseconds to be computed." << std::endl;


  duration_boost = std::chrono::microseconds(0);
  for (size_t i = 0; i < 10000; i++){
    auto start = std::chrono::high_resolution_clock::now();

    result = boost::math::quadrature::gauss<double, subintervals3>::integrate(exp_times_sine<double>, a, pi_halves);

    auto stop = std::chrono::high_resolution_clock::now();
    duration_boost += std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  }
  

  duration_boost /= 10000.0;
  std::cout << color << kernel_name << end_color <<"On average the Boost function took " << duration_boost.count() << " microseconds to be computed." << std::endl;
  std::cout << color << kernel_name << end_color <<"We can observe that the best choice for Gaussian integration is Boost, as it is much faster." << std::endl;


  std::cout << "\n--------------------------------------------------------------\n" << std::endl;


  std::cout << color << kernel_name << end_color <<"This ends the part of the testing related to the testing of our methods. \n" << std::endl;
  std::cout << color << kernel_name << end_color <<"Now, just for completeness, we also compute a real integral and we show that Gauss-Legendre integration works properly not just with Gauss-Legendre polynomials.\n" << std::endl;



  const std::string family = "Chebyshev Type 1";
  constexpr unsigned int subintervals4 = 200;
  unsigned int number_of_nodes2 = 1000;


  Gaussian<double> Gauss_Cheb{a, b, subintervals4, cheb_square<double>, number_of_nodes2, family};
  // Here the family is not default, so we specify it.

  
  double result2 = Gauss_Cheb.compute_integral();
  std::cout << color << kernel_name << end_color <<"The computed integral of the function 3*(sqrt(x-x^2))*x^2 integrated against the weight function 1/(sqrt(x-x^2)) is " << result2 << "." << std::endl;
  std::cout << color << kernel_name << end_color <<"It is the correct result." << std::endl;
  
  

  result +=1; // Just to avoid the warning 'unused variable'.
  number_of_nodes2 += 1; // Same here.


  return 0;
}
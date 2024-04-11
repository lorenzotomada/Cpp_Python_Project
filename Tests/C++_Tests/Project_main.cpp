// For the statistics part

#include "../../C++_Code/Includes/Statistics/Data_Handling.hpp"
#include "../../C++_Code/Includes/Statistics/Data.hpp"
#include "../../C++_Code/Includes/Statistics/Test_QoL.hpp"
#include <unordered_set>
#include <sstream> 


// For the numerical part

#include "../../C++_Code/Includes/Integration/Functions.hpp"
#include "../../C++_Code/Includes/Integration/Numerical_Integration.hpp"
#include <complex>
#include <iomanip> // For std::fixed, see comments below in the tests regarding order of convergence.
#include <boost/math/quadrature/gauss.hpp> // https://www.boost.org/doc/libs/1_83_0/libs/math/doc/html/math_toolkit/gauss.html
#include <chrono>




int main(){
  std::string color = "\033[1;34m";
  std::string end_color = "\033[0m ";
  std::string s = "s";
  std::string kernel_name = "PROJECT";
  std::unordered_set<std::string> acceptable_answers = {"I", "Integration", "S", "Statistics", "Q","Quit", "q", "INTEGRATION", "i", "s", "STATISTICS", "esc", "QUIT", "ESC", "^["};
  std::unordered_set<std::string> integration_answers = {"I", "Integration", "INTEGRATION", "i"};
  std::unordered_set<std::string> statistics_answers =  {"S", "Statistics", "s", "STATISTICS"};
  std::unordered_set<std::string> quitting_answers =  {"Q", "Quit", "q", "QUIT", "esc", "ESC", "^["};
  std::string quit = "";
  while (!quitting_answers.count(quit)) {
    std::string phrase_1 = "What do you wish to test? (I = Integration, S = Statistics, Q = quit). To get a full documentation of the project type: ?\n";
    quit = ReadMe_choice(acceptable_answers, phrase_1, color, kernel_name, end_color);
    if (integration_answers.count(quit)) {
      std::string color = "\033[1;35m";
      std::string kernel_name = "NUMERICAL_INTEGRATION";
      std::cout << color << kernel_name << ">" << end_color;
     // Numerical part




/* In the first part of this file, the main properties of the implemented methods are shown.
In particular, for each method we show the polynomial order, the order of convergence and we compare the
execution time with an integration quadrature rule coming from the library Boost.


There are not too many comments on the file because most of the things that are done were either implemented in
the .hpp files or explained in the printed strings (or in any case fairly straight-forward). 


All the functions were considered as complex-valued functions in order to prove the efficiency of the templating.*/



  // The following lines are used to print on the terminal in a similar way with respect to what happens in the
  // statistics module.



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
  or less equivalent, so we opted for this option in order to allow for greater flexibiliy in case a user has
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

    }
    else if (statistics_answers.count(quit)) {
      std::cout << "\n \n " << std::endl;
      std::cout << "========================================================================================================================================================================" << std::endl;
      std::cout << "\n \n \n \n \n" << std::endl;
      color = "\033[1;33m";
      kernel_name = "STATISTICAL_ANALYSIS";
      std::cout << color << kernel_name << ">" << end_color;
      std::cout << "Would you like to get information about a custom file or would you like to test the code on any one of the files contained in CSV_files? [1/2]" << std::endl;
      std::cin >> s;
      if (s == "1") {
	std::string path_of_the_file = "";
	std::string name_of_the_file = "";
	while (true) {
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Please insert the full path of the file you wish to open. Alternatively you may also specify the relative path starting from the build directory." << std::endl;
	  std::cin >> path_of_the_file;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Please insert the full path of the file you wish to open. Alternatively you may also specify the relative path starting from the build directory." << std::endl;
	  std::cin >> name_of_the_file;
	  std::cout << color << kernel_name << ">" << end_color;
	  CSV_Handler Your_CSV = {path_of_the_file, name_of_the_file, false};
	  try {
	    Your_CSV.open();
	    break;
	  }
	  catch(std::runtime_error &e) {
	    std::cerr << "The file could not be opened. Please make sure you are using the correct path." << std::endl;
	  }
	}
	CSV_Handler Your_CSV = {path_of_the_file, name_of_the_file, true};
	std::string choice = "";
	std::unordered_set<std::string> acceptable_choices = {"y", "Yes", "Sì", "Si", "s", "si", "sì", "yes", "YES", "Y", "n", "No", "NO", "N"};
	std::unordered_set<std::string> affirmative_choices = {"y", "Yes", "Sì", "Si", "s", "si", "sì", "yes", "YES", "Y"};
	std::unordered_set<std::string> negative_choices = {"n", "No", "NO", "N"};
	phrase_1 = "Do you wish to store the informations of the file in a container? [y/n] (Useful if you need to have access quickly to all of the data. If you only need to access specific data, we advise to avoid this choice so as to minimise space complexity)\n" + color + kernel_name + ">" + end_color + "To know more about the data structure we used to store your file's data, please print ?.\n";
	choice = ReadMe_choice(acceptable_choices, phrase_1, color, kernel_name, end_color);
	if (affirmative_choices.count(choice)) {
	  Data_Table Your_Table = Convert_file_to_Table(Your_CSV);
	  phrase_1 = "Would you like do execute a specific action (press ? to see allowed operations) or would you like to get a file summarizing all the relevant informations?" ;
	  acceptable_choices = {"q", "2", "1"};
	  choice = multiple_choice(acceptable_choices, phrase_1, true, true, color, kernel_name, end_color);
	  if (choice == "1") {
	    while (choice != "q") {
	      acceptable_choices = {"q", "quit","access", "mean", "median", "variance", "std_dev", "standard deviation", "standard_deviation", "number of columns", "n_columns", "print keys", "keys", "correlation"};
	      phrase_1 = "What would you like to do? (Pressing ? will print the list of allowed operations).";
	      choice = multiple_choice(acceptable_choices, phrase_1, true, false, color, kernel_name, end_color);
	      if (choice == "mean") {
		acceptable_choices = {Your_Table.get_column_keys()[0]};
		for (unsigned int i = 1; i < Your_Table.get_column_keys().size(); ++i) {
		  acceptable_choices.insert(Your_Table.get_column_keys()[i]);
		}
		phrase_1 = "Which column would you like to compute the mean of? (press ? to get a list of all keys)";
		choice = multiple_choice(acceptable_choices, phrase_1, false, false, color, kernel_name, end_color);
		std::cout << color << kernel_name << ">" << end_color;
		std::cout << "The mean value of the column: " << choice << " is:\n";
		try {
		  std::cout << color << kernel_name << ">" << end_color;
		  std::cout << Your_Table.compute_mean(choice) << std::endl;
		}
		catch(std::runtime_error &e) {
		  std::cout << "not computable since there are no numerical values." << std::endl;
		}
		
	      }
	      else if (choice == "median") {
		acceptable_choices = {Your_Table.get_column_keys()[0]};
		for (unsigned int i = 1; i < Your_Table.get_column_keys().size(); ++i) {
		  acceptable_choices.insert(Your_Table.get_column_keys()[i]);
		}
		phrase_1 = "Which column would you like to compute the median of? (press ? to get a list of all keys)";
		choice = multiple_choice(acceptable_choices, phrase_1, false, false, color, kernel_name, end_color);
		std::cout << color << kernel_name << ">" << end_color;
		std::cout << "The median value of the column: " << choice << " is:\n";
		try {
		  std::cout << color << kernel_name << ">" << end_color;
		  std::cout << Your_Table.compute_median(choice) << std::endl;
		}
		catch(std::runtime_error &e) {
		  std::cout << "not computable since there are no numerical values." << std::endl;
		}
		
	      }
	      else if (choice == "variance") {
		acceptable_choices = {Your_Table.get_column_keys()[0]};
		for (unsigned int i = 1; i < Your_Table.get_column_keys().size(); ++i) {
		  acceptable_choices.insert(Your_Table.get_column_keys()[i]);
		}
		phrase_1 = "Which column would you like to compute the variance of? (press ? to get a list of all keys)";
		choice = multiple_choice(acceptable_choices, phrase_1, false, false, color, kernel_name, end_color);
		std::cout << color << kernel_name << ">" << end_color;
		std::cout << "The variance of the column: " << choice << " is:\n";
		try {
		  std::cout << color << kernel_name << ">" << end_color;
		  std::cout << Your_Table.compute_variance(choice) << std::endl;
		}
		catch(std::runtime_error &e) {
		  std::cout << "not computable since there are no numerical values." << std::endl;
		}
		
	      }
	      if (choice == "std_dev" || choice == "standard deviation" || choice == "standard_deviation") {
		acceptable_choices = {Your_Table.get_column_keys()[0]};
		for (unsigned int i = 1; i < Your_Table.get_column_keys().size(); ++i) {
		  acceptable_choices.insert(Your_Table.get_column_keys()[i]);
		}
		phrase_1 = "Which column would you like to compute the standard deviation of? (press ? to get a list of all keys)";
		choice = multiple_choice(acceptable_choices, phrase_1, false, false, color, kernel_name, end_color);
		std::cout << color << kernel_name << ">" << end_color;
		std::cout << "The standard deviation of the column: " << "choice is:\n";
		try {
		  std::cout << color << kernel_name << ">" << end_color;
		  std::cout << Your_Table.compute_std_dev(choice) << std::endl;
		}
		catch(std::runtime_error &e) {
		  std::cout << "not computable since there are no numerical values." << std::endl;
		}
		
	      }
	      if (choice == "keys" || choice == "print keys") {
		std::cout << color << kernel_name << ">" << end_color << "The keys are: ";
		for (unsigned int i = 0; i < Your_Table.get_column_keys().size(); ++i) {
		  std::cout << Your_Table.get_column_keys()[i] << ", ";
		}
	      }
	      if (choice == "n_rows" || choice == "number of rows") {
		std::cout << color << kernel_name << ">" << end_color << "The file has: " << Your_Table.get_n_rows(); 
	      }
	    
	    }
	  }
	  if (choice == "2" || choice == "summary") {
	    std::string txt_name = Your_Table.summary();
	    std::string summary_file = "cat " + txt_name;
	    acceptable_choices = {"y", "Yes", "Sì", "Si", "s", "si", "sì", "yes", "YES", "Y", "n", "No", "NO", "N"};
	    while (true) {
	      std::cout << color << kernel_name << ">" << end_color;
	      std::cout << "Would you like to print the result?" << std::endl;
	      std::cin >> choice;
	      if (acceptable_choices.count(choice)) {
		break;
	      }
	      else {
		std::cout << color << kernel_name << ">" << end_color;
		std::cout << "please type a valid answer" << std::endl;
	      }
	    }
	    if (affirmative_choices.count(choice)) {
	      system(summary_file.c_str());
	    }
	  }
	
	
	}
      }
      if (s== "2") {
	std::string file_name = "Undefined";
	std::unordered_set<std::string> acceptable_names = {"Stores", "Stores_Data", "Stores Data", "Banking", "Banking_Data", "Banking Data", "Banks", "Housing_Data", "Housing Data", "California_Housing", "California Housing", "Housing", "Housing_2", "Housing_Data_2", "Housing Data 2","SUS_Housing", "Housing 2", "Sus_Housing", "SUS Housing", "US Crime Rates", "US_Crime_Rates"};
	std::unordered_set<std::string> stores_names = {"Stores", "Stores_Data", "Stores Data"};
	std::unordered_set<std::string> bank_names = {"Banking", "Banking_Data", "Banking Data", "Banks"};
	std::unordered_set<std::string> housing_names = {"Housing_Data", "Housing Data", "California_Housing", "California Housing", "Housing"};
	std::unordered_set<std::string> SUS_housing_names = {"Housing_2", "Housing_Data_2", "Housing Data 2","SUS_Housing", "Housing 2", "Sus_Housing", "SUS Housing"};
	std::unordered_set<std::string> US_crime_names = {"US Crime Rates", "US_Crime_Rates"};
	std::unordered_map<std::string, std::unordered_set<std::string>> mp;
	mp["Stores_Data"] = stores_names;
	mp["Banking_Data"] = bank_names;
	mp["Housing_Data"] = housing_names;
	mp["Housing_Data_2"] = SUS_housing_names;
	mp["US_Crime_Rates"] = US_crime_names;
	while (true) {
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Please insert the name of the file you wish to analise. Press ? for a list of accepted names." << std::endl;
	  std::cin>> file_name;
	  if (file_name == "?") {
	    for (auto it:mp) {
	      std::cout << color << kernel_name << ">" << end_color;
	      std::cout << "To operate on the file named: " << it.first << ".csv, you can type any of the following: ";
	      for (std::string acceptable_n : it.second) {
		std::cout << acceptable_n << ", ";
	      }
	      std::cout << "\n";
	    }
	  }
	  else if (acceptable_names.count(file_name)) {
	    break;
	  }
	  else{
	    std::cout << color << kernel_name << ">" << end_color << " please enter a valid file name. Press ? for help." << std::endl;
	  }
	}
	if (stores_names.count(file_name)) {
	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "The file has " << Stores_Data.get_n_rows() << " rows." << std::endl;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "The file has " << Stores_Data.get_column_keys().size() << " columns. Their names are:" << std::endl;
	  for (size_t i = 0; i < Stores_Data.get_column_keys().size(); ++i){
	    if (i == Stores_Data.get_column_keys().size()-1) {std::cout << Stores_Data.get_column_keys()[i] << std::endl;}
	    else {std::cout << Stores_Data.get_column_keys()[i] << ", ";}  
	  }
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "There are " << Stores_Data.count_NaNs() << " missing values in the dataset." << std::endl;

	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

	  // Some examples for testing.
	  std::string shopping_mall = "shopping_mall";
	  std::string cost_price_per_unit = "cost_price_per_unit";
	  std::string selling_price_per_unit = "selling_price_per_unit";
	  std::string age = "age";
	  std::string category = "category";
	  std::string test_age = "43";
	  std::string state = "state";
	  std::string gender = "gender";
	  std::string North_Carolina = "North Carolina";
	  std::string Female = "Female";
	  std::string Kanyon = "Kanyon";

	  unsigned int x_1 = Stores_Data.frequency(age, test_age);
	  unsigned int x_2 = Stores_Data.frequency(state, North_Carolina);
	  unsigned int x_3 = Stores_Data.frequency(gender,Female);
	  unsigned int x_4 = Stores_Data.frequency(shopping_mall, Kanyon);

	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "There are " << x_1  << " owners of age 43, " << percentage(x_1, Stores_Data.get_n_rows()) << " of the total number of owners." << std::endl;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "There are " << x_2 << " owners in North Carolina, "  << percentage(x_2, Stores_Data.get_n_rows()) << " of the total number of shops." << std::endl;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "There are " << x_3 << " female owners, " <<  percentage(x_3, Stores_Data.get_n_rows()) << " of the total number of owners." << std::endl;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "There are " << x_4 << " Kanyon shops, " << percentage(x_4, Stores_Data.get_n_rows()) << " of the total amount." << std::endl;


	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Now we want to test the access." << std::endl;

	  std::unordered_map<std::string, std::string> first_line = Stores_Data.get_Table()[4];
	  std::string entry = first_line[shopping_mall];
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "The element in the 4th row with key " << shopping_mall << " is " << entry << "." << std::endl;

	  unsigned int idx;
	  std::string key;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Please enter a row index: " << std::endl;
	  std::cin >> idx;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Please enter the name of a column: " << std::endl;
	  std::cin >> key;

	  try {
	    std::variant<double, std::string> element = Stores_Data(idx, key);

	    if (std::holds_alternative<std::string>(element)) {
	      std::string s = std::get<std::string>(element);
	      std::cout << color << kernel_name << ">" << end_color;
	      std::cout << "The element in position " << idx << " in the column " << key << " is " << s << "." << std::endl;
	    }

	    if (std::holds_alternative<double>(element)) {
	      double d = std::get<double>(element);
	      std::cout << color << kernel_name << ">" << end_color;
	      std::cout << "The element in position " << idx << " in the column " << key << " is " << d << "." << std::endl;
	    }
	  }
	  catch (const NaNException &e) {
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cerr << "Error: the value is missing." << std::endl;
	  }
	  catch (const std::out_of_range& e) {
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cerr << "Error: indices out of bounds." << std::endl;
	  }


	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Now we will test our iterators. The following are the first 100 shopping malls:" << std::endl;
	  unsigned int counter = 0;
	  for (auto it : Stores_Data) {
	    std::cout << it["shopping_mall"] << ", ";
	    if (counter > 100) {
	      break;
	    }
	    counter ++;
	  }

	  std::cout << "\n";
	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;


	  std::string checker = "";
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Would you like to create a file summarising all the data contained in the file? [y/n]" << std::endl;
	  std::cin >> checker;
	  if (checker == "y") {
	    std::string file_named = Stores_Data.summary();
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "Would you like to print the content of the file you've just created? [y/n]" << std::endl;
	    std::cin >> checker;
	    if (checker == "y") {
	      std::string txt_file_name = "cat "+file_named;
	      system(txt_file_name.c_str());
	    }
	  }


	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;


	  try {
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "The mean of the column " << category << " is: " << Stores_Data.compute_mean(category) << "." << std::endl;
	  }
	  catch (const std::runtime_error &e) {std::cout << "the column holds no numerical data." << std::endl;}

	  try {
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "The mean of the column " << age << " is: " << Stores_Data.compute_mean(age) << "." << std::endl;
	  }
	  catch (std::runtime_error &e) {
	    std::cout << "The mean of the column age could not be computed as there are no numerical values" << std::endl;
	  }
	  try {
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "The median of the column " << age << " is: " << Stores_Data.compute_median(age) << "." << std::endl;
	  }
	  catch (std::runtime_error &e) {
	    std::cout << "The median of the column age could not be computed as there are no numerical values" << std::endl;
	  }
	  try {
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "The variance of the column " << age << " is: " << Stores_Data.compute_variance(age) << "." << std::endl;
	  }
	  catch (std::runtime_error &e) {
	    std::cout << "The variance of the column age could not be computed as there are no numerical values" << std::endl;
	  }
	  try {
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "The standard deviationn of the column " << age << " is: " << Stores_Data.compute_std_dev(age) << "." << std::endl;
	  }
	  catch (std::runtime_error &e) {
	    std::cout << "The standard deviation of the column age could not be computed as there are no numerical values" << std::endl;
	  }
	  try {
	    std::cout << color << kernel_name << ">" << end_color;
	    double correlation = Stores_Data.compute_correlation(cost_price_per_unit, selling_price_per_unit);
	    std::cout << "The correlation between the columns " << cost_price_per_unit << " and " << selling_price_per_unit << " is " << correlation << "." << std::endl;

	  }
	  catch (std::range_error &e) {
	    std::cout << "The correlation of the columns \"cost price per unit\" and \"selling price per unit\" could not be computed as the two have different sizes." << std::endl;
	  }
	  catch (std::runtime_error &e) {
	    std::cout << "The correlation of the columns \"cost price per unit\" and \"selling price per unit\" could not be computed as in one of the two there are no numerical values" << std::endl;
	  }
	  
	  
	  

	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;


	  double max_age = Stores_Data.column_max(age);
	  double min_age = Stores_Data.column_min(age);

	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "The max in the column " << age << " is " << max_age << "." << std::endl;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "The min in the column " << age << " is " << min_age << "." << std::endl;


	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Now we select all the rows containing 43 as age." << std::endl;
	  std::vector<std::unordered_map<std::string, std::string>> age_43 = Stores_Data.select_values(age, test_age);
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "The number of people who are " << test_age << " is (again) " << age_43.size() << "." << std::endl;


	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;


	  unsigned int row_to_drop = 0;
	  Stores_Data.drop_row(row_to_drop);
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "We have dropped a row. Now the number of rows is " << Stores_Data.get_n_rows() << "." << std::endl;

	  Stores_Data.drop_column(state);
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "We have dropped a column. Now the number of columns is " << Stores_Data.get_column_keys().size() << "." << std::endl;

	  Stores_Data.drop_NaNs();
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "We have dropped all the rows containing missing values. Now the number of rows is " << Stores_Data.get_n_rows() << "." << std::endl;

	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	  
	}

        else if (housing_names.count(file_name)) {
	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Now we will test our iterators by printing the first 100 elements of each column." << std::endl;
	  for (std::string key : CA_Housing_Data.get_column_keys()) {
	    unsigned int counter = 0;
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "The first 100 elements of the column: " << key << " is:\n" << std::endl;
	    std::cout << color << kernel_name << ">" << end_color;
	    for (auto it : CA_Housing_Data) {
	      std::cout << it[key] << ", ";
	      if (counter > 100) {
		break;
	      }
	      counter ++;
	    }
	    std::cout << "\n\n" << std::endl;
	  }
	  std::string keyy = "median_house_value";
	  unsigned int idx = 1;
	  std::variant<double, std::string> elements = CA_Housing_Data(idx, keyy);
	  //std::cout << std::get<double>(elements) << "\n";
	  try {
	    std::variant<double, std::string> element = CA_Housing_Data(idx, keyy);

	    if (std::holds_alternative<std::string>(element)) {
	      std::string s = std::get<std::string>(element);
	      std::cout << "The element in position " << idx << " in the column " << keyy << " is " << s << "." << std::endl;
	    }

	    if (std::holds_alternative<double>(element)) {
	      double d = std::get<double>(element);
	      std::cout << "The element in position " << idx << " in the column " << keyy << " is " << d << "." << std::endl;
	    }
	  }
	  catch (const NaNException &e) {std::cerr << "Error: the value is missing." << std::endl;}
	  catch (const std::out_of_range& e) {std::cerr << "Error: indices out of bounds." << std::endl;}
	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;


	  std::string checker = "";
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Would you like to create a file summarising all the data contained in the file? [y/n]" << std::endl;
	  std::cin >> checker;
	  if (checker == "y") {
	    std::string file_named = CA_Housing_Data.summary();
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "Would you like to print the content of the file you've just created? [y/n]" << std::endl;
	    std::cin >> checker;
	    if (checker == "y") {
	      std::string txt_file_name = "cat "+file_named;
	      system(txt_file_name.c_str());
	    }
	  }


  


	  unsigned int row_to_drop = 0;
	  CA_Housing_Data.drop_row(row_to_drop);
	  std::cout << "We have dropped a row. Now the number of rows is " << CA_Housing_Data.get_n_rows() << "." << std::endl;

	  CA_Housing_Data.drop_NaNs();
	  std::cout << "We have dropped all the rows containing missing values. Now the number of rows is " << CA_Housing_Data.get_n_rows() << "." << std::endl;


	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	}
	else if (bank_names.count(file_name)) {
	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Now we will test our iterators by printing the first 100 elements of each column." << std::endl;
	  for (std::string key : Banking_Data.get_column_keys()) {
	    unsigned int counter = 0;
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "The first 100 elements of the column: " << key << " is:\n" << std::endl;
	    std::cout << color << kernel_name << ">" << end_color;
	    for (auto it : Banking_Data) {
	      std::cout << it[key] << ", ";
	      if (counter > 100) {
		break;
	      }
	      counter ++;
	    }
	    std::cout << "\n\n" << std::endl;
	  }
	  

	  std::string checker = "";
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Would you like to create a file summarising all the data contained in the file? [y/n]" << std::endl;
	  std::cin >> checker;
	  if (checker == "y") {
	    std::string file_named = Banking_Data.summary();
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "Would you like to print the content of the file you've just created? [y/n]" << std::endl;
	    std::cin >> checker;
	    if (checker == "y") {
	      std::string txt_file_name = "cat "+file_named;
	      system(txt_file_name.c_str());
	    }
	  }

  


	  unsigned int row_to_drop = 0;
	  Banking_Data.drop_row(row_to_drop);
	  std::cout << "We have dropped a row. Now the number of rows is " << Banking_Data.get_n_rows() << "." << std::endl;

	  Banking_Data.drop_NaNs();
	  std::cout << "We have dropped all the rows containing missing values. Now the number of rows is " << Banking_Data.get_n_rows() << "." << std::endl;


	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	}
	else if (SUS_housing_names.count(file_name)) {
	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Now we will test our iterators by printing the first 100 elements of each column." << std::endl;
	  for (std::string key : SUS_Housing_Data.get_column_keys()) {
	    unsigned int counter = 0;
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "The first 100 elements of the column: " << key << " is:\n" << std::endl;
	    std::cout << color << kernel_name << ">" << end_color;
	    for (auto it : SUS_Housing_Data) {
	      std::cout << it[key] << ", ";
	      if (counter > 100) {
		break;
	      }
	      counter ++;
	    }
	    std::cout << "\n\n" << std::endl;
	  }

	  
	  std::string checker = "";
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Would you like to create a file summarising all the data contained in the file? [y/n]" << std::endl;
	  std::cin >> checker;
	  if (checker == "y") {
	    std::string file_named = SUS_Housing_Data.summary();
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "Would you like to print the content of the file you've just created? [y/n]" << std::endl;
	    std::cin >> checker;
	    if (checker == "y") {
	      std::string txt_file_name = "cat "+file_named;
	      system(txt_file_name.c_str());
	    }
	  }
	    


  


	  unsigned int row_to_drop = 0;
	  SUS_Housing_Data.drop_row(row_to_drop);
	  std::cout << "We have dropped a row. Now the number of rows is " << SUS_Housing_Data.get_n_rows() << "." << std::endl;

	  SUS_Housing_Data.drop_NaNs();
	  std::cout << "We have dropped all the rows containing missing values. Now the number of rows is " << SUS_Housing_Data.get_n_rows() << "." << std::endl;


	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	}
	else if (US_crime_names.count(file_name)) {
	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Now we will test our iterators by printing the first 100 elements of each column." << std::endl;
	  for (std::string key : Crime_Data.get_column_keys()) {
	    unsigned int counter = 0;
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "The first 100 elements of the column: " << key << " is:\n" << std::endl;
	    std::cout << color << kernel_name << ">" << end_color;
	    for (auto it : Crime_Data) {
	      std::cout << it[key] << ", ";
	      if (counter > 100) {
		break;
	      }
	      counter ++;
	    }
	    std::cout << "\n\n" << std::endl;
	  }


	 std::string checker = "";
	 std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Would you like to create a file summarising all the data contained in the file? [y/n]" << std::endl;
	  std::cin >> checker;
	  if (checker == "y") {
	    std::string file_named = Crime_Data.summary();
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "Would you like to print the content of the file you've just created? [y/n]" << std::endl;
	    std::cin >> checker;
	    if (checker == "y") {
	      std::string txt_file_name = "cat "+file_named;
	      system(txt_file_name.c_str());
	    }
	  }


	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	}
      }
    }
    std::cout << "\n \n \n" << std::endl;
    std::cout << "========================================================================================================================================================================" << std::endl;
    std::cout << "\n \n \n" << std::endl;
    kernel_name = "PROJECT";
    color = "\033[1;34m";
  }







  return 0;
}
#include "../Includes/Integration/Numerical_Integration.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/complex.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h> // we work with function wrappers to keep things as general as possible
#include <string> // for to_string, useful for documentation.



/* In order to successfully wrap the abstract class, we follow what was done in the example 11 in lection 13,
creating a class which inheriting the constructor and then using trampolines.
The class is templatized; however, it will be explicitly instantiated when binding the real and the complex
versions of the integrator. */



template<typename T>
class PyIntegration : public Integration<T> {
public:
  using Integration<T>::Integration;

  virtual T compute_integral() override {
    PYBIND11_OVERRIDE_PURE(
        T,
        Integration<T>,
        compute_integral()
    );
  }
};



namespace py = pybind11;

PYBIND11_MODULE(integration, m) {

    m.doc()="This module can be used to integrate real-valued real or complex functions. Integrators for real or complex functions are wrapped separately, so choose which to use depending on the situation. Integrators are labelled by [Valuetype]_[Method], e.g. 'Real_Midpoint'.";



    // Real case:



    py::class_<Integration<double>, PyIntegration<double>>(m, "Real_Base") /* Binding the abstract class
    (see the already cited example 11 for more details). */

        // We begin binding the constructor and the virtual method. 

        .def(py::init<const double&, const double&, const unsigned int&, std::function<double(double)>>(), py::arg("begin"), py::arg("end"), py::arg("subdivision_n"), py::arg("integrand"))
        
        .def("compute_integral", &Integration<double>::compute_integral, "Pure virtual method used to perform integration.")


        /* We use 'readonly' for all the attributes which are labelled as const in the C++ implementation. For
        modifiable attributes we use 'readwrite'. */
        
        .def_readonly("begin", &Integration<double>::begin, "The left endpoint 'a' of the interval [a,b] on which integration is being performed.")
        .def_readonly("end", &Integration<double>::end, "The right endpoint 'b' of the interval [a,b] on which integration is being performed.")
        .def_readonly("subdivision_n", &Integration<double>::subdivision_n, "The number of points which are used for the subdivision of the interval for composite integration.")

        .def_readwrite("integrand", &Integration<double>::integrand, "The integrand function.")
        .def_readonly("h", &Integration<double>::h, "The stepsize h = (b-a)/n used for composite integration.")
        .def_readonly("partition", &Integration<double>::partition, "The set of subintervals used for composite integration. On each one of them, the chosen non-composite integration rule will be used.")

        .def("__doc__", [](){return "This class serves as abstract base class for all the real integration methods. The attributes are begin (representing the left endpoint of the integration interval), end (right endpoint), subdivision_n (number of points for the subdivision for composite integration), h (stepsize), integrand, partition (the points delimiting subintervals on which simple integration is performed). The method is compute_integral().";})
        .def("__repr__", [](const Integration<double> &integrator) {return "<Real_Base> instance. Integrating on the interval ["+std::to_string(integrator.begin)+", "+std::to_string(integrator.end)+"], stepsize "+std::to_string(integrator.h)+" .";});



    py::class_<Midpoint<double>, Integration<double>>(m, "Real_Midpoint")

        .def(py::init<const double&, const double&, const unsigned int&, std::function<double(double)>>(), py::arg("begin"), py::arg("end"), py::arg("subdivision_n"), py::arg("integrand"))

        .def("compute_integral", &Midpoint<double>::compute_integral, "Function to perform integration of a real function by using the composite midpoit rule.")

        .def("__doc__", [](){return "This class performs integration of real-valued functions using the composite midpoint rule. The attributes are begin (representing the left endpoint of the integration interval), end (right endpoint), subdivision_n (number of points for the subdivision for composite integration), h (stepsize), integrand, partition (the points delimiting subintervals on which simple integration is performed). The method is compute_integral().";})
        .def("__repr__", [](const Midpoint<double> &integrator) {return "<Real_Midpoint> instance. Integrating on the interval ["+std::to_string(integrator.begin)+", "+std::to_string(integrator.end)+"], stepsize "+std::to_string(integrator.h)+".";});



    py::class_<Trapezoidal<double>, Integration<double>>(m, "Real_Trapezoidal")

        .def(py::init<const double&, const double&, const unsigned int&, std::function<double(double)>>(), py::arg("begin"), py::arg("end"), py::arg("subdivision_n"), py::arg("integrand"))
        
        .def("compute_integral", &Trapezoidal<double>::compute_integral, "This class performs integration by using the composite trapezoidal rule.")

        .def("__doc__", [](){return "This class performs integration of real-valued functions using the composite trapezoidal rule. The attributes are begin (representing the left endpoint of the integration interval), end (right endpoint), subdivision_n (number of points for the subdivision for composite integration), h (stepsize), integrand, partition (the points delimiting subintervals on which simple integration is performed). The method is compute_integral().";})
        .def("__repr__", [](const Trapezoidal<double> &integrator) {return "<Real_Trapezoidal> instance. Integrating on the interval ["+std::to_string(integrator.begin)+", "+std::to_string(integrator.end)+"], stepsize "+std::to_string(integrator.h)+".";});


    py::class_<Simpson<double>, Integration<double>>(m, "Real_Simpson")

        .def(py::init<const double&, const double&, const unsigned int&, std::function<double(double)>>(), py::arg("begin"), py::arg("end"), py::arg("subdivision_n"), py::arg("integrand"))
        
        .def("compute_integral", &Simpson<double>::compute_integral, "This class performs integration by using the composite Simpson rule.")

        .def("__doc__", [](){return "This class performs integration of real-valued functions using the composite Simpson rule. The attributes are begin (representing the left endpoint of the integration interval), end (right endpoint), subdivision_n (number of points for the subdivision for composite integration), h (stepsize), integrand, partition (the points delimiting subintervals on which simple integration is performed). The method is compute_integral().";})
        .def("__repr__", [](const Simpson<double> &integrator) {return "<Real_Simpson> instance. Integrating on the interval ["+std::to_string(integrator.begin)+", "+std::to_string(integrator.end)+"], stepsize "+std::to_string(integrator.h)+".";});


    py::class_<Gaussian<double>, Integration<double>>(m, "Real_Gaussian")

        .def(py::init<const double&, const double&, const unsigned int&, std::function<double(double)>, const unsigned int&, const std::string&, const double&, const double&>(), py::arg("begin"), py::arg("end"), py::arg("subdivision_n"), py::arg("integrand"), py::arg("number_of_nodes"), py::arg("family_of_polynomials")="Legendre", py::arg("alpha")=1, py::arg("beta")=1)
        
        .def("compute_integral", &Gaussian<double>::compute_integral, "This class performs integration by using a gaussian quadrature rule.")

        .def_readonly("family_of_polynomials", &Gaussian<double>::family_of_polynomials, "The family of polynomials against which we integrate and whose zeros will be the nodes used for polynomial interpolation.")
        .def_readonly("number_of_nodes", &Gaussian<double>::number_of_nodes, "Number of nodes to be used in the gaussian quadratue rule.")
        .def_readonly("alpha", &Gaussian<double>::alpha, "Parameter alpha to be used in Gegenbauer, Jacobi, exponential and Chebyshev type 2 integration.")
        .def_readonly("beta", &Gaussian<double>::beta, "Parameter beta to be used in Jacobi integration.")

        .def("__doc__", [](){return "This class performs integration of real-valued functions using the composite gaussian rule. The attributes are begin (representing the left endpoint of the integration interval), end (right endpoint), subdivision_n (number of points for the subdivision for composite integration), h (stepsize), integrand, partition (the points delimiting subintervals on which simple integration is performed). The method is compute_integral(), number_of_nodes (number of nodes used for interpolatorial quadrature rule), family_of_polynomials (weight function), alpha and beta (parameters).";})
        .def("__repr__", [](const Gaussian<double> &integrator) {return "<Real_Gaussian> instance. Integrating on the interval ["+std::to_string(integrator.begin)+", "+std::to_string(integrator.end)+"], stepsize "+std::to_string(integrator.h)+", the number of nodes is "+std::to_string(integrator.number_of_nodes)+", the family of polynomials is "+integrator.family_of_polynomials + ", alpha and beta are "+ std::to_string(integrator.alpha)+", "+std::to_string(integrator.beta)+".";});



    // Complex case:



    py::class_<Integration<std::complex<double>>, PyIntegration<std::complex<double>>>(m, "Complex_Base")

        .def(py::init<const double&, const double&, const unsigned int&, std::function<std::complex<double>(double)>>(), py::arg("begin"), py::arg("end"), py::arg("subdivision_n"), py::arg("integrand"))
        
        .def("compute_integral", &Integration<std::complex<double>>::compute_integral, "Pure virtual method to perform integration.")

        .def_readonly("begin", &Integration<std::complex<double>>::begin, "The left endpoint 'a' of the interval [a,b] on which integration is being performed.")
        .def_readonly("end", &Integration<std::complex<double>>::end, "The right endpoint 'b' of the interval [a,b] on which integration is being performed.")
        .def_readonly("subdivision_n", &Integration<std::complex<double>>::subdivision_n, "The number of points which are used for the subdivision of the interval for composite integration.")

        .def_readwrite("integrand", &Integration<std::complex<double>>::integrand, "The integrand function.")
        .def_readwrite("h", &Integration<std::complex<double>>::h, "The stepsize h = (b-a)/n used for composite integration.")
        .def_readwrite("partition", &Integration<std::complex<double>>::partition, "The set of subintervals used for composite integration. On each one of them, the chosen non-composite integration rule will be used.")

        .def("__doc__", [](){return "This class serves as abstract base class for all the complex integration methods. The attributes are begin (representing the left endpoint of the integration interval), end (right endpoint), subdivision_n (number of points for the subdivision for composite integration), h (stepsize), integrand, partition (the points delimiting subintervals on which simple integration is performed). The method is compute_integral().";})
        .def("__repr__", [](const Integration<std::complex<double>> &integrator) {return "<Complex_Base> instance. Integrating on the interval ["+std::to_string(integrator.begin)+", "+std::to_string(integrator.end)+"], stepsize "+std::to_string(integrator.h)+" .";});


    py::class_<Midpoint<std::complex<double>>, Integration<std::complex<double>>>(m, "Complex_Midpoint")
    
        .def(py::init<const double&, const double&, const unsigned int&, std::function<std::complex<double>(double)>>(), py::arg("begin"), py::arg("end"), py::arg("subdivision_n"), py::arg("integrand"))
        
        .def("compute_integral", &Midpoint<std::complex<double>>::compute_integral, "Function to perform integration of a real-valued complex function by using the composite midpoit rule.")

        .def("__doc__", [](){return "This class performs integration of real-valued functions using the composite midpoint rule. The attributes are begin (representing the left endpoint of the integration interval), end (right endpoint), subdivision_n (number of points for the subdivision for composite integration), h (stepsize), integrand, partition (the points delimiting subintervals on which simple integration is performed). The method is compute_integral().";})
        .def("__repr__", [](const Midpoint<std::complex<double>> &integrator) {return "<Complex_Midpoint> instance. Integrating on the interval ["+std::to_string(integrator.begin)+", "+std::to_string(integrator.end)+"], stepsize "+std::to_string(integrator.h)+".";});



    py::class_<Trapezoidal<std::complex<double>>, Integration<std::complex<double>>>(m, "Complex_Trapezoidal")

        .def(py::init<const double&, const double&, const unsigned int&, std::function<std::complex<double>(double)>>(), py::arg("begin"), py::arg("end"), py::arg("subdivision_n"), py::arg("integrand"))
        
        .def("compute_integral", &Trapezoidal<std::complex<double>>::compute_integral, "Function to perform integration of a real-valued complex function by using the composite trapezoidal rule.")

        .def("__doc__", [](){return "This class performs integration of real-valued functions using the composite trapezoidal rule. The attributes are begin (representing the left endpoint of the integration interval), end (right endpoint), subdivision_n (number of points for the subdivision for composite integration), h (stepsize), integrand, partition (the points delimiting subintervals on which simple integration is performed). The method is compute_integral().";})
        .def("__repr__", [](const Midpoint<std::complex<double>> &integrator) {return "<Complex_Trapezoidal> instance. Integrating on the interval ["+std::to_string(integrator.begin)+", "+std::to_string(integrator.end)+"], stepsize "+std::to_string(integrator.h)+".";});


    py::class_<Simpson<std::complex<double>>, Integration<std::complex<double>>>(m, "Complex_Simpson")

        .def(py::init<const double&, const double&, const unsigned int&, std::function<std::complex<double>(double)>>(), py::arg("begin"), py::arg("end"), py::arg("subdivision_n"), py::arg("integrand"))
        
        .def("compute_integral", &Simpson<std::complex<double>>::compute_integral, "Function to perform integration of a real-valued complex function by using the composite Simpson rule.")

        .def("__doc__", [](){return "This class performs integration of real-valued functions using the composite Simpson rule. The attributes are begin (representing the left endpoint of the integration interval), end (right endpoint), subdivision_n (number of points for the subdivision for composite integration), h (stepsize), integrand, partition (the points delimiting subintervals on which simple integration is performed). The method is compute_integral().";})
        .def("__repr__", [](const Simpson<std::complex<double>> &integrator) {return "<Complex_Simpson> instance. Integrating on the interval ["+std::to_string(integrator.begin)+", "+std::to_string(integrator.end)+"], stepsize "+std::to_string(integrator.h)+".";});



    py::class_<Gaussian<std::complex<double>>, Integration<std::complex<double>>>(m, "Complex_Gaussian")

        .def(py::init<const double&, const double&, const unsigned int&, std::function<std::complex<double>(double)>, const unsigned int&, const std::string&, const double&, const double&>(), py::arg("begin"), py::arg("end"), py::arg("subdivision_n"), py::arg("integrand"), py::arg("number_of_nodes"), py::arg("family_of_polynomials")="Legendre", py::arg("alpha")=1, py::arg("beta")=1)
        
        .def("compute_integral", &Gaussian<std::complex<double>>::compute_integral, "Function to perform integration of a real-valued complex function by using a gaussian quadrature rule.")

        .def_readonly("family_of_polynomials", &Gaussian<std::complex<double>>::family_of_polynomials, "The family of polynomials against which we integrate and whose zeros will be the nodes used for polynomial interpolation. Default value = 'Legendre'.")
        .def_readonly("number_of_nodes", &Gaussian<std::complex<double>>::number_of_nodes, "Number of nodes to be used in the gaussian quadrature rule.")
        .def_readonly("alpha", &Gaussian<std::complex<double>>::alpha, "Parameter alpha to be used in Gegenbauer, Jacobi, exponential and Chebyshev type 2 integration. Default value = 1.")
        .def_readonly("beta", &Gaussian<std::complex<double>>::beta, "Parameter beta to be used in Jacobi integration. Default value = 1.")

        .def("__doc__", [](){return "This class performs integration of real-valued functions using the composite gaussian rule. The attributes are begin (representing the left endpoint of the integration interval), end (right endpoint), subdivision_n (number of points for the subdivision for composite integration), h (stepsize), integrand, partition (the points delimiting subintervals on which simple integration is performed). The method is compute_integral(), number_of_nodes (number of nodes used for interpolatorial quadrature rule), family_of_polynomials (weight function), alpha and beta (parameters).";})
        .def("__repr__", [](const Gaussian<std::complex<double>> &integrator) {return "<Complex_Gaussian> instance. Integrating on the interval ["+std::to_string(integrator.begin)+", "+std::to_string(integrator.end)+"], stepsize "+std::to_string(integrator.h)+", the number of nodes is "+std::to_string(integrator.number_of_nodes)+", the family of polynomials is "+integrator.family_of_polynomials + ", alpha and beta are "+ std::to_string(integrator.alpha)+", "+std::to_string(integrator.beta)+".";});



    py::register_exception<std::runtime_error>(m, "RuntimeError"); // if the string 'family_of_polynomials' is invalid

}
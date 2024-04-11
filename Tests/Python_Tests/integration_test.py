#usr/bin/env python3

import sys
sys.path.append('../../Python_Code/')
sys.path.append('../../build/')

import numpy as np
import integration_py.py_integration as pitg
import integration as itg # the module 'integration' compiled using C++ was saved as 'itg' in the
# Python integration_py in order to allow greater readability

from math import *
from scipy.integrate import quad
import time



# We have defined everything in the main function in order to make profiling easier (see integration_profile.py)

def main():


    print("\n-----------------------------\n")

    print('TESTING THE C++ BINDINGS:')

    pitg.main() # Some tests regarding the C++ part of the code. Please check the folder python_Code/integration_py
    # for more details.



    # We begin instatiating some objects and checking that everything works as expected.



    one = lambda x : 1
    linear = lambda x : x
    square = lambda x : x**2
    cube = lambda x : x**3
    fourth = lambda x : x**4

    some_ugly_function = lambda x : np.sin(x)*x**4 - 4*x +np.log(x+1)



    # computing some integrals and testing the estim_orders() method on real and complex integrators of all the three derived classes:
    print('Now we will compute some interals and perform some estimations of the polynomial order.')


    n_nodes = 3 # the polynomial order of the corresponding Gaussian quadrature rule should be 2n -1

    PCM = pitg.ComplexMidpoint(begin = -1, end = 1, subdivision_n = 13, integrand = square)
    PRT = pitg.RealTrapezoidal(0,1,8,cos)
    PRS = pitg.RealSimpson(0,1,3,some_ugly_function)
    PCG = pitg.ComplexGaussian(begin = -1, end = 8, subdivision_n = 3, integrand = some_ugly_function, number_of_nodes = n_nodes)


    print(f'The integral of the function f(x) = x^2 on [-1, 1] is {PCM.compute_integral()}. The value provided by scipy is {quad(square,-1,1)[0]} ')

    print("\n-----------------------------\n")

    print(f"The integral of the function f(x) = cos(x) on [0, 1] is {PRT.compute_integral()}. The value provided by scipy is {quad(cos,0,1)[0]}.")

    print("\n-----------------------------\n")

    print(f"The integral of the function f(x) = sin(x)*x^4 -a4 +log(x+1) on [0, 1] is {PRS.compute_integral()}. The value provided by scipy is {quad(some_ugly_function,0,1)[0]}.")

    print("\n-----------------------------\n")

    print(f"The integral of the function f(x) = sin(x)*x^4 -a4 +log(x+1) on [0, 5] is {PCG.compute_integral()}. The value provided by scipy is {quad(some_ugly_function,0,5)[0]}.")




    print("\n-----------------------------\n")




    assert(PCM.estim_orders() == (1, 2))

    print("\n-----------------------------\n")

    assert(PRT.estim_orders() == (1, 2))

    print("\n-----------------------------\n")

    assert(PRS.estim_orders() == (3, 4))

    print("\n-----------------------------\n")

    assert(PCG.estim_orders() == (5, 6))

    print("\n-----------------------------\n")



    # Testing the setter method for h
    print('Now we will test the setter method for h.')



    print(f"Now the stepsize h of the integrator PCS is equal to {PCM.h}.")

    PCM.h = 0.001

    print(f"Now the stepsize h of the integrator PCS is equal to {PCM.h}.")



    # Testing the __repr__ method.
    print('Testing the __repr__ method:')



    print("The object PCM is printed as follows: ", PCM)

    print("\n-----------------------------\n")



    # Printing the attributes



    print('Now we will print the attributes of an integrator and the ones of its C++ backend to ensure they coincide.')

    PRG = pitg.RealGaussian(begin = -5, end = 6, subdivision_n = 5, integrand = square, number_of_nodes = 8, family_of_polynomials = 'Gegenbauer', alpha = 4, beta = 0)

    print("\n-----------------------------\n")

    print('PRG is:', PRG)
    print('PRG.cpp_backend is:', PRG.cpp_backend)

    print("\n-----------------------------\n")

    print(f'PRG.begin is {PRG.begin}.')
    print(f'PRG.cpp_backend.begin is {PRG.cpp_backend.begin}.')

    print("\n-----------------------------\n")

    print(f'PRG.end is {PRG.end}.')
    print(f'PRG.cpp_backend.end is {PRG.cpp_backend.end}.')

    print("\n-----------------------------\n")


    print(f'PRG.subdivision_n is {PRG.subdivision_n}.')
    print(f'PRG.cpp_backend.subdivision_n is {PRG.cpp_backend.subdivision_n}.')

    print("\n-----------------------------\n")

    # print(f'PRG.integrand is {PRG.integrand}.') # this is not really readable
    # print(f'PRG.cpp_backend.integrand is {PRG.cpp_backend.integrand}.') # not readable

    # print("\n-----------------------------\n")

    print(f'PRG.number_of_nodes is {PRG.number_of_nodes}.')
    print(f'PRG.cpp_backend.number_of_nodes is {PRG.cpp_backend.number_of_nodes}.')

    print("\n-----------------------------\n")

    print(f'PRG.family_of_polynomials is {PRG.family_of_polynomials}.')
    print(f'PRG.cpp_backend.family_of_polynomials is {PRG.cpp_backend.family_of_polynomials}.')

    print("\n-----------------------------\n")

    print(f'PRG.alpha is {PRG.alpha}.')
    print(f'PRG.cpp_backend.alpha is {PRG.cpp_backend.alpha}.')

    print("\n-----------------------------\n")

    print(f'PRG.beta is {PRG.beta}.')
    print(f'PRG.cpp_backend.beta is {PRG.cpp_backend.beta}.')

    print("\n-----------------------------\n")

    print(f'The integral of PRG is {PRG.compute_integral()}')
    print(f'The integral of its C++ backend is {PRG.cpp_backend.compute_integral()}')

    print("\n-----------------------------\n")



    # Nowe plot the error trend agains the number of subintervals used for composite integration.



    print('Now we plot the error trend of PCG.')
    pitg.plot_error(PCM)

    print('Now we plot the error trend of PCG.')
    pitg.plot_error(PRT)

    print('Now we plot the error trend of PCG.')
    pitg.plot_error(PRS)

    print('Now we plot the error trend of PCG.')
    pitg.plot_error(PCG)



    print("\n-----------------------------\n")



    # BENCHMARKING:




    print('Benchmarking: this might take a minute.')

    py_Simpson = pitg.py_simpson(0, 1, 30, cos)
    cpp_Simpson = itg.Real_Simpson(0, 1, 30, cos)

    result1 = py_Simpson.compute_integral()
    result2 = cpp_Simpson.compute_integral()

    assert(abs(result1-result2)<1e-7)



    # Time for the comparison!



    print("\n-----------------------------\n")


    pitg.compare_eff_simps_py(begin = 0, end = 1, subdivision_n = 50000, integrand = some_ugly_function, iterations = 10)

    print("\n-----------------------------\n")

    pitg.compare_eff_scipy(begin = 0, end = 1, subdivision_n = 20, integrand = some_ugly_function, iterations = 10)

    print("\n-----------------------------\n")





    return






if __name__ == "__main__":

    main()

print("Thank you for testing the integration module.")
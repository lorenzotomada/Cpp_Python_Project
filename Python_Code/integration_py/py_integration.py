#!/usr/env python3

import sys
sys.path.append('../../build/.')
import time
from abc import ABC, abstractmethod
import integration as itg
import numpy as np
from scipy.integrate import quad
import matplotlib.pyplot as plt





# For binding the classes, we sticked to the C++ inheritance structure (clearly with two different sets of 
# classes, for real and complex integrators, due to the requirements of PyBind11). The only naming difference
# between our Python and C++ classes is that the Python ones do not have the underscore in their name in order
# to be able to distunguish them, i.e. Real_Simpson becomes RealSimpson.


# In order for our classes to resemble the C++ ones, when we decided to modify something (unless it was, say,
# adding a setter method) we did it outside of the class, using decorators.
# In this way, creating new methods for the classes or using a timer for benchmarking did not require to
# write actual code inside the class, hence keeping everthing more readable amd closer to the C++ source code.




# We define a decorator, which will be applied to the method "compute_integral" in order to make benchmarking
# easier to perform.



def timer(my_function):

    """
    Decorator wrapping a function and measuring the time needed for its execution.
    
    Parameters:
    - my_function: function
        The function we want to benchmark.

    Returns:
    - float
        Time needed for the execution of the function.
    """

    def wrapper(*args, **kwargs):

        t1 = time.time()
        result = my_function(*args, **kwargs)
        t2 = time.time()

        print(f"{my_function.__name__} ran in {t2 - t1:.7f} sec")

        return result
    
    return wrapper






# Now we define a decorator adding a method to a class. The method will be called __estim_pol_order and it will
# be hidden (marked by __ at the beginning of the name) since it is just an auxiliary method for the method
# estim_orders (see below).






def add_estim_pol_order(cls):

    """
    Decorator wrapping an integration class and adding a method to compute its polynomial order.
    The method is added as a hidden method, called __estim_pol_order()
    
    Parameters:
    - cls: RealBase or ComplexBase or derived classes
        The class to which we want to add a method.

    Returns:
    - class: the same class given as a parameter
        Enhanced with the new functionality.
    """

    def __estim_pol_order(self):

        """
        Method to compute the polynomial order of a class.
        The method computes the integral of x^i, starting with i = 0 and proceding until the result is not correct.
        In that case, it means that we have reached the order.
    
        Parameters:
        - no parameters

        Returns:
        - int:
            The estimation of the polynomial order.
        """

        i=0 # counter used to keep trace of the integral computed correctly

        # Now we save the old attributes of the class, as they will be modified during the execution of the 
        # function. In order to avoid having side effects, we save them and we re-set them once computations
        # have been done.

        old_subdivision_n = self.subdivision_n
        old_integrand = self.integrand

        self.subdivision_n=1 # Simple version, not the composite one. Otherwise we could overestimate the order,
        # due to the higher effectiveness of the composite version.

        def f(x,k=i):
            return x**k    # We need to test for  which degrees of x^i the integration method is exact
        
        self.integrand=f

        result=self.cpp_backend.compute_integral()
        exact_result = (self.end**(i+1)-self.begin**(i+1))/(i+1)

        while abs(result-exact_result)<1e-8:   # check if they coincide, apart from maybe minimal errors due to
            # floating point arithmetics.
             
            i+=1   # If they do, we increase our estimation of the order.

            def f(x,k=i):       # higher degree now
                return x**k
            
            self.integrand=f

            result=self.cpp_backend.compute_integral()      # We call the backend version of compute integral
            # because the one of python classes will be decorated using timer and it would have printed the time
            # each time the method is called, which is not useful in this context. 

            exact_result = (self.end**(i+1)-self.begin**(i+1))/(i+1)

        self.subdivision_n = old_subdivision_n    # To avoid side effects.
        self.integrand = old_integrand

        return i-1  # we return i-1 and not i because i is incremented before actually doing the check: if i is
            # equal to the polynomial, then we increment it and we find an incorrect result, meaning that the 
            # result was correct before incrementing it  

    cls.__estim_pol_order=__estim_pol_order     # add the method to the class and return the class.

    return cls
    





# The following wrapper takes a class end enhances it with a method which returns both the plynomial order and
# the convergence order. 






def add_estim_orders(cls):

    """
    Decorator wrapping an integration class and adding a method to compute its polynomial and its convergence order.
    The method is added with the name 'estim_orders'.
    
    Parameters:
    - class: RealBase or ComplexBase or derived classes
        The class to which we want to add a method.

    Returns:
    - class: the same class given as a parameter
        Enhanced with the new functionality.
    """

    def estim_orders(self, plots=False):

        """
        Method to compute the polynomial and convergence order of a class.
        The polynomial order is computed using __estim_pol_order
        The convergence order is computed taking a function for which the computed integral is not exact and then
        reducing the stepsize, to see to which power the error decreases.
    
        Parameters:
        -plot: bool, optional
            Default value: False
            If plot is True, then the method also plots a line corresponding to the plot of the error after and 
            before reducing the stepsize, in logarithmic scale. The order of convergence can be estimated using
            the steepness of that straight line, so it can be interesting if you want to check yourself that
            things parse.
            

        Returns:
        - tuple:
            (int, int) couple. The first element is the estimation of the polynomial order, the second one the
            estimation of the order of convergence.

        """

        old_subdivision_n = self.subdivision_n   # to avoid side effects.
        old_integrand = self.integrand
        old_begin = self.begin
        old_end = self.end
        self.begin = 0 # not necessary actually, but to make it more uniform
        self.end = 1


        i = self.__estim_pol_order()
        i+=1     # we estimate the polynomial order and we take a power of x with a higher exponent.
        # In this way, the formula will not be exact, hence it is sensible to reduce the stepsize to see
        # the power according to which the error decreases.

        def f(x,j=i):
            return x**j
        
        self.integrand=f

        exact_result = ((self.end-self.begin)**(i+1))/(i+1)
        errors=[]

        for i in [1,10]: # compute first for a stepsize h equal to end-begin and then for h_new = (1/10)*h_old.
            # Hence, if the method has order p, the new error should be approximately equal to ((1/10)^p)*old_error
             
            self.subdivision_n=i

            result=self.cpp_backend.compute_integral()
            errors.append(abs(result-exact_result))
        
        log_errors=[np.log10(errors[i]) for i in [0,-1]]   #  in logplots, the order is the angular coefficient
        # of the straight line joining the two points. 

        if plots:
            import matplotlib.pyplot as plt    # lazy way of importing

            plt.plot([1,10], log_errors, label='Absolute value of the error') # for some reason, this gives some warnings. However, it does
            # not change the correctness of the plot or of the result.

            plt.title('Error trend')
            plt.xlabel('Number of nodes')
            plt.ylabel('log10(error)')
            plt.legend()
            plt.show()

        conv_order = round(abs(np.log10(errors[-1])-np.log10(errors[0])))    # rounded to have an integer
        polyn_order = self.__estim_pol_order()

        print(f'The method has polynomial order equal to {polyn_order}.')

        if conv_order == polyn_order+1:
            print(f'The method has convergence order equal to {conv_order}. This is be correct, since it is equal to 1 + the polynomial order.')
        else:
            print(f'The method has convergence order equal to {conv_order}. It should have order {polyn_order+1}.')

        self.subdivision_n = old_subdivision_n
        self.integrand = old_integrand
        self.begin = old_begin
        self.end = old_end

        return polyn_order, conv_order

    cls.estim_orders=estim_orders
    return cls






# The following class inherits from ABC to make it an abstract class. We decorate it with the two decorators
# for the estimation of the orders.






@add_estim_pol_order
@add_estim_orders
class RealBase(ABC):

    """
    Base, abstract class to perform composite numerical integration of real-valued functions.

    Parameters:
    - begin: float
        Left endpoint of the integration interval
    -end: float
        Right endpoint of the integration interval
    -subdivision_n: int
        Number of points used for subdivision of the interval in order to perform composite integration.
    -integrand: function
        The real-valued function we want to integrate.
    """


    def __init__(self, begin, end, subdivision_n, integrand):
        self.begin=begin
        self.end=end
        self.subdivision_n=subdivision_n
        self.integrand=integrand



    # The cpp backend of the method is defined as property. In this way, the user is allowed to  modify any
    # parameter with great flexibility and the corresponding C++ object will be created and saved automatically.
    # The possibility to do that allows us to define the method to compute the order.



    @property
    def cpp_base_backend(self):

        """
        itg.Real_Base object
            This is the C++ backend of the base integrator. It is automatically created once an object is instantiated
            and gets uploaded each time one of the other attributes is modified.
        """

        return itg.Real_Base(self.begin, self.end, self.subdivision_n, self.integrand)
    


    # As before, we define the partition as a property in order to be updated if the user makes any changes
    # to the object.



    @property
    def partition(self):

        """
        list:
            This is the partition of the interval used for composite integration. It is automatically created once
        object has been instantiated. It is modified each time one of the other attributes is modified. 
        """

        return self.cpp_base_backend.partition



    # Same for the stepsize.



    @property
    def h(self):

        """
        float:
            The stepsize used for composite integration. It is automatically created once the object is instantiated.
        It is modified each time one of the other parameters is upgraded.
        """

        return self.cpp_base_backend.h
    


    # For the stepsize, we also provide a setter method, to enable users to change it with ease.



    @h.setter
    def h(self, new_h):

        """
        Setter method for the stepsize.
        It upgrades the stepsize by changing the value of subdivision_n.

        Parameters:
        - float:
            The new stepsize to be used in complex integration.

        Returns:
        - nothing
        """

        self.subdivision_n = int((self.end-self.begin)/new_h)   # we adjust the number of subdivisions accordingly        
    
    

    # The following method is the abstract method to perform integration.
    

    
    @abstractmethod
    def compute_integral(self):

        """
        Compute the integral according to the attributes of the class.
        This is an abstract method, hence it is supposed to be overridden and no implementation is given.

        Parameters:
        - no parameters

        Returns:
        - it cannot be called, being an abstract methods, hence it cannot return anything
        """

        pass      # it is an abstract method. We provide no definition


    # Just to print some nicer things:



    def __repr__(self):
        return "py_integration.<RealBase> object. Call 'help' for further details."







class RealMidpoint(RealBase):

    """
    Class to perform composite numerical integration of real-valued functions through the composite midpoint rule.
    It inherits from RealBase. The constructor is the same of the base class.

    Parameters:
    - begin: float
        Left endpoint of the integration interval
    -end: float
        Right endpoint of the integration interval
    -subdivision_n: int
        Number of points used for subdivision of the interval in order to perform composite integration.
    -integrand: function
        The real-valued function we want to integrate.
    """


    def __init__(self, begin, end, subdivision_n, integrand):
        RealBase.__init__(self, begin, end, subdivision_n, integrand)


    # We define the C++ backend, which will be used for computations.



    @property
    def cpp_backend(self):

        """
        itg.Real_Midpoint object:
            This is the C++ backend of the real midpoint integrator. It is automatically created once an object
            is instantiated and gets uploaded each time one of the other attributes is modified.
            
        """

        return itg.Real_Midpoint(self.begin, self.end, self.subdivision_n, self.integrand)
    


    # We decorate compute_integral using timer, making it easier to benchmark the method later on.



    @timer  
    def compute_integral(self):

        """
        Compute the integral according to the attributes of the class using the composite midpoint rule.
        It is implemented in C++.

        Parameters:
        - no parameters

        Returns:
        - float
            The result of the integration.
        """

        return self.cpp_backend.compute_integral()
    


    def __repr__(self):
        return "py_integration.<RealMidpoint> object. Call 'help' for further details."






class RealTrapezoidal(RealBase):

    """
    Class to perform composite numerical integration of real-valued functions through the composite trapezoidal rule.
    It inherits from RealBase. The constructor is the same of the base class.

    Parameters:
    - begin: float
        Left endpoint of the integration interval
    -end: float
        Right endpoint of the integration interval
    -subdivision_n: int
        Number of points used for subdivision of the interval in order to perform composite integration.
    -integrand: function
        The real-valued function we want to integrate.
    """


    def __init__(self, begin, end, subdivision_n, integrand):
        RealBase.__init__(self, begin, end, subdivision_n, integrand)



    @property
    def cpp_backend(self):

        """
        itg.Real_Trapezoidal object:
            This is the C++ backend of the real trapezoidal integrator. It is automatically created once an object
            is instantiated and gets uploaded each time one of the other attributes is modified.
            
        """

        return itg.Real_Trapezoidal(self.begin, self.end, self.subdivision_n, self.integrand)
    

    
    @timer
    def compute_integral(self):

        """
        Compute the integral according to the attributes of the class using the composite trapezoidal rule.
        It is implemented in C++.

        Parameters:
        - no parameters

        Returns:
        - float
            The result of the integration.
        """

        return self.cpp_backend.compute_integral()



    def __repr__(self):
        return "py_integration.<RealTrapezoidal> object. Call 'help' for further details."






class RealSimpson(RealBase):

    """
    Class to perform composite numerical integration of real-valued functions through the composite Simpson rule.
    It inherits from RealBase. The constructor is the same of the base class.

    Parameters:
    - begin: float
        Left endpoint of the integration interval
    -end: float
        Right endpoint of the integration interval
    -subdivision_n: int
        Number of points used for subdivision of the interval in order to perform composite integration.
    -integrand: function
        The real-valued function we want to integrate.
    """


    def __init__(self, begin, end, subdivision_n, integrand):
        RealBase.__init__(self, begin, end, subdivision_n, integrand)



    @property
    def cpp_backend(self):

        """
        itg.Real_Simpson object:
            This is the C++ backend of the real Simpson integrator. It is automatically created once an object
            is instantiated and gets uploaded each time one of the other attributes is modified.
            
        """

        return itg.Real_Simpson(self.begin, self.end, self.subdivision_n, self.integrand)
    


    @timer
    def compute_integral(self):

        """
        Compute the integral according to the attributes of the class using the composite Simpson rule.
        It is implemented in C++.

        Parameters:
        - no parameters

        Returns:
        - float
            The result of the integration.
        """

        return self.cpp_backend.compute_integral()
    


    def __repr__(self):
        return "py_integration.<RealSimpson> object. Call 'help' for further details."






class RealGaussian(RealBase):

    """
    Class to perform composite numerical integration of real-valued functions through the Gaussian quadrature rule.
    It inherits from RealBase. The constructor also adds some new attributes.

    Parameters:
    - begin: float
        Left endpoint of the integration interval
    -end: float
        Right endpoint of the integration interval
    -subdivision_n: int
        Number of points used for subdivision of the interval in order to perform composite integration.
    -integrand: function
        The real-valued function we want to integrate.
    -number_of_nodes: int
        The number of nodes used for the interpolatorial quadrature rule.
    -family_of_polynomials: string, optional
        Weight function against which integration is performed.
        Default value = 'Legendre'
    -alpha: float, optional
        Value used e.g. in Gegenbauer integration.
        Default value = 1
    -beta: float, optional
        Value used, e.g., in exponential integration.
        Default value = 1
    """


    def __init__(self, begin, end, subdivision_n, integrand, number_of_nodes, family_of_polynomials='Legendre', alpha=1, beta=1):
        RealBase.__init__(self, begin, end , subdivision_n, integrand)
        self.number_of_nodes=number_of_nodes
        self.family_of_polynomials=family_of_polynomials
        self.alpha=alpha
        self.beta=beta



    @property
    def cpp_backend(self):

        """
        itg.Real_Gaussian object:
            This is the C++ backend of the real Gaussian integrator. It is automatically created once an object
            is instantiated and gets uploaded each time one of the other attributes is modified.
            
        """

        return itg.Real_Gaussian(self.begin, self.end, self.subdivision_n, self.integrand, self.number_of_nodes, self.family_of_polynomials, self.alpha, self.beta)



    @timer
    def compute_integral(self):

        """
        Compute the integral according to the attributes of the class using the chosen Gaussian quadrature formula.
        It is implemented in C++.

        Parameters:
        - no parameters

        Returns:
        - float
            The result of the integration.
        """

        return self.cpp_backend.compute_integral()
    


    def __repr__(self):
        return "py_integration.<RealGaussian> object. Call 'help' for further details."






@add_estim_pol_order
@add_estim_orders
class ComplexBase(ABC):

    """
    Base, abstract class to perform composite numerical integration of complex-valued functions.

    Parameters:
    - begin: float
        Left endpoint of the integration interval
    -end: float
        Right endpoint of the integration interval
    -subdivision_n: int
        Number of points used for subdivision of the interval in order to perform composite integration.
    -integrand: function
        The real-valued function we want to integrate.
    """


    def __init__(self, begin, end, subdivision_n, integrand):
        self.begin=begin
        self.end=end
        self.subdivision_n=subdivision_n
        self.integrand=integrand



    @property
    def cpp_base_backend(self):

        """
        itg.Complex_Base object
            This is the C++ backend of the base integrator. It is automatically created once an object is instantiated
            and gets uploaded each time one of the other attributes is modified.
        """

        return itg.Complex_Base(self.begin, self.end, self.subdivision_n, self.integrand)
    


    @property
    def partition(self):

        """
        list:
            This is the partition of the interval used for composite integration. It is automatically created once
        object has been instantiated. It is modified each time one of the other attributes is modified. 
        """

        return self.cpp_base_backend.partition



    @property
    def h(self):

        """
        float:
            The stepsize used for composite integration. It is automatically created once the object is instantiated.
        It is modified each time one of the other parameters is upgraded.
        """

        return self.cpp_base_backend.h
        


    @h.setter
    def h(self, new_h):

        """
        Setter method for the stepsize.
        It upgrades the stepsize by changing the value of subdivision_n.

        Parameters:
        - float:
            The new stepsize to be used in complex integration.

        Returns:
        - nothing
        """

        self.subdivision_n = int((self.end-self.begin)/new_h)



    @abstractmethod
    def compute_integral(self):

        """
        Compute the integral according to the attributes of the class.
        This is an abstract method, hence it is supposed to be overridden and no implementation is given.

        Parameters:
        - no parameters

        Returns:
        - it cannot be called, being an abstract methods, hence it cannot return anything
        """

        pass



    def __repr__(self):
        return "py_integration.<ComplexBase> object. Call 'help' for further details."






class ComplexMidpoint(ComplexBase):
    """
    Class to perform composite numerical integration of complex-valued functions through the composite midpoint rule.
    It inherits from ComplexBase. The constructor is the same of the base class.

    Parameters:
    - begin: float
        Left endpoint of the integration interval
    -end: float
        Right endpoint of the integration interval
    -subdivision_n: int
        Number of points used for subdivision of the interval in order to perform composite integration.
    -integrand: function
        The real-valued function we want to integrate.
    """



    def __init__(self, begin, end, subdivision_n, integrand):
        ComplexBase.__init__(self, begin, end, subdivision_n, integrand)



    @property
    def cpp_backend(self):

        """
        itg.Complex_Midpoind object:
            This is the C++ backend of the complex gaussian integrator. It is automatically created once an object
            is instantiated and gets uploaded each time one of the other attributes is modified.
            
        """

        return itg.Complex_Midpoint(self.begin, self.end, self.subdivision_n, self.integrand)
    


    @timer
    def compute_integral(self):
   
        """
        Compute the integral according to the attributes of the class using the composite midpoint rule.
        It is implemented in C++.

        Parameters:
        - no parameters

        Returns:
        - complex
            The result of the integration.
        """

        return self.cpp_backend.compute_integral()
    


    def __repr__(self):
        return "py_integration.<ComplexMidpoint> object. Call 'help' for further details."






class ComplexTrapezoidal(ComplexBase):

    """
    Class to perform composite numerical integration of complex-valued functions through the composite trapezoidal rule.
    It inherits from ComplexBase. The constructor is the same of the base class.

    Parameters:
    - begin: float
        Left endpoint of the integration interval
    -end: float
        Right endpoint of the integration interval
    -subdivision_n: int
        Number of points used for subdivision of the interval in order to perform composite integration.
    -integrand: function
        The real-valued function we want to integrate.
    """

    def __init__(self, begin, end, subdivision_n, integrand):
        ComplexBase.__init__(self, begin, end, subdivision_n, integrand)



    @property
    def cpp_backend(self):

        """
        itg.Complex_Trapezoidal object:
            This is the C++ backend of the complex trapezoidal integrator. It is automatically created once an object
            is instantiated and gets uploaded each time one of the other attributes is modified.
            
        """

        return itg.Complex_Trapezoidal(self.begin, self.end, self.subdivision_n, self.integrand)
    


    @timer
    def compute_integral(self):

        """
        Compute the integral according to the attributes of the class using the composite trapezoidal rule.
        It is implemented in C++.

        Parameters:
        - no parameters

        Returns:
        - complex
            The result of the integration.
        """

        return self.cpp_backend.compute_integral()
    


    def __repr__(self):
        return "py_integration.<ComplexTrapezoidal> object. Call 'help' for further details."
    





class ComplexSimpson(ComplexBase):

    """
    Class to perform composite numerical integration of real-valued functions through the composite Simpson rule.
    It inherits from RealBase. The constructor is the same of the base class.

    Parameters:
    - begin: float
        Left endpoint of the integration interval
    -end: float
        Right endpoint of the integration interval
    -subdivision_n: int
        Number of points used for subdivision of the interval in order to perform composite integration.
    -integrand: function
        The real-valued function we want to integrate.
    """


    def __init__(self, begin, end, subdivision_n, integrand):
        ComplexBase.__init__(self, begin, end, subdivision_n, integrand)



    @property
    def cpp_backend(self):

        """
        itg.Complex_Simpson object:
            This is the C++ backend of the complex Simpson integrator. It is automatically created once an object
            is instantiated and gets uploaded each time one of the other attributes is modified.
            
        """

        return itg.Complex_Simpson(self.begin, self.end, self.subdivision_n, self.integrand)
    


    @timer
    def compute_integral(self):

        """
        Compute the integral according to the attributes of the class using the composite Simpson rule.
        It is implemented in C++.

        Parameters:
        - no parameters

        Returns:
        - complex
            The result of the integration.
        """
        return self.cpp_backend.compute_integral()
    


    def __repr__(self):
        return "py_integration.<ComplexSimpson> object. Call 'help' for further details."
    





class ComplexGaussian(ComplexBase):

    """
    Class to perform composite numerical integration of complex-valued functions through the Gaussian quadrature rule.
    It inherits from ComplexBase. The constructor also adds some new attributes.

    Parameters:
    - begin: float
        Left endpoint of the integration interval
    -end: float
        Right endpoint of the integration interval
    -subdivision_n: int
        Number of points used for subdivision of the interval in order to perform composite integration.
    -integrand: function
        The real-valued function we want to integrate.
    -number_of_nodes: int
        The number of nodes used for the interpolatorial quadrature rule.
    -family_of_polynomials: string, optional
        Weight function against which integration is performed.
        Default value = 'Legendre'
    -alpha: float, optional
        Value used e.g. in Gegenbauer integration.
        Default value = 1
    -beta: float, optional
        Value used, e.g., in exponential integration.
        Default value = 1
    """


    def __init__(self, begin, end, subdivision_n, integrand, number_of_nodes, family_of_polynomials='Legendre', alpha=1, beta=1):
        RealBase.__init__(self, begin, end , subdivision_n, integrand)
        self.number_of_nodes=number_of_nodes
        self.family_of_polynomials=family_of_polynomials
        self.alpha=alpha
        self.beta=beta



    @property
    def cpp_backend(self):

        """
        itg.Complex_Gaussian object:
            This is the C++ backend of the complex Gaussian integrator. It is automatically created once an object
            is instantiated and gets uploaded each time one of the other attributes is modified.
            
        """

        return itg.Complex_Gaussian(self.begin, self.end, self.subdivision_n, self.integrand, self.number_of_nodes, self.family_of_polynomials, self.alpha, self.beta)



    @timer
    def compute_integral(self):
          
        """
        Compute the integral according to the attributes of the class using the chosen Gaussian quadrature formula.
        It is implemented in C++.

        Parameters:
        - no parameters

        Returns:
        - complex
            The result of the integration.
        """

        return self.cpp_backend.compute_integral()
    


    def __repr__(self):
        return "py_integration.<ComplexGaussian> object. Call 'help' for further details."
    







# We decided to implement in Python one of the integration classes in order to test the efficienty gain.
# To this aim, we used the Simpson quadrature rule. Using one of the other ones would have been totally
# equivalent for the purposes of testing.
# This class and the two following functions will be used in the Tests folder (Integration_Test.py).



class py_simpson:

    """
    This class is a python implementation of the composite Simpson quadrature rule.
    No further comments have been written regarding this class, as it will be used only for benchmarking.
    """

    def __init__(self, begin, end, subdivision_n, integrand):
        self.begin = begin
        self.end = end
        self.subdivision_n = subdivision_n
        self.integrand = integrand

        self.partition = []
        self.partition.append(begin)
        self.h = (self.end-self.begin)/self.subdivision_n

        for i in range(1, self.subdivision_n+1):
            self.partition.append(self.partition[i-1]+self.h)



    def compute_integral(self):

        values = []

        for i in range(1, len(self.partition)):

            midpoint = (self.partition[i]+self.partition[i-1])/2


            a = self.integrand(self.partition[i-1])
            b = self.integrand(self.partition[i])
            mid = self.integrand(midpoint)

            values.append(a/6.0+b/6.0+2.0*mid/3.0)

        result = 0
        
        for i in values:
            result +=i

        return result*self.h
    
    




# Now we define a function which automatically compares the computational time needed for the two classes to
# integrate a given function.





def compare_eff_simps_py(begin, end, subdivision_n, integrand, iterations = 10000):

    """
    Function comparing the efficiency of the C++ implementation and of the Python one.

    Parameters:
    - begin: float
        Left endpoint of the integration interval
    -end: float
        Right endpoint of the integration interval
    -subdivision_n: int
        Number of points used for subdivision of the interval in order to perform composite integration.
    -integrand: function
        The real-valued function we want to integrate.
    -iterations: int
        The number of iterations in the for loop

    Returns:
    - float
        The speedup factor
    """

    cpp_time = 0
    python_time = 0


    t1 = time.time()
    for i in range(iterations):

        Simps = itg.Real_Simpson(begin, end, subdivision_n, integrand)
        result1 =Simps.compute_integral()

    t2 = time.time()


    cpp_time += t2-t1


    print(f"C++ implementation took {cpp_time:.6f} seconds.")
    t3 = time.time()

    for i in range(iterations):

        Simps2 = py_simpson(begin, end, subdivision_n, integrand)
        result2 = Simps2.compute_integral()

    t4 = time.time()


    python_time += t4-t3


    print(f"Python implementation took {python_time:.6f} seconds.")

    speedup = python_time/cpp_time

    print(f"Speedup: {speedup}")

    print(f"The last two computed results are {result1} and {result2}.")

    return speedup






# Now we do the same using scipy:






def compare_eff_scipy(begin, end, subdivision_n, integrand, iterations = 1000):

    """
    Function comparing the efficiency of the C++ implementation and of the scipy one.

    Parameters:
    - begin: float
        Left endpoint of the integration interval
    -end: float
        Right endpoint of the integration interval
    -subdivision_n: int
        Number of points used for subdivision of the interval in order to perform composite integration.
    -integrand: function
        The real-valued function we want to integrate.
    -iterations: int
        The number of iterations in the for loop

    Returns:
    - float
        The speedup factor
    """
    cpp_time = 0
    python_time = 0

    t1 = time.time()

    for i in range(iterations):
        Simps = itg.Real_Simpson(begin, end, subdivision_n, integrand)
        result1 =Simps.compute_integral()

    t2 = time.time()

    cpp_time = t2-t1

    print(f"C++ implementation took {cpp_time:.6f} seconds.")

    t3 = time.time()
    for i in range(iterations):

        result2 = quad(integrand, begin, end)[0]

    t4 = time.time()

    python_time = t4-t3

    print(f"Scipy implementation took {python_time:.6f} seconds.")

    speedup = python_time/cpp_time

    print(f"Speedup: {speedup}")

    print(f"The last two computed results are {result1} and {result2}.")

    return speedup






# The following function is used to plot the trend of the error as the number of points increases.
# It was not inserted as a method in the class in order to avoid making it too heavy.







def plot_error(integrator):

    """
    Function plotting the error trend of an integrator.
    The exact result is assumed to be the one provided by scipy 'quad'.

    Parameters:
    - integrator : object derived from <RealBase>
        The object used for integration

    Returns:
    - nothing
        It shows a plot
    """


    old_subdivision_n = integrator.subdivision_n # To avoid side effects
    x_s =  np.arange(1, 26, 1).T
    y_s = []


    for i in x_s:
        integrator.subdivision_n = i
        exact_result = quad(integrator.integrand, integrator.begin, integrator.end)[0]
        y_s.append(abs(exact_result - integrator.cpp_backend.compute_integral()))


    plt.plot(x_s, y_s, 'r')
    plt.xlabel('Number of subintervals')
    plt.ylabel('Error')
    plt.legend('Error trend')
    plt.show()

    integrator.subdivision_n = old_subdivision_n






# The following function is used to test the correctness of the bindings of the C++ code.
# Therefore in these tests we will mainly focus on the C++ classes, since the code written in Python is
# already extensively tested in Homework03/Tests/python_Tests




def main():
    
    one = lambda x : 1
    linear = lambda x : x
    square = lambda x : x**2
    cube = lambda x : x**3
    fourth = lambda x : x**4

    some_ugly_function1 = lambda x : np.sin(x)*x**4 - 4*x +np.log(x+1) / np.exp(1/(x+1))
    some_ugly_function2 = lambda x : np.sin(x) -8*np.log(x+1) + 5j



    print("\n-----------------------------\n")



    # Here we test just a real integrator and a complex one. The other cases are perfectly equivalent.



    R_S = itg.Real_Simpson(begin = 0, end = 1, subdivision_n = 6, integrand = one)

    print("Summary of info about R_S: ", R_S)

    print("\n-----------------------------\n")


    print(f"The integral of the function f(x) = 1 on [0, 1] is {R_S.compute_integral()}. The value provided by scipy is {quad(R_S.integrand, R_S.begin, R_S.end)[0]}.")

    R_S.integrand = cube
    print(f"Integrand has been changed. The integral of the function f(x) = x^3 on [0, 1] is {R_S.compute_integral()}. The value provided by scipy is {quad(R_S.integrand, R_S.begin, R_S.end)[0]}.")

    R_S.integrand = fourth
    print(f"The approximate value integral of the function f(x) = x^4 is {R_S.compute_integral()}. The value provided by scipy is {quad(R_S.integrand, R_S.begin, R_S.end)[0]}.")


    print("\n-----------------------------\n")

    print('Now we will access the members of the class.')


    print(f"The integral [a, b] over which we are integrating is [{R_S.begin}, {R_S.end}].")

    print(f"The stepsize for composite integration is {R_S.h}")
    print(f"The partition of the interval into subintervals for composite integration is {R_S.partition}")
    print(f"The number of subintervals for composite integration is {R_S.subdivision_n}")


    print("\n-----------------------------\n")


    R_S.integrand = some_ugly_function1

    result1 = R_S.compute_integral() # the C++ bindings work!
    result2 = quad(some_ugly_function1, 0, 1)[0] # Compare with scipy

    print(f"The result of the integral is {result1}. It should be {result2}.")


    print("\n-----------------------------\n")


    C_G = itg.Complex_Gaussian(begin = 3, end = 5, subdivision_n = 2, integrand = some_ugly_function2, number_of_nodes = 5, family_of_polynomials = 'Legendre')
    print("Some info about C_G: ", C_G)

    print("\n-----------------------------\n")

    print(f"The number of nodes is {C_G.number_of_nodes}.")
    print(f"The family of polynomials is {C_G.family_of_polynomials}.")
    print(f"The value of alpha is {C_G.alpha}.")
    print(f"The value of beta is {C_G.beta}.")

    print("\n-----------------------------\n")
     
# We have tested pretty much everything regarding C++ bindings. If you wish to test the Python code as well,
# please check the folder "Tests".



if __name__ == '__main__':  # Some minimal tests if executed as standalone. 
    main()
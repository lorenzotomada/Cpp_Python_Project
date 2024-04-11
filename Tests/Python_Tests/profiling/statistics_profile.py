import sys
sys.path.append('../')
sys.path.append('../../../Python_Code/')
sys.path.append('../../../build/')


import os
import cProfile   # https://docs.python.org/3/library/profile.html
import pstats


print('We will profile the code using cProfile. This will also execute the content of statistics_test.py')


if __name__ == "__main__":

    profiling_results_path = "statistics_profile_results.prof"  # Save the results in the current working directory

    profiler = cProfile.Profile()
    profiler.enable()

    
    








    #usr/bin/env python3
    import pandas as pd

    import statistical_analysis as sa
    import statistics_py.py_statistical_analysis as psa
    from os import system

    import time




    # In this file we test basically all the functionalities of our class and compare them to the ones of pandas.
    # Most things are straight-forward, but the non-trivial ones are documented.




    # We define a function wrapper to measure the execution time of a function.

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




    """
    Here we define some functions used just to compare the efficiency of our implementation to pandas'
    """

    @timer
    def openin(path,name):
        psa.DataTable(path,name).cpp_backend    #  create a C++ object Data_Table



    @timer
    def pandas_open(path,name):
        return pd.read_csv(path+name)     # read from a csv using pandas



    @timer
    def mean(Table,key):
        return Table.compute_mean(key)    # compute the mean using our class, given a key



    @timer
    def pandas_mean(DataFrame, key):
        return DataFrame[key].mean()      # the same, using pandas



    @timer
    def median(Table,key):
        return Table.compute_median(key)    # same with the median



    @timer
    def pandas_median(DataFrame, key):
        return DataFrame[key].median()



    @timer
    def std_dev(Table,key):
        return Table.compute_std_dev(key)     # same with std dev



    @timer
    def pandas_std(DataFrame, key):
        return DataFrame[key].std()



    @timer
    def variance(Table,key):
        return Table.compute_variance(key)



    @timer
    def pandas_var(DataFrame, key):
        return DataFrame[key].var()       # with variance



    @timer
    def correlation(Table,key_1,key_2):
        return Table.compute_correlation(key_1,key_2)      # correlation



    @timer
    def pandas_corr(DataFrame, key_1,key_2):
        return DataFrame[[key_1,key_2]].corr()



    print("\n-----------------------------\n")





    print("Here we are interested in analysing the file \"Different_stores_dataset.csv\"")

    print("To have something to compare our results to we first open it via pandas and print the result.")


    print("\n-----------------------------\n")


    path = '../../CSV_files/'
    name = 'Different_stores_dataset.csv'

    psa.CSV_to_panda(name,path)
    A = psa.CSV_to_panda(name,path)

    print(A)
    print("We now open it with our class and again we print the result")



    B = psa.DataTable(path,name)
    B.cpp_backend

    print(B)



    print("\n-----------------------------\n")

    print("To test if the C++ code has been correctly implemented, we compute some properties of the file. To do everything quickly we invoke the method summary that creates a file with all the relevant info")

    x =B.summary()

    system("cat " + x)
    print("\n \n \n \n \n \n \n \n")
    print("Let's now compare it to pandas' one")

    print(A.describe())
    print("\n\n\n\n\n")



    print("As we can see, though slower, ours is way more complete (and that is not because we omitted most of the optional arguments of pandas' function")



    print("\n-----------------------------\n")




    # Test one by one our methods


    y = ["q","Q"]

    print("We now test one by one every method of the class to see that they work.\n If you wish to skip it please press q. Please keep in mind that the tests are not meant to be readable from terminal as they are only used to\n prove that everything was bound correctly.\n Please refer to the tests in the C++ mains for human readable tests.")
    print("Please press something", flush=True)

    a = sys.stdin.readline().strip()  # in order to accept an input, see https://blog.finxter.com/read-from-stdin-in-python/



    if a in y:
        print("Good choice")
        pass

    else:
        #  else we do computations
        print(B.are_numbers("age"))
        print(B.are_categorical("state"))
        print(B["age"])
        print(B.frequency("age","43"))
        print(B.column_max("age"))
        print(B.column_min("age"))
        print(B.compute_mean("age"))
        print(B.compute_std_dev("age"))
        print(B.compute_variance("age"))
        print(B.compute_correlation("selling_price_per_unit","cost_price_per_unit"))
                
    print("As we can see all methods work correctly!")



    print("\n-----------------------------\n")



    print("We now test that exceptions are handled as we'd like to")
    print("Here we try to take the mean of some categorical data:")


    try:
        B.compute_mean("state")
    except:
        print("As we can see we failed, so we have arrived at this message")



    print("\n-----------------------------\n")



    print('In the dataset there are ', B.count_NaNs(), ' missing values')

    # Let us try with the python class
    print('To show that also the method for the python class works as expected, we count missing values once more')
    print('In the dataset there are ', B.count_NaNs(), 'missing values')



    print("\n-----------------------------\n")



    # Testing histogram and scatterplot


    print("We now move to testing the histogram and scatterplot functionalities.")


    #B.histogram("state", True)
    B.scatterplot("cost_price_per_unit", "selling_price_per_unit")


    print("Let's end this with some profiling. Unfortunately pandas takes almost every win. On the plus side, if we execute these functions in C++ we get better efficiency than pandas so most of the trouble is due to the conversion to python\n")
    print("In the following, the first result is our function's time, the second one is pandas'")



    print("\n-----------------------------\n")
    # Test again the functions performing statistical analysis on numerical data

    print("mean")
    mean(B,"age")
    pandas_mean(A,"age")

    print("\n-----------------------------\n")

    print("median")
    median(B,"age")
    pandas_median(A,"age")

    print("\n-----------------------------\n")

    print("stdandard deviation")
    std_dev(B,"age")
    pandas_std(A,"age")

    print("\n-----------------------------\n")

    print("variance")
    variance(B,"age")
    pandas_var(A,"age")

    print("\n-----------------------------\n")

    print("correlation")
    correlation(B,"cost_price_per_unit","selling_price_per_unit")
    pandas_corr(A,"cost_price_per_unit","selling_price_per_unit")

    print("\n-----------------------------\n")

    print("opening the file")

    openin(path,name)
    pandas_open(path,name)




    print("Thank you for testing the statistics module.")








    profiler.disable()

    profiler.dump_stats(profiling_results_path)

    stats = pstats.Stats(profiling_results_path)  # Print the results
    stats.sort_stats("cumulative")
    stats.print_stats()

    print("Profiling results saved to", profiling_results_path)
General info
-

This folder contains all the code written by me and my colleague Riccardo Redigolo for the last homework project of the _Advanced Programming_ course held at SISSA (academic year: 2023-2024, see https://www.math.sissa.it/course/phd-course-master-course/advanced-programming-5 for more details).

The task was to build a Statistics and Numerical Analysis toolbox. In particular, in addition to the compulsory Statistics part, we implemented the Numerical Integration module.

This project is built on top of the previous homework, as it consists in the Python bindings (as well as other additions) for the previously written C++ backend.


Project structure
-

The folder contains the following subfolders: Python_Code, C++_Code and Tests.




C++_Code
-
The folder C++_Code contains all the code written in C++ for the previous project, which has seen no major changes with respect to Homework 2. The subfolder called `Bindings` contains two different files, called, respectively, `Statistics_py.cpp` and `Numerical_Integration_py.cpp`.

Each one of them contains the bindings of every method and attribute which is present in the C++ code, complemented with the corresponding documentation.




Python code
-
The folder Python_Code contains all the code written in Python in order to complement the functionalities of the classes and the functions imported from C++.
There are two subfolders: statistics_py and integration_py.

The names of the subfolders are in lower case, contrarily to the other names in the project, in order to be consistent with the naming convention of modules in Python. The same also applies to the filenames of files containing Pytohn code.

Each subfolder name-of-the-module_py contains an `__init__.py` file which writes which are the classes or functions to make available when writing

```python
from [name] import [something]
```



INTEGRATION: 
In integration_py.py we created Python classes to enhance the C++ classes with some new functionalities using decorators or free functions.

Regarding design choices:

Each class saves the corresponding C++ class as property (hence making it possible to change e.g. the stepsize or the endpoints of the integration interval, automatically uploading the integrator).

We imported ABC and abstractmethod from abc in order to be able to declare RealBase and ComplexBase as abstract classes, consistently with what was done in C++.

The newly added method is called `estim_orders` and has the goal of providing an estimation of the polynomial order and of the convergence order of the method considered.

Regarding performance balance between C++ and Python: see "Tests" below.



STATISTICS:
In statistical_analysis_py.py we allow users to do statistical analysis on CSV files.

Regarding design choices: Here we implement a Python class where we implement all the methods we defined in the C++ one to make everything easier for Python users. We complement our methods with some of pandas to make some things prettier (e.g. printing a Data_Table). We also use seaborn to allow users to easily plot the data in histograms or scatterplots. For ease of usability, we allowed the user to choose some options (like the dimension of the dots of the scatterplot) interactively.

Regarding performance balance between C++ and Python: see "Tests" below.



Tests:
-
This folder contains the tests which we have written in order to show the correctness of our bindings and to do  some benchmarking tests, comparing our functionalities with the one provided by standard libaries, such as numpy, pandas and scipy.

There are three subfolders: C++_Tests, Python_Tests and CSV_Files.

CSV_Files contains some CSV files used as examples in the statistical analysis module tests.

C++_Tests contains the code of the tests written in C++ for Homework02 (the corresponding executable is generated in the build folder).

Python_Tests, on the other hand, contains two files, one for each module for the testing of the corresponding part, and one subfolder called `profiling`, containing two python modules which run the tests in the parent folder and do the profiling.

For profiling we have used Python's cProfile. The profiling files can be run as usual and they perform the same tests which can be found in [corresponding_module]_test.py, also profiling.
The outputs are then saved.


INTEGRATION:
Regarding the test of the integration module, we tested pretty much each of the methods and attributes present in the binding and basically all of the functionalities of the Python classes.

Regarding efficiency gain, it is possible to see that the scipy method for integration `quad` is faster than our implementation, although for a relatively small number of intervals used for composite integration this difference is at most of one order of magnitude.

We have also implemented directly in Python one of the classes (the real Simpson integrator) and tested it against the C++ version.

Interestingly enough, the C++ code takes around the same execution time of the Python one.
This is probably due to the fact that the bindings introduce some overhead which is not compensated by C++ superior computational power since the operations performed are very simple ones.


STATISTICS:
Unfortunately, although more efficient than just writing code in Python, our binded code is not comparable to pandas. If creating the data frame is twice as inefficient, computing medians or means is almost 500 times slower than doing it in pandas. However, if we do these operations with the compiled C++ code, we see that the picture is nowhere near as bleak:
opening files is more efficient than pandas' version and our functions compute means and medians just a little slower than pandas does. So a lot of the inefficiency is due to binding the C++ code to python.




INSTRUCTION FOR COMPILATION:
-
To compile using CMake type in your terminal:


```bash
mkdir build

cd build

cmake -S ..

```


The last command might be followed by:


```bash
-D STATISTICS=ON
```

if you wish to compile *only* the statistics part


```bash
-D INTEGRATION=ON
```

if you wish to compile *only* the integration part.

The resulting executable(s) are saved in the build directory.


Alternatively, it is possible to compile everything (assume you want to compile both libraries) typing on yuor terminal:

```bash
source makeCMake.sh
```

In addition to compilation, this exports the PYTHONPATH variable appending to it what is necessary to run the tests in the `Python_Tests` folder. This should not be necessary when executing our default tests, since we have appended the path to the relevant folders using `sys.path.append`, however it might be useful if you wish to do some tests on your own.

Alternatively, instead of running CMake, thanks to the presence of `setup.py`, it is possible to install the package via pip, writing

```bash
pip install .
```
In that case, the C++ tests' executables will be saved in build/temp.linux-x86_64-cpython-312

whereas the resulting shared libraries will be placed at build/lib.linux-x86_64-cpython-312

The installation via pip is available only in case of combined compilation of the statistics and integration parts.




After compilation, in any case you can just run the tests typing in the Python_Tests folder

```bash
python3 [module_to_test]_test.py
```



OLD README:
-
For completeness, here we also insert all the info contained in the previous README.md file of Homework02.




STATISTICS: Our code is able to read CSV files. We created two classes, CSV_Handler and Data_Table.

CSV_Handler is used to read the file but saves no information, whereas Data_Table is used to actually store the data and to perform statistical operations.

CSV_Handler was inserted because if a user only needs to access some element (or to perform operations not requiring to know the whole column/row) this class has a great advantage in terms of spatial complexity
Data_Table is a vector of unordered maps. This choice allowed us to save the rows by their index and each column by the name of
its key. The choice to go for an unordered map instead of an ordered one was made looking at computational efficiency.


All the statistical operations are methods of the Data_Table class. These allow users to compute the most significant values out of the columns containing numbers (mean, median, variance, standard deviation, correlation of two columns), to access single elements in the dataset, to count missing numbers, to count the occurrency of a specific event, to classificate an element in a given position (numerical/categorical), to drop selected rows/columns and to drop all the rows containing missing values.
Also, we created a function which prints to an external file all the relevant info.


The file Test_QoL.hpp contains functions that are useful in the main.
The file Iterators.hpp contains the definition of the iterators for our class.


More details regarding the statistics part can be found in the dedicated files.
IMPORTANT: The dataset US_Crime_Rates.CSV is not well done, in the sense that it is not properly organized. It was downloaded from Kaggle and included here in order to test the function which opens the files, but we suggest not to use it for the testing because the data is not stored properly in the file itself.
The other datasets, on the contrary, work perfectly.


NUMERICAL ANALYSIS: Our code allows users to perform integration of real or complex valued functions of real variable.
We created an abstract class from which the derived classes of the specific methods inherit.
We implemented the midpoint rule, the trapezoidal rule, the Cavalieri-Simpson formula and the Gaussian quadrature formulas.
Regarding convergence order and polynomial order, the results of the (detailed) study carried out is that they match the theoretical predictions.
Regarding their efficiency, they tend to be not as efficient as the formulas provided by the library Boost. This particularly applies to the case of Gaussian quadrature, which we implemented using GNU GSL. In the other cases, the order of magnitude is the same or at most one more.


The file Functions.hpp contains some examples of functions which are used in the testing.


More details regarding the numerical analysis part can be found in the dedicated files.


The two libraries which we used are Boost and GNU GSL. Boost was used to test the properties of the methods, while GSL was used in order to perform Gaussian Integration. Since they are common libraries and can easily be installed using sudo apt, we decided not to include their code in this folder, also for memory reason (the compressed boost folder alone already occupies more than 130MB).

Thank you for reading the README.md file.

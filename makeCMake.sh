#!bin/bash

echo "Compiling the project..."

# rm -r build/ # This would make it non-portable (Windows does not support the command rm)

if [ ! -d "./build/" ]; then
    mkdir build
fi


cd build

cmake -S .. -B .
make -j4

echo "Done!"
echo "Non we will export the variable PYTHONPATH, adding to it the path to the folders containing python files."

cd ../Tests/Python_Tests/
export PYTHONPATH=$PYTHONPATH:../../Python_Code/:../../Python_Code/integration_py/:../../Python_Code/statistics_py/:../../build/:./
echo "You have been redirected to the folder python_Tests. This folder contains some tests to check that everything works as expected as refers to the binding of the C++ code and its enrichment using python."
echo "To test the python code, type python3 followed by the name of the file you wish to open."
echo "The folder ../cpp_Tests contains the source code needed to compile either the main for the numerical integration part, for the statistical analysis part or for both."
echo "The C++ tests are  unchanged from the last project, as no improvement were suggested. If you wish to run them, please type cd ../../build/ and ./executable, where the name of the executable depends on what part of the project you decided to compile (the default is Project_Test)".

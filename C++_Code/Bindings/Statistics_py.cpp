#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../Includes/Statistics/Data_Handling.hpp"
#include "../Includes/Statistics/Iterators.hpp"


namespace py = pybind11;


PYBIND11_MODULE(statistical_analysis, m) {
  m.doc() = "This module is used to perform statistical analysis operations on the content of a CSV file.";



/* Now we bind the class CSV_Handler. We bind all the attributes and methods we have defined in the C++ code in
order to provide seamless interoperabilty between Python and C++ code. */



  py::class_<CSV_Handler>(m, "CSV_Handler")

    .def(py::init<const std::string &, const std::string &, bool> ())         // bind the constructor in a standard way 
    
    .def("open", &CSV_Handler::open, "The following functions opens the CSV file and allows you to use the object of the class CSV_Handler to perform statistical operations on the file")
    
    .def("size", &CSV_Handler::size, "Gives you the amount of rows of the CSV file")

    .def("keys", &CSV_Handler::keys, "Gives you a vector containing all the different keys of the CSV file")


    /* Now using magic methods. For __call__, we used overload_cast becaus operator() was overloaded in the C++
    code, as it could accept either the number of the line or the number and a string. */


    .def("__call__", py::overload_cast<const unsigned int>(&CSV_Handler::operator()), "Gives you the selected row of the file")
    .def("__call__", py::overload_cast<unsigned int &, std::string &>(&CSV_Handler::operator()), "Gives you the element in the position you indicated (unsigned int row, std::string key)")

    .def("__repr__", &CSV_Handler::summary_string, "Gives you the name (with path) of the CSV file you are operating on togheter with its opening status")

    .def("__doc__",[](){
      return "------------------------------------------------------------------------------------------------------\n This class was implemented so as to have a way to access the information contained in the .csv file without the need of storing it into a container, hence allowing for great space complexity.\n The constructor takes as input the path of the file (absolute or relative) and a boolean (used to determine wheter the file is open or not) to create an ifstream that allows us to operate on the file.\n Since most of the statistical operations require to know a lot about the file, to prioritise efficiency, we define them only for the objects affering to the class Data_Table, which, storing all the information in a container allow for better efficiency. Nevertheless, in this class we implement some methods, like access or printing all keys, that do not require to know a lot about the file and are thus definible without loss of efficiency.\n ------------------------------------------------------------------------------------------------";
    })


    .def_property_readonly("path", &CSV_Handler::get_path, "The (relative) path to the file")
    .def_property_readonly("name", &CSV_Handler::get_name, "The name of the file")
    .def_property_readonly("open_status", &CSV_Handler::get_status, "Tells you if the file is currently open or not")
    .def_readonly("file", &CSV_Handler::file, "The file");
  
  
  
  py::class_<Data_Table>(m,"Data_Table")

    .def(py::init<unsigned int &, std::vector<std::string> &, std::vector<std::unordered_map<std::string, std::string>> &> ())
    
    .def("__call__", &Data_Table::operator(), "Allows you to access the element in the position you indicated (unsigned int row, std::string key)")
  
    .def("drop_column", &Data_Table::drop_column, "Drops a column")
    .def("drop_row", &Data_Table::drop_row, "Drops a row")
    .def("drop_NaNs", &Data_Table::drop_NaNs, "Drops all rows containg NaNs")

    // For the next two functions overload_cast does not work, so we do it in a more old-fashioned way:
    .def("count_NaNs", static_cast<unsigned int (Data_Table::*)(const std::string&) const>(&Data_Table::count_NaNs), "Counts the number of NaNs present in the given column")
    .def("count_NaNs", static_cast<unsigned int (Data_Table::*)() const>(&Data_Table::count_NaNs),"Counts the total number of NaNs present in the file")

    .def("are_numbers",&Data_Table::are_numbers, "Extracs a vector with all the objects of integral type in the given column")
    .def("are_categorical",&Data_Table::are_categorical, "Extracs a vector with all the objects of categorical type in the given column")
    .def("select_values", &Data_Table::select_values)
    .def("classification", &Data_Table::classification, "Tells you the type of the item you selected")
    .def("frequency", &Data_Table::frequency, "Tells you how many times the value you selected appears in the selected column")

    .def("column_min", &Data_Table::column_min, "Tells you the minimal value present in the column. Throws an exception if the column contains no numerical values")
    .def("column_max", &Data_Table::column_max,"Tells you the maximal value present in the column. Throws an exception if the column contains no numerical values")
    .def("compute_mean", &Data_Table::compute_mean,"Computes the mean of the values present in the column. Throws an exception if the column contains no numerical values")
    .def("compute_median", &Data_Table::compute_median, "Computes the meadian of the values present in the column. Throws an exception if the column contains no numerical values")
    .def("compute_variance", &Data_Table::compute_variance,"Computes the variance of the values present in the column. Throws an exception if the column contains no numerical values")
    .def("compute_std_dev", &Data_Table::compute_std_dev,"Computes the standard deviation of the values present in the column. Throws an exception if the column contains no numerical values")
    .def("compute_correlation", &Data_Table::compute_correlation,"Computes thecorrelation of the values present in the columns you selected. Throws an exception if any of the two columns contains no numerical values or the two have different dimensions")

    .def("summary", &Data_Table::summary, "Saves most of the relevant info in a .txt file, whose name you are allowed to choose in iterim, in the folder \"output_files\". If the folder is not yet present, it first creates it")
    
    .def_readonly("file_name",&Data_Table::file_name)
    .def_property_readonly("n_rows", &Data_Table::get_n_rows)
    .def_property_readonly("column_keys", &Data_Table::get_column_keys)
    .def_property_readonly("Table", &Data_Table::get_Table);
    
  

  m.def("Convert_file_to_Table", [](CSV_Handler& CSV_file){return Convert_file_to_Table(CSV_file);}, "Function to convert a CSV_Handler object in a Data_Table./n/n It reads the content of the file from the CSV_Handler object and saves it to a Data_Table./n/nParameters:/n--------/n    CSV_file: statistical_analysis.CSV_Handler/n        Object containing the content of the CSV file.nReturns:/n--------/nstatistical_analysis.Data_Table/n    The returned object saves the content of the CSV file.");
  m.def("column_from_key", [](Data_Table& Table, std::string key){return column_from_key(Table,key);}, "Function returning the selected column of a Data_Table./n/n It checks if the column exists and returns it as a vector of strings./n/nParameters:/n--------/n    Table: statistical_analysis.Data_Table/n        Table of which you want to select the column./n    key:/n    string/n        Label of the column you want to select./nReturns:/n--------/nA vector/n    The column, saved as a vector of strings.");
  m.def("unique_column", [](Data_Table& Table, std::string key){return unique_column(Table,key);});

  py::register_exception<std::runtime_error>(m, "RuntimeError");


  /* The following class consists of the bindings of the iterators for the CSV_Handler class.
We do not provide further explanation since the iterators we are binding are standard ones. */



  py::class_<Random_Access_Iterator<std::string>>(m, "Random_Access_Iterator_CSV_Handler")

    .def(py::init<std::string*> ())     // we bind the constructor


    // Now using Python's magic methods syntax 


    .def("__eq__", &Random_Access_Iterator<std::string>::operator==, "Equality comparison for iterators.")

    .def("__ne__",&Random_Access_Iterator<std::string>::operator!=, "Inequality comparison for iterators. ")

    .def("__next__", py::overload_cast<>(&Random_Access_Iterator<std::string>::operator++), "Increment.");



}

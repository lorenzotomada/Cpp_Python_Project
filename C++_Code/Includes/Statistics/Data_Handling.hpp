#ifndef Data_Handling_Hpp
#define Data_Handling_Hpp

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <variant>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <algorithm>
#include <memory>
#include "Iterators.hpp"


/* This class was implemented so as to have a way to access the information contained in the .csv file without the need of storing it into a container, hence allowing for great space complexity. The constractor takes as input the path of the file (absolute or relative) and a boolean (used to determine wheter the file is open or not)to create an ifstream that allows us to operate on the file. 
Since most of the statistical operations require to know a lot about the file, to prioritise efficiency, we define them only for the objects affering to the next class, which, storing all the information in a container allow for better efficiency. Nevertheless, in this class we implement some methods, like access or printing all keys, that do not require to know a lot about the file and are thus definible without loss of efficiency.*/


class CSV_Handler {
public:
  CSV_Handler(const std::string &path, const std::string &name, bool open_status = false) : path(path), name(name), open_status(open_status) {
    if (this->open_status == true) {
      file.open(this->path+this->name, std::ifstream::in);
      if (this->file.is_open()) {std::cout << "File opened successfully" << std::endl;}
      else {
	this->open_status = false;
        throw std::runtime_error("Could not open file");   //throwing an exception if the file could not be opened. Having written error safe code, we also set the opening status to false so as to avoid 
      }                                                    //the file being used in other functions.
    }
  }






  /*Here we define the class' methods*/

  
  void open();


  unsigned int size();
  

  std::string operator()(const unsigned int line_num); /* The compiler does not allow us to declare it as a
  const method due to the fact that we are working with ifstream. */


  std::vector<std::string> keys(); // Same regarding the use of const.


  std::variant<double, std::string> operator()(unsigned int &line_num, std::string &key);
  // Also here we cannot declare the method as const since we are working with ifstream.

  std::string summary_string();


  typedef Random_Access_Iterator<std::string> iterator;

  typedef Random_Access_Iterator<const std::string> const_iterator;

  iterator begin();
  
  iterator end();




  

  /*We define some getter methods to get the protected data*/


  std::string get_path() const {return this->path;}
  std::string get_name() const {return this->name;}
  bool get_status() const {return this->open_status;}






  /*we define a custom destructor that closes the file when the object goes out of scope*/

  ~CSV_Handler(){
    if (this->open_status == true) {
      this->file.close();
    }
  }







  /*Here we define field names*/

  

  std::ifstream file;                //this is the field name that allows us to operate on the file. Since ifstreams are not allowed to be r-values we define it as public (we cannot define a getter method)

protected:  
  std::string path;
  std::string name;
  bool open_status;
};




/*This class was created to simplify the access to the information contained in a CSV file. Through a function that we will comment at the end of this file, we are able to convert an object of the previous class to one of this one by storing all of its data in the container "Table" given by a vector of maps. This makes accessing the information of the file much easier (although this comes at the expence of a much higher space complexity [O(columns*rows)]) as each element of the vector indicates a different row. Using maps we can easily organise the information contained in these rows by using as keys the elements contained in the first row of the file. In this class we also implement most of the statistical methods.*/ 

class Data_Table{
public:
  
  /*The constructor initialises the file_name as "Undefined" as in general objects of the class Data Table do not come from objects of the class CSV Handler. Nevertheless since when we convert a CSV Handler object to a Data Table object we still want to know which file we are operating on, we define this field name.*/
  
  Data_Table(unsigned int &n_rows, std::vector<std::string> &column_keys, std::vector<std::unordered_map<std::string, std::string>> &Table) : n_rows(n_rows), column_keys(column_keys), Table(Table){
    file_name = "Undefined";
  }
  



  

  /*Here we define the begin ed end functions allowing us to use the custom iterators defined in the Iterators header.*/

  typedef Random_Access_Iterator<std::unordered_map<std::string, std::string>> iterator;      //we redefine the type just to have it look nicer

  iterator begin() {return iterator(&this->Table[0]);}

  iterator end() {return iterator(&this->Table[Table.size()]);}
 





  /*Here we define the class' methods to perform statistical analysis. */


  // The following method is used to access an element in the table using row index and column name.
  // If the value is not present, an exception of type NaNException is thrown (see the end of this file)

  std::variant<double, std::string> operator()(const unsigned int &row_number, const std::string &key) const;



  //The following functions are used, respectively, to drop a selected column, to drop a selected row and to drop
  // all the rows containing missing values.
  //They are not labelled as const because using them modifies the dataset.

  void drop_column(const std::string &column_to_drop);

  void drop_row(const unsigned int &row_to_drop);

  void drop_NaNs();



  // The following two functions are used to count, respectively, all the missing values in a given column and
  // all the missing values in the dataset.

  unsigned int count_NaNs(const std::string &key) const;

  unsigned int count_NaNs() const;



  /* The following two member functions act on a selected column.

  are_numbers returns a vector containing all the numerical values in a column (which might be different from
  the column itself if there are missing values or a mixture of categorical and numerical values). This method
  will be crucial for the ones which compute mean, median or other significant quantitative values. */

  std::vector<double> are_numbers(const std::string &key) const;


  // are_categorical returns the number of categorical observations in a given column

  unsigned int are_categorical(const std::string &key) const;




  /* The following function creates an unordered map (same structure as Data_Table) which only
  contains the rows whose feature in the column "key" is the one given by "target". */

  std::vector<std::unordered_map<std::string, std::string>> select_values(const std::string &key, const std::string &target) const;



  // classification just prints on the terminal the type of the value in the given position (or it specifies
  // that the value is missing).

  void classification(unsigned int &row, std::string &column) const;




  // the following function is used to compute the number of occurrencies of the attribute "value" in the
  // column with string "key"

  unsigned int frequency(const std::string &key, const std::string &value) const;




  /* The following functions perform statistical operations on the dataset.
  They take as input a column (which is supposed to contain numerical values).
  They are able to run even if the column contains both numerical and categorical data, by just 
  considering the numbers in the column.
  
  Apart from that, the names of the methods suggest their aim. They all on a given column. */

  double column_max(const std::string &key) const;

  double column_min(const std::string &key) const;



  double compute_mean(const std::string &key) const;

  double compute_median(const std::string &key) const;

  double compute_variance(const std::string & key) const;

  double compute_std_dev(const std::string &key) const;



  /* The following method needs two column names as input and computes the correlation between them, if possible.
  It is not labelled as const for reasons specified in Statistics.cpp. */

  double compute_correlation(const std::string &key1, const std::string &key2);



 // The following function prints all the relevant info on an output file, whose name can be specified by the user.

  std::string summary() const;




  // getter methods

  unsigned int get_n_rows() const {return this->n_rows;}

  std::vector<std::string> get_column_keys() const {return this->column_keys;}

  std::vector<std::unordered_map<std::string, std::string>> get_Table() const {return this->Table;}



  /*No need to define a custom destructor as there is nothing dynamic in this class*/




  /*Here we define the field names of the class*/
  
  std::string file_name;             /*field name is left public to distinguish it from the field names required to construct the class. (Also because no sensible information in stored in it)*/

  
protected:
  unsigned int n_rows;
  std::vector<std::string> column_keys;
  std::vector<std::unordered_map<std::string, std::string>> Table;
};





/*This is a crucial function that allows us to get all the information stored in a .csv file and store it in a container for later use. More documentation can be found in the ReadMe file or in
  Data_Handling.cpp */ 

Data_Table Convert_file_to_Table(CSV_Handler& CSV_file);


// The following funcion has the goal of selecting a column given the corresponding key.

std::vector<std::string> column_from_key(Data_Table& Table, std::string key);


// The following function creates a vector containing all the values of the chosen column only once.

std::vector<std::string> unique_column(Data_Table& Table, std::string &key);




/*This function is just a templatization of the % that outputs a string. Useful for saving time when printing results.*/
template <typename type>
std::string percentage(const type &number, const type &total) {
  double x_1 = number;
  double x_2 = total;
  double result = x_1/x_2*100;
  std::stringstream percentage;
  percentage << result << "%";
  return percentage.str();
}





/*This function modifies slightly the std::stod function to "correct" an unpleasant behaviuour of the latter.
std::stod infact converts strings to doubles even if only the beginning of the string is convertible without
throwing an exception. (e.g. it will convert "3dicembre2023" to 3.0) Since we needed to distinguish categorical
values from numerical ones, we modified it so as to only modify "true" numbers.*/
double exception_stod(const std::string& input);



// We define a custom exception class which is thrown when we try to access a missing value in the dataset.

class NaNException : public std::exception {
public:
  const char* what() const noexcept override {return "Error: you are trying to access a missing value.";}
};

#endif

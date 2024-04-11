#include "../../Includes/Statistics/Data_Handling.hpp"
#include "../../Includes/Statistics/Iterators.hpp"
#include <numeric>
#include <cmath>
#include <exception>



// Please read Data_Handling.hpp if you want more info reagarding the aim to which these methods have been written.


// We use count_if coupled with a suitable lambda function in order to count all the occurrencies of a given
// attribute (given by the string value) in the column labelled by key

unsigned int Data_Table::frequency(const std::string &key, const std::string &value) const{
  return std::count_if(this->Table.begin(), this->Table.end(), [&](const std::unordered_map<std::string, std::string> &map){return map.at(key) == value;});
}



void Data_Table::drop_column(const std::string &column_to_drop){
  for (size_t i = 0; i < this->Table.size(); ++i) {(this->Table[i]).erase(column_to_drop);}
  this->column_keys.erase(std::find(this->column_keys.begin(), this->column_keys.end(), column_to_drop));
  // In addition to erasing the column, we also erase the name of the key from the list.
}



void Data_Table::drop_row(const unsigned int &row_to_drop){
  std::vector<std::unordered_map<std::string, std::string>>::iterator to_be_erased = this->Table.begin() + row_to_drop;
    
  // https://stackoverflow.com/questions/671423/c-stl-vectors-get-iterator-from-index to get an iterator from an index
  
  this->Table.erase(to_be_erased);
  this->n_rows = this->Table.size();
}



void Data_Table::drop_NaNs(){
  for (size_t i = 0; i < this->Table.size(); ++i){
    for (std::string key : this->column_keys){
      if (((this->Table[i]).at(key)).empty()) {this->drop_row(i);}
    }
  }
}



// We use count_if coupled with a suitable lambda function in order to count the missing values in a given column.

unsigned int Data_Table::count_NaNs(const std::string &key) const {
  return std::count_if(this->Table.begin(), this->Table.end(), [&](const std::unordered_map<std::string, std::string> &map){return map.at(key) == "";});
}



unsigned int Data_Table::count_NaNs() const {
  unsigned int counter = 0;

  for (std::string key : this->column_keys) {
    unsigned int column_NaNs = this->count_NaNs(key);
    counter += column_NaNs;
  }

  return counter;
}



std::vector<double> Data_Table::are_numbers(const std::string &key) const {
  std::vector<double> are_numbers;

  for (auto map: this->Table){

    try {
      double x = exception_stod(map.at(key)); // We try to convert to a number using the function exeption_stod
      // (please read the Data_Handling.hpp file for more informations regarding exception_stod).
      are_numbers.push_back(x);
    }

    catch (const std::invalid_argument & e) {} // If we cannot convert, we do not have a number, so we do not
    // insert it and we just go on.
    }

  return are_numbers; // we return the vector containing all the numbers in the column
}



unsigned int Data_Table::are_categorical(const std::string &key) const {

  unsigned int counter = 0;

  for (auto map: this->Table){
    try {exception_stod(map.at(key));} // if we can convert, it is not a categorical variable, but a number
    catch (const std::invalid_argument & e) {++counter;} // If we cannot convert it, then it is not a number, so
    // we add one to the counter
  }

  return counter-Data_Table::count_NaNs(key); // subtracting the number of NaNs means that missing values are not
  //considered as categorical values
}



std::vector<std::unordered_map<std::string, std::string>> Data_Table::select_values(const std::string &key, const std::string &target) const {
  
  std::vector<std::unordered_map<std::string, std::string>> selected_values;
  
  for (size_t i = 0; i < this->Table.size(); ++i){
    if (this->Table[i].at(key) == target) {selected_values.push_back(this->Table[i]);}
    // We just push back the rows corresponding to the desired key.
  }

  return selected_values;
}



void Data_Table::classification(unsigned int &row, std::string &key) const {
  try{
    std::variant<double, std::string> content = this->operator()(row, key); // we access using the overload of ()
    // and then we try to convert.
    if (std::holds_alternative<double>(content)) {std::cout << "The element in position " << row << " in the column " << key << " is a number." << std::endl;}
    if (std::holds_alternative<std::string>(content)) {std::cout << "The element in position " << row << " in the column " << key << " is categorical." << std::endl;}
    else {throw std::runtime_error("Invalid type.");}
  }
  catch (const NaNException &e){std::cout << "The value is missing." << std::endl;}
}



/* In the following, we define functions that as input take columns that should contain numbers.
If the column does not contain any numbers, the functions throw an exception.
However, if the column contains both numerical and categorical values, then the functions compute the
mean/variance... of all the numbers that are present, so it is not needed that they are all numbers, there
could be also categorical variables.
The following functions are not commented in detail because they are just implementations of standard
statistical formulas. */



double Data_Table::column_max(const std::string &key) const {

  std::vector<double> are_numbers = this->are_numbers(key);
  if (are_numbers.size()==0) {throw std::runtime_error("It is impossible to compute the maximum of the column because there are no numerical data in it.");}
  
  auto it = std::max_element(are_numbers.begin(), are_numbers.end()); // max_element returns an iterator
  return *it; // we dereference it to return the actual value
}



double Data_Table::column_min(const std::string &key) const {

  std::vector<double> are_numbers = this->are_numbers(key);
  if (are_numbers.size()==0) {throw std::runtime_error("It is impossible to compute the maximum of the column because there are no numerical data in it.");}
  
  double min_value = *std::min_element(are_numbers.begin(), are_numbers.end());
  return min_value;
}



double Data_Table::compute_mean(const std::string &key) const {

  // We create a vector containing all the numbers in the column and we compute the mean.
  // This will be the standard approach in these functions.

  double mean = 0.0;
  std::vector<double> are_numbers = this->are_numbers(key);

  if (are_numbers.size()!=0) {mean = std::accumulate(are_numbers.begin(), are_numbers.end(), mean)/(1.0*are_numbers.size());}
  else {throw std::runtime_error("It is not possible to compute the mean because there are no numerical data in the column.");}
  
  return mean;
}



double Data_Table::compute_median(const std::string &key) const {

  double median = 0.0;
  std::vector<double> are_numbers = this->are_numbers(key);

  if (are_numbers.size()!=0){
    std::sort(are_numbers.begin(), are_numbers.end());
    if (are_numbers.size()%2!=0) {median = are_numbers[are_numbers.size()/2];}
    else {median = (are_numbers[are_numbers.size()/2]+are_numbers[are_numbers.size()/2-1])/2;}
  }

  else {throw std::runtime_error("It is impossible to compute the median because there are no numerical data in the column.");}
  
  return median;
}



double Data_Table::compute_variance(const std::string & key) const {

  std::vector<double> are_numbers = this->are_numbers(key);
  double variance = 0.0;

  if (are_numbers.size()!=0) {
    double mean = std::accumulate(are_numbers.begin(), are_numbers.end(), 0.0)/(1.0*are_numbers.size());
    variance = std::accumulate(are_numbers.begin(), are_numbers.end(), 0.0, [&mean, &key](double var, double j){
    return var + (j - mean) * (j+ mean);})/(1.0*are_numbers.size());
  }

  else {throw std::runtime_error("It is impossible to compute the variance because there is no numerical data in the column.");}
  
  return variance;
}



double Data_Table::compute_std_dev(const std::string & key) const {

  double std_dev = this->compute_variance(key);
  std_dev = std::sqrt(std_dev);

  return std_dev;
}



double Data_Table::compute_correlation(const std::string &key1, const std::string &key2) {

  // We drop the rows containing missing values because we want the two columns to have the same length.
  // The we go on just implementing the standard formula which can be found, for instance, on wikipedia.

  this->drop_NaNs();
  std::vector<double> are_numbers1 = are_numbers(key1);
  std::vector<double> are_numbers2 = are_numbers(key2);

  if (are_numbers1.size()!=are_numbers2.size()) {throw std::range_error("Error. The two columns do not contain the same amount of numbers. Please use the command drop_NaNs on the dataset or on a copy to get rid of missing values and try again.");}
  
  double mean1 = std::accumulate(are_numbers1.begin(), are_numbers1.end(), 0.0)/(1.0*are_numbers1.size());
  double mean2 = std::accumulate(are_numbers2.begin(), are_numbers2.end(), 0.0)/(1.0*are_numbers2.size());

  double correlation = 0;
  
  
  for (size_t i = 0; i < are_numbers1.size(); ++i) {correlation += (are_numbers1[i] - mean1)*(are_numbers2[i] - mean2);} 

  double stddev1 = this->compute_std_dev(key1);
  double stddev2 = this->compute_std_dev(key2);
  correlation =(correlation*are_numbers1.size())/(std::pow((are_numbers1.size()-1), 2.0)*stddev1*stddev2);
  
  return correlation;
}



std::string Data_Table::summary() const {

  // The following two lines are needed to print in a fashionable way.

  std::string output;
  std::string kernel = "\033[1;33mSTATISTICAL_ANALYSIS>\033[0m ";
  std::cout << kernel << "Please enter the name of the output file (do not specify the extension .txt). By typing default you will get a file named like the one you are operating on followed by \"_summary.txt\"." << std::endl;
  std::cin >> output;

  if (output == "default") {output = this->file_name+"_summary";} // providing default names

  output+=".txt";
  std::cout << kernel << "Saving the most relevant info... " << std::endl; 

  // Now we print some information which specify the status of the file (whether it is created from scratch or not)

  int t =  system("cd output_files");

  if (t == 0) {
    std::cout << kernel << "The file will be saved inside the already existing \"output_files\" directory.\n";
  }

  else {
    std::cout << kernel << "To store the file a directory named \"output_files\" will now be created.\n";
    system("mkdir output_files");
  }
  std::ofstream output_txt("./output_files/"+output); // We create the file or we overwrite it if already existing.

  if (!output_txt.is_open()) {
    std::cerr << "It was not possible to create the file. Please try again.";
    this->summary(); // We call the function again so that the file can be regularly created.
  }

  // Now we can print the info on the file.

  output_txt << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

  output_txt << "There are " << this->n_rows << " rows and " << this->column_keys.size() << " columns in the dataset." << std::endl;
  
  output_txt << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
  
  output_txt << "The names of the columns are: " << std::endl;

  for (size_t i = 0; i < this->column_keys.size(); ++i){
    if (i == this->column_keys.size()-1) {output_txt << this->column_keys[i] << std::endl;}
    else {output_txt << this->column_keys[i] << ", ";}  
  }

  output_txt << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

  output_txt << "There are " << this->count_NaNs() << " missing values in the dataset." << std::endl;

  output_txt << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

  for (std::string key : this->column_keys) {
    std::vector<double> are_numbers = this->are_numbers(key);
    output_txt << "The column " << key << " contains " << are_numbers.size() << " numerical values and " << this->are_categorical(key) << " categorical variables." << std::endl;
    output_txt << "In the column " << key << " there are " << this->count_NaNs(key) << " missing values." << std::endl;
    output_txt << std::endl;
  }

  output_txt << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;


  /* Now we create some unordered maps. For each column containing some numerical data, its mean/median... is
  computed and it is stored in the map means/medians... using as key the name of the column itself.
  This will be useful in the following for loops in order to print the statistical info of the dataset
  with ease. */


  std::unordered_map<std::string, double> means;
  std::unordered_map<std::string, double> medians;
  std::unordered_map<std::string, double> variances;
  std::unordered_map<std::string, double> std_devs;


  for (std::string key : this->column_keys){
    try {
      double mean = this->compute_mean(key);
      double median = this->compute_median(key);
      double variance = this->compute_variance(key);
      double std_dev = this->compute_std_dev(key);

      means[key] = mean;
      medians[key] = median;
      variances[key] = variance;
      std_devs[key] = std_dev;
    }
    catch (std::runtime_error &e) {} /* If we try to compute the mean/median... of a column not holding
    numerical data, we just do not consider that column when writing the summary of the numerical quantity. */
  }


  if (means.size() == 0) {std::cout << "There are no columns holding numerical data." << std::endl;}
  else {
    for (auto i : means){output_txt << "The mean of the numbers present in the column " << i.first << " is " << i.second << std::endl;}
    
    output_txt << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
  
    for (auto i: medians) {output_txt <<"The median of the numbers present in the column " << i.first << " is " << i.second << std::endl;}
    
    output_txt << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
  
    for (auto i: variances) {output_txt <<"The variance of the numbers present in the column " << i.first << " is " << i.second << std::endl;}
    
    output_txt << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
  
    for (auto i: std_devs) {output_txt <<"The standard deviation of the numbers present in the column " << i.first << " is " << i.second << std::endl;}
    
    output_txt << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
  }

  output_txt.close(); // We close the file for safety reasons.

  std::cout << kernel << "Done! (See " << output << ")." << std::endl;
  return "./output_files/" + output;
}

std::vector<std::string> column_from_key(Data_Table& Table, std::string key){        //extracts a column from the table given a key
  std::vector<std::string> column;
  std::vector<std::string> keys = Table.get_column_keys();
  auto iter = std::find(keys.begin(), keys.end(), key);                
  if (iter == keys.end()) {
    throw std::runtime_error("The key is not present");
  }
  for (auto it : Table) {                              //We use the iterators we defined to cycle over the table
    column.push_back(it[key]);
  }
  return column;
}








std::vector<std::string> unique_column(Data_Table& Table, std::string& key){              //extracts the unique values contained in the given column. This is used just to get labels for histograms,
                                                                                          //so the data is not taken in the order it appears in.
  std::unordered_set<std::string> unique_set;
  std::vector<std::string> keys = Table.get_column_keys();
  auto iter = std::find(keys.begin(), keys.end(), key);              //each one of them
  if (iter == keys.end()) {
    throw std::runtime_error("The key is not present");
  }
  for (auto v : column_from_key(Table,key)){                           
    unique_set.insert(v);                                                                 //since we do not care about ordering, we use a set to select the values without repetition
  }
  std::vector<std::string> unique_vector;
  for (auto v: unique_set){                              
    unique_vector.push_back(v);
  }
  return unique_vector;
}

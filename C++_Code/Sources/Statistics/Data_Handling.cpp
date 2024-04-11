#include "../../Includes/Statistics/Data_Handling.hpp"
#include "../../Includes/Statistics/Iterators.hpp"
#include <cctype>
#include <cmath>
#include <numeric>
#include <type_traits>
#include <typeinfo>
#include <exception>



void CSV_Handler::open() {
    this->file.open(this->path+this->name, std::ifstream::in);
    if (file.is_open()) {
      this->open_status = true;
    }
    else {
      this->open_status = false;
      throw std::runtime_error("Could not open file");
    }
  }






unsigned int CSV_Handler::size() {
  std::string line;
  unsigned int counter = 0;
  if (this->open_status) {                                 //if the file is open we increase counter until we get to the end of the file
    while(std::getline(this->file,line)) {
      counter ++;
    }
  }
  else {                                                   //otherwise we try to open it and try to do the same
    this->open();
    if (this->open_status) {                                  
      while(std::getline(this->file,line)) {
	counter ++;
      }
    }
    else {                                                //if opening fails we throw an exception
      this->open_status = false;
      throw std::runtime_error("Could not open file");
    }
  }
  this->file.clear();                                     //now we have getline go back to the beginning of the file. This way we may read it again from the beginnning when using other functions.
  this->file.seekg(0, std::ios::beg);
  char fin = line.back();                                 //since some (but not all!) .csv file lines end with a '\r' char signaling the end of the reading process to allow for nicer key names we remove it
  if (fin == '\r') {
    line.pop_back();
  }
  return counter;
}









std::string CSV_Handler::operator()(const unsigned int line_num) {
    std::string line;
    if (this->open_status) {
      for (unsigned int i = 1; i<line_num; ++i) {std::getline(this->file,line);}         //skip all the lines before the one we are looking for
      std::getline(this->file,line);                                                     //get the line
    }
    else {
      this->open();                                                                     //Here we try to do the same after opening the file
      if (this->open_status) {
        for (unsigned int i = 1; i<line_num; ++i) {std::getline(this->file,line);}
        std::getline(this->file,line);
      }
      else {throw std::runtime_error ("Failed to open the file.");} 
    }
    this->file.clear();                                                                   //This block of code was documented above
    this->file.seekg(0, std::ios::beg);
    char fin = line.back();
    if (fin == '\r') {
      line.pop_back();
    }
    return line;
}





/*This function uses the previous one to access a line and then splits by ',' to access the elemnt affering to the selected key*/

std::variant<double, std::string> CSV_Handler::operator()(unsigned int &line_num, std::string &key) {
  if (this->open_status == false) {
    try {
      this->open();
    }
    catch(std::runtime_error &e) {                                                //if we are unable to open the file we will get a runtime error exception
      std::variant<double, std::string> exit = "nan";
      return exit;
    }
  }
  std::vector<std::string> keys = this->keys();                                   //Here we get a vector containing all the keys. Being sorted by appearence, this allows us to easily determine the index of 
  auto it = std::find(keys.begin(), keys.end(), key);                             //each one of them
  unsigned int index;
  if (it != keys.end()) {
    index = it - keys.begin();
  }
  else {
    std::cerr << "The key is not present" << std::endl;
    std::variant<double, std::string> exit = "nan";
    return exit;
  }
  std::string line = this->operator()(line_num+2);
  std::string token;
  char fin = line.back();
  if (fin == '\r') {
      line.pop_back();
  }
  std::istringstream string_s(line);                                            //We use stringstream to split the line by ','
  for (unsigned int i = 1; i<index+1; ++i) {                                    //We skip elements until we get to the index of the key
    std::getline(string_s,token, ',');
  }
  std::getline(string_s,token,',');
  std::variant<double, std::string> me = token;
  return me;
}







/*This function give a vector containg all the keys*/

std::vector<std::string> CSV_Handler::keys() {
  if (this->open_status == false) {
    try {
      this->open();
    }
    catch(std::runtime_error &e) {                                                //if we are unable to open the file we will get a runtime error exception
      std::vector<std::string> exit = {"nan"};
      return exit;
    }
  }
  std::vector<std::string> keys(0);                              
  std::string token;
  std::istringstream string_s(this->operator()(1));                             //The keys are contained in the first row of the .csv file
  while(std::getline(string_s, token, ',')) {                                   //We split the string by ',' and push each element in a vector
    keys.push_back(token);
  }
  return keys;
}

std::string CSV_Handler::summary_string() {
  std::string status = "closed ";
  if (this->open_status) {
    status = "open ";
  }
  return "You are currently operating on the file: "+this->name+" located at: "+this->path+".\nThe file is currently "+status;
}
  







double exception_stod(const std::string& input) {
  std::size_t pos;
  double cast = stod(input,& pos);                               //this will initialise an iterator (pos) with the position where the conversion stopped
  std::string checker = ""+input[pos];                           //if the string was not completly convertible this will be non empty
  if (checker != "") {throw std::invalid_argument("Error: it is not possible to convert the data properly.");} 
  return cast;
}







/*This implements access by using the container structure of vector of maps*/
std::variant<double, std::string> Data_Table::operator()(const unsigned int& row_number, const std::string &key) const {
  std::variant<double, std::string> content;
  std::unordered_map<std::string, std::string> mp;
  try {                                                        //Here we avoid segmentation faults when trying to access values out of range
    mp = this->Table[row_number];
  }
  catch (std::runtime_error & e) {std::cout << "Error: the indices are out of range" << std::endl;}
  if (mp.at(key) == "") {throw NaNException();}
  try {content = exception_stod(mp.at(key));}
  catch (const std::invalid_argument & e) {content = mp.at(key);}
  return content;
}








Data_Table Convert_file_to_Table(CSV_Handler& CSV_file) {
  if (CSV_file.get_status() == false) {
    CSV_file.open();
  }
  std::vector<std::string> column_keys = CSV_file.keys();               //here we get all the keys
  unsigned int n_rows = 0;
  std::vector<std::unordered_map<std::string, std::string>> Table;
  std::string line;
  while(getline(CSV_file.file, line)) {
    if (n_rows == 0) {                                                    //if we are at the first row, we have nothing to do so we just increase the numer of rows
      n_rows ++;                                                       
    }
    else {
      std::unordered_map<std::string, std::string> mp;
      std::string token;
      unsigned int counter = 0;                                            //this saves the key the element affers to by storing its position in the line 
      char fin = line.back();
      if (fin == '\r') {
	line.pop_back();
      }
      std::istringstream string_s(line);
      while(std::getline(string_s, token, ',')) {                           //Here we split the line by ',' and insert the values in map, thus encoding all the row
	      std::string key = column_keys[counter];
	      mp[key] = token;
	      counter ++;
      }
      Table.push_back(mp);                                                  //Having completed to read the row we may insert the map and pass to the next one
      n_rows ++;                                                            //everytime we move to another line we increase the counter of rows
    }
  }
  n_rows --;
  CSV_file.file.clear();
  CSV_file.file.seekg(0, std::ios::beg);
  Data_Table Table_of_Data = {n_rows, column_keys, Table};
  std::string name_of_file = CSV_file.get_name();
  name_of_file.pop_back();                                                   //The only thing left to do is update the name of the file. To have a nicer looking string we remove the extension via 4 pops
  name_of_file.pop_back();
  name_of_file.pop_back();
  name_of_file.pop_back();
  Table_of_Data.file_name = name_of_file;
  return Table_of_Data;
}

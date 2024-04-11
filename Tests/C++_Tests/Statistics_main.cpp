#include "../../C++_Code/Includes/Statistics/Data_Handling.hpp"
#include "../../C++_Code/Includes/Statistics/Data.hpp"
#include "../../C++_Code/Includes/Statistics/Test_QoL.hpp"
#include <unordered_set>
#include <sstream> 

int main(){
  std::string color = "\033[1;34m";
  std::string end_color = "\033[0m ";
  std::string s = "s";
  std::string kernel_name = "PROJECT";
  std::unordered_set<std::string> acceptable_answers = {"I", "Integration", "S", "Statistics", "Q","Quit", "q", "INTEGRATION", "i", "s", "STATISTICS", "esc", "QUIT", "ESC", "^["};
  std::unordered_set<std::string> integration_answers = {"I", "Integration", "INTEGRATION", "i"};
  std::unordered_set<std::string> statistics_answers =  {"S", "Statistics", "s", "STATISTICS"};
  std::unordered_set<std::string> quitting_answers =  {"Q", "Quit", "q", "QUIT", "esc", "ESC", "^["};
  std::string quit = "";
  while (!quitting_answers.count(quit)) {
    std::string phrase_1 = "What do you wish to do? (S = test Statistics, Q = quit). To get a full documentation of the project type: ? \n";
    quit = ReadMe_choice(acceptable_answers, phrase_1, color, kernel_name, end_color);
    if (integration_answers.count(quit)) {
      std::string color = "\033[1;35m";
      std::string kernel_name = "NUMERICAL_INTEGRATION";
      std::cout << color << kernel_name << ">" << end_color;
      std::cout << "See ya!" << std::endl;
    }
    else if (statistics_answers.count(quit)) {
      std::cout << "\n \n " << std::endl;
      std::cout << "========================================================================================================================================================================" << std::endl;
      std::cout << "\n \n \n \n \n" << std::endl;
      color = "\033[1;33m";
      kernel_name = "STATISTICAL_ANALYSIS";
      std::cout << color << kernel_name << ">" << end_color;
      std::cout << "Would you like to get information about a custom file or would you like to test the code on any one of the files contained in CSV_files? [1/2]" << std::endl;
      std::cin >> s;
      if (s == "1") {
	std::string path_of_the_file = "";
	std::string name_of_the_file = "";
	while (true) {
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Please insert the full path of the file you wish to open. Alternatively you may also specify the relative path starting from the build directory." << std::endl;
	  std::cin >> path_of_the_file;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Please insert the full path of the file you wish to open. Alternatively you may also specify the relative path starting from the build directory." << std::endl;
	  std::cin >> name_of_the_file;
	  std::cout << color << kernel_name << ">" << end_color;
	  CSV_Handler Your_CSV = {path_of_the_file, name_of_the_file, false};
	  try {
	    Your_CSV.open();
	    break;
	  }
	  catch(std::runtime_error &e) {
	    std::cerr << "The file could not be opened. Please make sure you are using the correct path." << std::endl;
	  }
	}
	CSV_Handler Your_CSV = {path_of_the_file, name_of_the_file, true};
	std::string choice = "";
	std::unordered_set<std::string> acceptable_choices = {"y", "Yes", "Sì", "Si", "s", "si", "sì", "yes", "YES", "Y", "n", "No", "NO", "N"};
	std::unordered_set<std::string> affirmative_choices = {"y", "Yes", "Sì", "Si", "s", "si", "sì", "yes", "YES", "Y"};
	std::unordered_set<std::string> negative_choices = {"n", "No", "NO", "N"};
	phrase_1 = "Do you wish to store the informations of the file in a container? [y/n] (Useful if you need to have access quickly to all of the data. If you only need to access specific data, we advise to avoid this choice so as to minimise space complexity)\n" + color + kernel_name + ">" + end_color + "To know more about the data structure we used to store your file's data, please print ?.\n";
	choice = ReadMe_choice(acceptable_choices, phrase_1, color, kernel_name, end_color);
	if (affirmative_choices.count(choice)) {
	  Data_Table Your_Table = Convert_file_to_Table(Your_CSV);
	  phrase_1 = "Would you like do execute a specific action (press ? to see allowed operations) or would you like to get a file summarizing all the relevant informations?" ;
	  acceptable_choices = {"q", "2", "1"};
	  choice = multiple_choice(acceptable_choices, phrase_1, true, true, color, kernel_name, end_color);
	  if (choice == "1") {
	    while (choice != "q") {
	      acceptable_choices = {"q", "quit","access", "mean", "median", "variance", "std_dev", "standard deviation", "standard_deviation", "number of columns", "n_columns", "print keys", "keys", "correlation"};
	      phrase_1 = "What would you like to do? (Pressing ? will print the list of allowed operations).";
	      choice = multiple_choice(acceptable_choices, phrase_1, true, false, color, kernel_name, end_color);
	      if (choice == "mean") {
		acceptable_choices = {Your_Table.get_column_keys()[0]};
		for (unsigned int i = 1; i < Your_Table.get_column_keys().size(); ++i) {
		  acceptable_choices.insert(Your_Table.get_column_keys()[i]);
		}
		phrase_1 = "Which column would you like to compute the mean of? (press ? to get a list of all keys)";
		choice = multiple_choice(acceptable_choices, phrase_1, false, false, color, kernel_name, end_color);
		std::cout << color << kernel_name << ">" << end_color;
		std::cout << "The mean value of the column: " << choice << " is:\n";
		try {
		  std::cout << color << kernel_name << ">" << end_color;
		  std::cout << Your_Table.compute_mean(choice) << std::endl;
		}
		catch(std::runtime_error &e) {
		  std::cout << "not computable since there are no numerical values." << std::endl;
		}
		
	      }
	      else if (choice == "median") {
		acceptable_choices = {Your_Table.get_column_keys()[0]};
		for (unsigned int i = 1; i < Your_Table.get_column_keys().size(); ++i) {
		  acceptable_choices.insert(Your_Table.get_column_keys()[i]);
		}
		phrase_1 = "Which column would you like to compute the median of? (press ? to get a list of all keys)";
		choice = multiple_choice(acceptable_choices, phrase_1, false, false, color, kernel_name, end_color);
		std::cout << color << kernel_name << ">" << end_color;
		std::cout << "The median value of the column: " << choice << " is:\n";
		try {
		  std::cout << color << kernel_name << ">" << end_color;
		  std::cout << Your_Table.compute_median(choice) << std::endl;
		}
		catch(std::runtime_error &e) {
		  std::cout << "not computable since there are no numerical values." << std::endl;
		}
		
	      }
	      else if (choice == "variance") {
		acceptable_choices = {Your_Table.get_column_keys()[0]};
		for (unsigned int i = 1; i < Your_Table.get_column_keys().size(); ++i) {
		  acceptable_choices.insert(Your_Table.get_column_keys()[i]);
		}
		phrase_1 = "Which column would you like to compute the variance of? (press ? to get a list of all keys)";
		choice = multiple_choice(acceptable_choices, phrase_1, false, false, color, kernel_name, end_color);
		std::cout << color << kernel_name << ">" << end_color;
		std::cout << "The variance of the column: " << choice << " is:\n";
		try {
		  std::cout << color << kernel_name << ">" << end_color;
		  std::cout << Your_Table.compute_variance(choice) << std::endl;
		}
		catch(std::runtime_error &e) {
		  std::cout << "not computable since there are no numerical values." << std::endl;
		}
		
	      }
	      if (choice == "std_dev" || choice == "standard deviation" || choice == "standard_deviation") {
		acceptable_choices = {Your_Table.get_column_keys()[0]};
		for (unsigned int i = 1; i < Your_Table.get_column_keys().size(); ++i) {
		  acceptable_choices.insert(Your_Table.get_column_keys()[i]);
		}
		phrase_1 = "Which column would you like to compute the standard deviation of? (press ? to get a list of all keys)";
		choice = multiple_choice(acceptable_choices, phrase_1, false, false, color, kernel_name, end_color);
		std::cout << color << kernel_name << ">" << end_color;
		std::cout << "The standard deviation of the column: " << "choice is:\n";
		try {
		  std::cout << color << kernel_name << ">" << end_color;
		  std::cout << Your_Table.compute_std_dev(choice) << std::endl;
		}
		catch(std::runtime_error &e) {
		  std::cout << "not computable since there are no numerical values." << std::endl;
		}
		
	      }
	      if (choice == "keys" || choice == "print keys") {
		std::cout << color << kernel_name << ">" << end_color << "The keys are: ";
		for (unsigned int i = 0; i < Your_Table.get_column_keys().size(); ++i) {
		  std::cout << Your_Table.get_column_keys()[i] << ", ";
		}
	      }
	      if (choice == "n_rows" || choice == "number of rows") {
		std::cout << color << kernel_name << ">" << end_color << "The file has: " << Your_Table.get_n_rows(); 
	      }
	    
	    }
	  }
	  if (choice == "2" || choice == "summary") {
	    std::string txt_name = Your_Table.summary();
	    std::string summary_file = "cat " + txt_name;
	    acceptable_choices = {"y", "Yes", "Sì", "Si", "s", "si", "sì", "yes", "YES", "Y", "n", "No", "NO", "N"};
	    while (true) {
	      std::cout << color << kernel_name << ">" << end_color;
	      std::cout << "Would you like to print the result?" << std::endl;
	      std::cin >> choice;
	      if (acceptable_choices.count(choice)) {
		break;
	      }
	      else {
		std::cout << color << kernel_name << ">" << end_color;
		std::cout << "please type a valid answer" << std::endl;
	      }
	    }
	    if (affirmative_choices.count(choice)) {
	      system(summary_file.c_str());
	    }
	  }
	
	
	}
      }
      if (s== "2") {
	std::string file_name = "Undefined";
	std::unordered_set<std::string> acceptable_names = {"Stores", "Stores_Data", "Stores Data", "Banking", "Banking_Data", "Banking Data", "Banks", "Housing_Data", "Housing Data", "California_Housing", "California Housing", "Housing", "Housing_2", "Housing_Data_2", "Housing Data 2","SUS_Housing", "Housing 2", "Sus_Housing", "SUS Housing", "US Crime Rates", "US_Crime_Rates"};
	std::unordered_set<std::string> stores_names = {"Stores", "Stores_Data", "Stores Data"};
	std::unordered_set<std::string> bank_names = {"Banking", "Banking_Data", "Banking Data", "Banks"};
	std::unordered_set<std::string> housing_names = {"Housing_Data", "Housing Data", "California_Housing", "California Housing", "Housing"};
	std::unordered_set<std::string> SUS_housing_names = {"Housing_2", "Housing_Data_2", "Housing Data 2","SUS_Housing", "Housing 2", "Sus_Housing", "SUS Housing"};
	std::unordered_set<std::string> US_crime_names = {"US Crime Rates", "US_Crime_Rates"};
	std::unordered_map<std::string, std::unordered_set<std::string>> mp;
	mp["Stores_Data"] = stores_names;
	mp["Banking_Data"] = bank_names;
	mp["Housing_Data"] = housing_names;
	mp["Housing_Data_2"] = SUS_housing_names;
	mp["US_Crime_Rates"] = US_crime_names;
	while (true) {
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Please insert the name of the file you wish to analise. Press ? for a list of accepted names." << std::endl;
	  std::cin>> file_name;
	  if (file_name == "?") {
	    for (auto it:mp) {
	      std::cout << color << kernel_name << ">" << end_color;
	      std::cout << "To operate on the file named: " << it.first << ".csv, you can type any of the following: ";
	      for (std::string acceptable_n : it.second) {
		std::cout << acceptable_n << ", ";
	      }
	      std::cout << "\n";
	    }
	  }
	  else if (acceptable_names.count(file_name)) {
	    break;
	  }
	  else{
	    std::cout << color << kernel_name << ">" << end_color << " please enter a valid file name. Press ? for help." << std::endl;
	  }
	}
	if (stores_names.count(file_name)) {
	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "The file has " << Stores_Data.get_n_rows() << " rows." << std::endl;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "The file has " << Stores_Data.get_column_keys().size() << " columns. Their names are:" << std::endl;
	  for (size_t i = 0; i < Stores_Data.get_column_keys().size(); ++i){
	    if (i == Stores_Data.get_column_keys().size()-1) {std::cout << Stores_Data.get_column_keys()[i] << std::endl;}
	    else {std::cout << Stores_Data.get_column_keys()[i] << ", ";}  
	  }
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "There are " << Stores_Data.count_NaNs() << " missing values in the dataset." << std::endl;

	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

	  // Some examples for testing.
	  std::string shopping_mall = "shopping_mall";
	  std::string cost_price_per_unit = "cost_price_per_unit";
	  std::string selling_price_per_unit = "selling_price_per_unit";
	  std::string age = "age";
	  std::string category = "category";
	  std::string test_age = "43";
	  std::string state = "state";
	  std::string gender = "gender";
	  std::string North_Carolina = "North Carolina";
	  std::string Female = "Female";
	  std::string Kanyon = "Kanyon";

	  unsigned int x_1 = Stores_Data.frequency(age, test_age);
	  unsigned int x_2 = Stores_Data.frequency(state, North_Carolina);
	  unsigned int x_3 = Stores_Data.frequency(gender,Female);
	  unsigned int x_4 = Stores_Data.frequency(shopping_mall, Kanyon);

	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "There are " << x_1  << " owners of age 43, " << percentage(x_1, Stores_Data.get_n_rows()) << " of the total number of owners." << std::endl;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "There are " << x_2 << " owners in North Carolina, "  << percentage(x_2, Stores_Data.get_n_rows()) << " of the total number of shops." << std::endl;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "There are " << x_3 << " female owners, " <<  percentage(x_3, Stores_Data.get_n_rows()) << " of the total number of owners." << std::endl;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "There are " << x_4 << " Kanyon shops, " << percentage(x_4, Stores_Data.get_n_rows()) << " of the total amount." << std::endl;


	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Now we want to test the access." << std::endl;

	  std::unordered_map<std::string, std::string> first_line = Stores_Data.get_Table()[4];
	  std::string entry = first_line[shopping_mall];
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "The element in the 4th row with key " << shopping_mall << " is " << entry << "." << std::endl;

	  unsigned int idx;
	  std::string key;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Please enter a row index: " << std::endl;
	  std::cin >> idx;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Please enter the name of a column: " << std::endl;
	  std::cin >> key;

	  try {
	    std::variant<double, std::string> element = Stores_Data(idx, key);

	    if (std::holds_alternative<std::string>(element)) {
	      std::string s = std::get<std::string>(element);
	      std::cout << color << kernel_name << ">" << end_color;
	      std::cout << "The element in position " << idx << " in the column " << key << " is " << s << "." << std::endl;
	    }

	    if (std::holds_alternative<double>(element)) {
	      double d = std::get<double>(element);
	      std::cout << color << kernel_name << ">" << end_color;
	      std::cout << "The element in position " << idx << " in the column " << key << " is " << d << "." << std::endl;
	    }
	  }
	  catch (const NaNException &e) {
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cerr << "Error: the value is missing." << std::endl;
	  }
	  catch (const std::out_of_range& e) {
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cerr << "Error: indices out of bounds." << std::endl;
	  }


	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Now we will test our iterators. The following are the first 100 shopping malls:" << std::endl;
	  unsigned int counter = 0;
	  for (auto it : Stores_Data) {
	    std::cout << it["shopping_mall"] << ", ";
	    if (counter > 100) {
	      break;
	    }
	    counter ++;
	  }

	  std::cout << "\n";
	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;


	  std::string checker = "";
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Would you like to create a file summarising all the data contained in the file? [y/n]" << std::endl;
	  std::cin >> checker;
	  if (checker == "y") {
	    std::string file_named = Stores_Data.summary();
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "Would you like to print the content of the file you've just created? [y/n]" << std::endl;
	    std::cin >> checker;
	    if (checker == "y") {
	      std::string txt_file_name = "cat "+file_named;
	      system(txt_file_name.c_str());
	    }
	  }


	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;


	  try {
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "The mean of the column " << category << " is: " << Stores_Data.compute_mean(category) << "." << std::endl;
	  }
	  catch (const std::runtime_error &e) {std::cout << "the column holds no numerical data." << std::endl;}

	  try {
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "The mean of the column " << age << " is: " << Stores_Data.compute_mean(age) << "." << std::endl;
	  }
	  catch (std::runtime_error &e) {
	    std::cout << "The mean of the column age could not be computed as there are no numerical values" << std::endl;
	  }
	  try {
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "The median of the column " << age << " is: " << Stores_Data.compute_median(age) << "." << std::endl;
	  }
	  catch (std::runtime_error &e) {
	    std::cout << "The median of the column age could not be computed as there are no numerical values" << std::endl;
	  }
	  try {
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "The variance of the column " << age << " is: " << Stores_Data.compute_variance(age) << "." << std::endl;
	  }
	  catch (std::runtime_error &e) {
	    std::cout << "The variance of the column age could not be computed as there are no numerical values" << std::endl;
	  }
	  try {
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "The standard deviationn of the column " << age << " is: " << Stores_Data.compute_std_dev(age) << "." << std::endl;
	  }
	  catch (std::runtime_error &e) {
	    std::cout << "The standard deviation of the column age could not be computed as there are no numerical values" << std::endl;
	  }
	  try {
	    std::cout << color << kernel_name << ">" << end_color;
	    double correlation = Stores_Data.compute_correlation(cost_price_per_unit, selling_price_per_unit);
	    std::cout << "The correlation between the columns " << cost_price_per_unit << " and " << selling_price_per_unit << " is " << correlation << "." << std::endl;

	  }
	  catch (std::range_error &e) {
	    std::cout << "The correlation of the columns \"cost price per unit\" and \"selling price per unit\" could not be computed as the two have different sizes." << std::endl;
	  }
	  catch (std::runtime_error &e) {
	    std::cout << "The correlation of the columns \"cost price per unit\" and \"selling price per unit\" could not be computed as in one of the two there are no numerical values" << std::endl;
	  }
	  
	  
	  

	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;


	  double max_age = Stores_Data.column_max(age);
	  double min_age = Stores_Data.column_min(age);

	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "The max in the column " << age << " is " << max_age << "." << std::endl;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "The min in the column " << age << " is " << min_age << "." << std::endl;


	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Now we select all the rows containing 43 as age." << std::endl;
	  std::vector<std::unordered_map<std::string, std::string>> age_43 = Stores_Data.select_values(age, test_age);
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "The number of people who are " << test_age << " is (again) " << age_43.size() << "." << std::endl;


	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;


	  unsigned int row_to_drop = 0;
	  Stores_Data.drop_row(row_to_drop);
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "We have dropped a row. Now the number of rows is " << Stores_Data.get_n_rows() << "." << std::endl;

	  Stores_Data.drop_column(state);
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "We have dropped a column. Now the number of columns is " << Stores_Data.get_column_keys().size() << "." << std::endl;

	  Stores_Data.drop_NaNs();
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "We have dropped all the rows containing missing values. Now the number of rows is " << Stores_Data.get_n_rows() << "." << std::endl;

	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	  
	}

        else if (housing_names.count(file_name)) {
	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Now we will test our iterators by printing the first 100 elements of each column." << std::endl;
	  for (std::string key : CA_Housing_Data.get_column_keys()) {
	    unsigned int counter = 0;
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "The first 100 elements of the column: " << key << " is:\n" << std::endl;
	    std::cout << color << kernel_name << ">" << end_color;
	    for (auto it : CA_Housing_Data) {
	      std::cout << it[key] << ", ";
	      if (counter > 100) {
		break;
	      }
	      counter ++;
	    }
	    std::cout << "\n\n" << std::endl;
	  }
	  std::string keyy = "median_house_value";
	  unsigned int idx = 1;
	  std::variant<double, std::string> elements = CA_Housing_Data(idx, keyy);
	  //std::cout << std::get<double>(elements) << "\n";
	  try {
	    std::variant<double, std::string> element = CA_Housing_Data(idx, keyy);

	    if (std::holds_alternative<std::string>(element)) {
	      std::string s = std::get<std::string>(element);
	      std::cout << "The element in position " << idx << " in the column " << keyy << " is " << s << "." << std::endl;
	    }

	    if (std::holds_alternative<double>(element)) {
	      double d = std::get<double>(element);
	      std::cout << "The element in position " << idx << " in the column " << keyy << " is " << d << "." << std::endl;
	    }
	  }
	  catch (const NaNException &e) {std::cerr << "Error: the value is missing." << std::endl;}
	  catch (const std::out_of_range& e) {std::cerr << "Error: indices out of bounds." << std::endl;}
	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;


	  std::string checker = "";
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Would you like to create a file summarising all the data contained in the file? [y/n]" << std::endl;
	  std::cin >> checker;
	  if (checker == "y") {
	    std::string file_named = CA_Housing_Data.summary();
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "Would you like to print the content of the file you've just created? [y/n]" << std::endl;
	    std::cin >> checker;
	    if (checker == "y") {
	      std::string txt_file_name = "cat "+file_named;
	      system(txt_file_name.c_str());
	    }
	  }


  


	  unsigned int row_to_drop = 0;
	  CA_Housing_Data.drop_row(row_to_drop);
	  std::cout << "We have dropped a row. Now the number of rows is " << CA_Housing_Data.get_n_rows() << "." << std::endl;

	  CA_Housing_Data.drop_NaNs();
	  std::cout << "We have dropped all the rows containing missing values. Now the number of rows is " << CA_Housing_Data.get_n_rows() << "." << std::endl;


	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	}
	else if (bank_names.count(file_name)) {
	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Now we will test our iterators by printing the first 100 elements of each column." << std::endl;
	  for (std::string key : Banking_Data.get_column_keys()) {
	    unsigned int counter = 0;
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "The first 100 elements of the column: " << key << " is:\n" << std::endl;
	    std::cout << color << kernel_name << ">" << end_color;
	    for (auto it : Banking_Data) {
	      std::cout << it[key] << ", ";
	      if (counter > 100) {
		break;
	      }
	      counter ++;
	    }
	    std::cout << "\n\n" << std::endl;
	  }
	  

	  std::string checker = "";
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Would you like to create a file summarising all the data contained in the file? [y/n]" << std::endl;
	  std::cin >> checker;
	  if (checker == "y") {
	    std::string file_named = Banking_Data.summary();
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "Would you like to print the content of the file you've just created? [y/n]" << std::endl;
	    std::cin >> checker;
	    if (checker == "y") {
	      std::string txt_file_name = "cat "+file_named;
	      system(txt_file_name.c_str());
	    }
	  }

  


	  unsigned int row_to_drop = 0;
	  Banking_Data.drop_row(row_to_drop);
	  std::cout << "We have dropped a row. Now the number of rows is " << Banking_Data.get_n_rows() << "." << std::endl;

	  Banking_Data.drop_NaNs();
	  std::cout << "We have dropped all the rows containing missing values. Now the number of rows is " << Banking_Data.get_n_rows() << "." << std::endl;


	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	}
	else if (SUS_housing_names.count(file_name)) {
	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Now we will test our iterators by printing the first 100 elements of each column." << std::endl;
	  for (std::string key : SUS_Housing_Data.get_column_keys()) {
	    unsigned int counter = 0;
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "The first 100 elements of the column: " << key << " is:\n" << std::endl;
	    std::cout << color << kernel_name << ">" << end_color;
	    for (auto it : SUS_Housing_Data) {
	      std::cout << it[key] << ", ";
	      if (counter > 100) {
		break;
	      }
	      counter ++;
	    }
	    std::cout << "\n\n" << std::endl;
	  }

	  
	  std::string checker = "";
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Would you like to create a file summarising all the data contained in the file? [y/n]" << std::endl;
	  std::cin >> checker;
	  if (checker == "y") {
	    std::string file_named = SUS_Housing_Data.summary();
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "Would you like to print the content of the file you've just created? [y/n]" << std::endl;
	    std::cin >> checker;
	    if (checker == "y") {
	      std::string txt_file_name = "cat "+file_named;
	      system(txt_file_name.c_str());
	    }
	  }
	    


  


	  unsigned int row_to_drop = 0;
	  SUS_Housing_Data.drop_row(row_to_drop);
	  std::cout << "We have dropped a row. Now the number of rows is " << SUS_Housing_Data.get_n_rows() << "." << std::endl;

	  SUS_Housing_Data.drop_NaNs();
	  std::cout << "We have dropped all the rows containing missing values. Now the number of rows is " << SUS_Housing_Data.get_n_rows() << "." << std::endl;


	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	}
	else if (US_crime_names.count(file_name)) {
	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	  std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Now we will test our iterators by printing the first 100 elements of each column." << std::endl;
	  for (std::string key : Crime_Data.get_column_keys()) {
	    unsigned int counter = 0;
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "The first 100 elements of the column: " << key << " is:\n" << std::endl;
	    std::cout << color << kernel_name << ">" << end_color;
	    for (auto it : Crime_Data) {
	      std::cout << it[key] << ", ";
	      if (counter > 100) {
		break;
	      }
	      counter ++;
	    }
	    std::cout << "\n\n" << std::endl;
	  }


	 std::string checker = "";
	 std::cout << color << kernel_name << ">" << end_color;
	  std::cout << "Would you like to create a file summarising all the data contained in the file? [y/n]" << std::endl;
	  std::cin >> checker;
	  if (checker == "y") {
	    std::string file_named = Crime_Data.summary();
	    std::cout << color << kernel_name << ">" << end_color;
	    std::cout << "Would you like to print the content of the file you've just created? [y/n]" << std::endl;
	    std::cin >> checker;
	    if (checker == "y") {
	      std::string txt_file_name = "cat "+file_named;
	      system(txt_file_name.c_str());
	    }
	  }


	  std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	}
      }
    }
    std::cout << "\n \n \n" << std::endl;
    std::cout << "========================================================================================================================================================================" << std::endl;
    std::cout << "\n \n \n" << std::endl;
    kernel_name = "PROJECT";
    color = "\033[1;34m";
  }
  return 0;
}

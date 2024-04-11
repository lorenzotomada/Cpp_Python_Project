#ifndef Data_Hpp
#define Data_Hpp

#include <iostream>
#include <string>
#include "Data_Handling.hpp"

std::string default_path = "../Tests/CSV_files/";


std::string Stores = "Stores_Data.csv";
std::string California_Housing = "Housing_Data.csv";
std::string SUS_Housing = "Housing_Data_2.csv";
std::string Crime = "US_Crime_Rates.csv";
std::string Banking = "Bank_Data.csv";



CSV_Handler Stores_CSV = {default_path, Stores, false};
Data_Table Stores_Data = Convert_file_to_Table(Stores_CSV);

CSV_Handler CA_Housing_CSV = {default_path, California_Housing, false};
Data_Table CA_Housing_Data = Convert_file_to_Table(CA_Housing_CSV);

CSV_Handler SUS_Housing_CSV = {default_path, SUS_Housing, false};
Data_Table SUS_Housing_Data = Convert_file_to_Table(SUS_Housing_CSV);

CSV_Handler Banking_CSV = {default_path, Banking, false};
Data_Table Banking_Data = Convert_file_to_Table(Banking_CSV);

CSV_Handler Crime_CSV = {default_path, Crime, true};
Data_Table Crime_Data = Convert_file_to_Table(Crime_CSV);


#endif

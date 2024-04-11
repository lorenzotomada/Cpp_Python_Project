#ifndef Testing_Quality_of_Life_Hpp
#define Testing_Quality_of_Life_Hpp


#include <iostream>
#include <string>
#include <unordered_set>



std::string multiple_choice(std::unordered_set<std::string> acceptable_choices, std::string phrase_1, bool quit, bool shortcuts, std::string color = "", std::string kernel_name = "", std::string end_color = "" ) {
  std::string choice;
  while (true) {
    std::cout << color << kernel_name << ">" << end_color;
    std::cout << phrase_1 << std::endl;
    std::cin >> choice;
    if (acceptable_choices.count(choice)) {
      break;
    }
    else if (choice != "?") {
      std::cout << color << kernel_name << ">" << end_color;
      std::cout << "please type a valid answer" << std::endl;
    }
    else if (choice == "?") {
      std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
      if (quit && ! shortcuts) {
	std::cout << color << kernel_name << ">" << end_color << "The following are all the possible operations: ";
	for (auto scelta : acceptable_choices) {
	  if (scelta != "q" && scelta != "quit") {
	    std::cout << scelta << ", ";
	  }
	  else if (scelta == "q" || scelta == "quit") {
	    std::cout << "\n" << color << kernel_name << ">" << end_color << "By pressing " << scelta << " you may get back to the main menu.\n";
	  }
		
	}
      }
      else if (shortcuts) {
	std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << color << kernel_name << ">" << end_color << "The following are all the possible operations: ";
	for (auto scelta : acceptable_choices) {
	  if (scelta != "1" && scelta != "2" && scelta != "q" && scelta != "quit") {
	    std::cout << scelta << ", ";
	  }
	  else if (scelta == "1") {
	    std::cout << "\n" << color << kernel_name << ">" << end_color << "By pressing 1 you access a submenu where you may choose an operation.\n";
	  }
	  else if (scelta == "2") {
	    std::cout << "\n" << color << kernel_name << ">" << end_color << "Pressing 2 is a shortcut to get a file summarising all of the information. The same can be achieved by pressing \"summary\".\n";
	  }
	  else if (scelta == "q" || scelta == "quit") {
	    std::cout << "\n" << color << kernel_name << ">" << end_color << "By pressing " << scelta << " you may get back to the main menu.\n";
	  }
		
	}
	std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
      }
      else {
	std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	for (auto scelta : acceptable_choices) {
	  std::cout << scelta << ", ";
	}
	std::cout << "\n";
		
      }
    }
    
    
  }
  return choice;
}





std::string ReadMe_choice(std::unordered_set<std::string> acceptable_choices, std::string phrase_1, std::string color = "", std::string kernel_name = "", std::string end_color = "" ) {
  std::string choice;
  while (true) {
    std::cout << color << kernel_name << ">" << end_color;
    std::cout << phrase_1 << std::endl;
    std::cin >> choice;
    if (acceptable_choices.count(choice)) {
      break;
    }
    else if (choice == "?") {
      std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
      std::cout << "\n \n " << std::endl;
      system("cat ../ReadMe.txt");
      std::cout << "\n \n " << std::endl;
      std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	
    }
    else {
      std::cout << color << kernel_name << ">" << end_color;
      std::cout << "please type a valid answer" << std::endl;
    }
    std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
    
  }
  return choice;
}


#endif
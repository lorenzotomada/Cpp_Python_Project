cmake_minimum_required(VERSION 3.12)


project(Homework03 VERSION 0.1
                  DESCRIPTION "A toolbox for scientific computing"
                  LANGUAGES CXX)



set(CMAKE_CXX_STANDARD 17) # https://matgomes.com/setting-cpp-standard-cmake/
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic -O3)



find_package(pybind11 REQUIRED)
include_directories(SYSTEM ${pybind11_INCLUDE_DIRS})



set(ALL_INCLUDES "./C++_Code/Includes/Statistics/Data_Handling.hpp;./C++_Code/Includes/Statistics/Iterators.hpp;./C++_Code/Includes/Integration/Numerical_Integration.hpp;./C++_Code/Includes/Statistics/Data.hpp;./C++_Code/Includes/Statistics/Test_QoL.hpp;./C++_Code/Includes/Integration/Functions.hpp")
set(SRCS "./C++_Code/Sources/Statistics/Data_Handling.cpp;./C++_Code/Sources/Statistics/Statistics.cpp;./C++_Code/Sources/Integration/Numerical_Integration.cpp")

set(PYBIND_INT_LIB_SRCS "./C++_Code/Sources/Integration/Numerical_Integration.cpp;./C++_Code/Bindings/Numerical_Integration_py.cpp")
set(PYBIND_STAT_LIB_SRCS "./C++_Code/Sources/Statistics/Data_Handling.cpp;./C++_Code/Sources/Statistics/Statistics.cpp;./C++_Code/Bindings/Statistics_py.cpp")

set(STATISTICS_SRCS "./C++_Code/Sources/Statistics/Data_Handling.cpp;./C++_Code/Sources/Statistics/Statistics.cpp")
set(STATISTICS_INCLUDES "./C++_Code/Includes/Statistics/Data_Handling.hpp;./C++_Code/Includes/Statistics/Iterators.hpp;./C++_Code/Includes/Statistics/Test_QoL.hpp")

set(INTEGRATION_SRCS "./C++_Code/Sources/Integration/Numerical_Integration.cpp")
set(INTEGRATION_INCLUDES "./C++_Code/Includes/Integration/Numerical_Integration.hpp;./C++_Code/Includes/Integration/Functions.hpp")



option(STATISTICS "Compile the statistics part" OFF)
option(INTEGRATION "Compile the numerical integration part" OFF)
  


if(STATISTICS)
  message("Creating a shared library named Statistics and the python bindings called statistical_analysis.")
  message("An executable file named Statistics_Test will be created where the statistics part can be tested.")

  add_library(Statistics SHARED ${SRCS} ${STATISTICS_INCLUDES})

  pybind11_add_module(statistical_analysis ${PYBIND_STAT_LIB_SRCS} ${STATISTICS_INCLUDES})

  add_executable(Statistics_Test ./Tests/C++_Tests/Statistics_main.cpp ${STATISTICS_SRCS} ${STATISTICS_INCLUDES})
  target_link_libraries(Statistics_Test PRIVATE Statistics)

endif()



if(INTEGRATION)
  message("Creating a shared library named Integration and the python bindings called integration.")
  message("An executable file named Integration_Test will be created where the integration part can be tested.")
  
  find_package(GSL REQUIRED)
  include_directories(${GSL_INCLUDE_DIRS})
  
  find_package(Boost REQUIRED)
  include_directories(${Boost_INCLUDE_DIRS})
  
  add_library(Integration SHARED ${INTEGRATION_LIB_SRCS} ${INTEGRATION_INCLUDES})
  target_link_libraries(Integration PRIVATE ${GSL_LIBRARIES} ${Boost_LIBRARIES})
  set_target_properties(Integration PROPERTIES LINKER_LANGUAGE CXX)

  pybind11_add_module(integration ${PYBIND_INT_LIB_SRCS} ${INTEGRATION_INCLUDES})
  target_link_libraries(integration PRIVATE ${GSL_LIBRARIES})

  add_executable(Integration_Test ./Tests/C++_Tests/Integration_main.cpp ${INTEGRATION_SRCS} ${INTEGRATION_INCLUDES})
  target_link_libraries(Integration_Test Integration ${GSL_LIBRARIES} ${Boost_LIBRARIES})

endif()



if(NOT STATISTICS AND NOT INTEGRATION)
  message("Creating a shared library named Stat_and_Int and the python bindings called, respectively, statistical_analysis and integration.")
  message("An executable file named Project_Test will be created where all the C++ code can be tested.")

  find_package(GSL REQUIRED)
  include_directories(${GSL_INCLUDE_DIRS})

  find_package(Boost REQUIRED)
  include_directories(${Boost_INCLUDE_DIRS})

  add_library(Stat_and_Int SHARED ${SRCS} ${ALL_INCLUDES})
  target_link_libraries(Stat_and_Int PRIVATE ${GSL_LIBRARIES})

  pybind11_add_module(statistical_analysis ${PYBIND_STAT_LIB_SRCS} ${STATISTICS_INCLUDES})

  pybind11_add_module(integration ${PYBIND_INT_LIB_SRCS} ${INTEGRATION_INCLUDES})
  target_link_libraries(integration PRIVATE ${GSL_LIBRARIES})

  add_executable(Project_Test ./Tests/C++_Tests/Project_main.cpp ${SRCS} ${ALL_INCLUDES})
  target_link_libraries(Project_Test PRIVATE Stat_and_Int ${GSL_LIBRARIES} ${Boost_LIBRARIES})

endif()
import statistical_analysis as sa
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os
import seaborn as sns
import sys



def panda_to_Table(Data_Structure):
        Data_Structure.to_csv("temp.csv")
        csv_handler = sa.CSV_Handler("./", "temp.csv", True)
        self_data_table = sa.Convert_file_to_Table(csv_handler)
        os.remove("temp.csv")
        return self_data_table




def CSV_to_Table(name, path="./"):
        return sa.Convert_file_to_Table(sa.CSV_Handler(path,name,True))




def CSV_to_panda(name, path="./"):
        return pd.read_csv(path+name)




def histogram(Data_Table, key,Legend=False):
    """
    Generate a histogram or countplot using Seaborn for a given key in a dataset.

    Parameters
    ----------
    Data_Table : statistical_analysis.Data_Table
        The Data set encoded in the binded class
    key : str
        The key or column in the dataset for which the histogram is generated.
    Legend : bool, optional
        Whether to show the legend in the plot (default is False).

    Returns
    -------
    None
        Displays the generated histogram using Seaborn.

    Notes
    -----
    The function prompts the user if they wish to distinguish values through another key ('Hue').
    If 'Hue' is chosen, the function creates a countplot with a hue.
    If not, it creates a countplot with the original key for hue

    The color palette used is "magma," and the number of unique values in the specified key is considered for setting colors.
    """

    q = sa.column_from_key(Data_Table,key)

    try:
        q=[float(a) for a in q]
        q.sort()
    except:
        pass

    sns.set()

    Hue = key

    print("Do you wish to distinguish the values through another key? [y/n](e.g. if the chosen key = \"region\", we may decide to also distinguish which city the data comes from.)", flush = True)
    
    # in order to accept an input, see https://blog.finxter.com/read-from-stdin-in-python/

    x = sys.stdin.readline().strip()
    
    if x=="y":
        print("Please insert the key", flush = True)
        Hue = sys.stdin.readline().strip()
        df = pd.DataFrame({key: q, Hue: sa.column_from_key(Data_Table, Hue)})

        sns.countplot(data = df,hue = Hue, dodge = False, palette = sns.color_palette("magma", len(sa.unique_column(Data_Table,Hue))), x=key)

        if Legend is not None:
            plt.legend(title=key)

        else:
            pass
          
        plt.show()

    
    else:
        u = len(sa.unique_column(Data_Table,key))
        colours = sns.color_palette("magma", u)
        df = pd.DataFrame({key: q})

        sns.countplot(data = df, palette = colours, hue = key, dodge = False, x=key)

        if Legend is not None:
            plt.legend(title=key)

        else:
            pass
        
        plt.show()




def scatterplot(Data_Table, key_1, key_2):
    """
    Create a scatterplot using Seaborn for two keys in a dataset.

    Parameters
    ----------
     Data_Table : statistical_analysis.Data_Table
        The Data set encoded in the binded class
    key_1 : str
        The key representing the x-axis values.
    key_2 : str
        The key representing the y-axis values.

    Returns
    -------
    None
        Displays the generated scatterplot using Seaborn.

    Notes
    -----
    The function prompts the user to enter another numerical key encoding the size of the dots.
    Optionally, the user can control the colors using a different key.
    """

    try:
        # Extract numerical values for the x-axis (key_1) and y-axis (key_2)
        x_1 = [float(a) for a in sa.column_from_key(Data_Table, key_1)]
        u_1 = len(x_1)
        x_2 = [float(a) for a in sa.column_from_key(Data_Table, key_2)]
        u_2 = len(x_2)

        # Check if the two selected keys have the same amount of data
        if u_1 != u_2:
            print("The two selected keys have different amounts of data")
            return

        print("Do you wish to enter another numerical key encoding the size of the dots? [y/n]", flush = True)
        x = sys.stdin.readline().strip()

        df = 0

        if x == "y":
            print("Please enter the key", flush = True)
            key_3 = sys.stdin.readline().strip()

            print("Do you want the colors to be controlled by a different key? [y/n]", flush = True)
            print("If not, it will be controlled by the key you've just inserted")
            x = sys.stdin.readline().strip()

            if x == "y":
                print("Please enter the key", flush = True)
                key_4 = sys.stdin.readline().strip()
                df = pd.DataFrame({key_1: x_1, key_2: x_2, key_3: [float(a) for a in sa.column_from_key(Data_Table, key_3)], key_4: sa.column_from_key(Data_Table, key_4)})
            else:
                key_4 = key_3
                df = pd.DataFrame({key_1: x_1, key_2: x_2, key_3: [float(a) for a in sa.column_from_key(Data_Table, key_3)]})

            # Create scatterplot with size and hue options
            sns.set()
            sns.scatterplot(data = df, x=key_1, y=key_2, hue=key_4, size=key_3, sizes=(20, 750), legend="full")
            plt.show()

        else:
            print("Do you want the colors to be controlled by a different key? [y/n]")
            print("If not, it will be controlled by the second key you've inserted", flush = True)
            x = sys.stdin.readline().strip()

            if x == "y":
                print("Please enter the key", flush = True)
                key_4 = sys.stdin.readline().strip()
                df = pd.DataFrame({key_1: x_1, key_2: x_2, key_4: sa.column_from_key(Data_Table, key_4)})
            else:
                key_4 = key_2
                df = pd.DataFrame({key_1: x_1, key_2: x_2})

            # Create scatterplot with hue option
            sns.set()
            sns.scatterplot(df, x=key_1, y=key_2, hue=key_4)
            plt.show()

    except:
        print("Please make sure the data indexed by the keys you chose contains numerical values")





# Here Data_Table has no underscore in order to differentiate it from the C++ class




class DataTable:
    """
    Represents a data table with various operations and properties.

    Parameters
    ----------
    file_path : str
        The path to the file containing the data.
    file_name : str
        The name of the file containing the data.

    Attributes
    ----------
    file_path : str
        The path to the file containing the data.
    file_name : str
        The name of the file containing the data.
    cpp_backend : statistical_analysis.Data_Table
        The data table represented using a C++ backend.
    column_keys : list
        List of column keys in the data table.
    n_rows : int
        Number of rows in the data table.

    dataframe: pandas.DataFrame
        The data may also be optionally stores in a pandas DataFrame. This is done by calling the property dataframe

    Methods
    -------
    __str__():
        Returns a string representation of the data table.
    __repr__():
        Returns a representation of the data table.
    __call__(row_number, key):
        Returns the value at a specific row and column key.
    __eq__(other):
        Checks if two DataTable objects are equal.
    __ne__(other):
        Checks if two DataTable objects are not equal.
    drop_column(column_to_drop):
        Drops a specified column from the data table.
    drop_row(row_to_drop):
        Drops a specified row from the data table.
    drop_NaNs():
        Drops rows with NaN values from the data table.
    count_NaNs(column=None):
        Counts NaN values in the entire data table or a specific column.
    are_numbers(column):
        Checks if values in a specified column are numbers.
    are_categorical(column):
        Checks if values in a specified column are categorical.
    __getite__(key):
        Gives the column corresponding to the  given key
    classification(row, column):
        Classifies a value in a specified row and column.
    frequency(key, value):
        Computes the frequency of a value in a specified column.
    column_max(key):
        Computes the maximum value in a specified column.
    column_min(key):
        Computes the minimum value in a specified column.
    compute_mean(key=None, *args, **kwargs):
        Computes the mean of a specified column or the entire data table.
    compute_median(key=None, *args, **kwargs):
        Computes the median of a specified column or the entire data table.
    compute_variance(key=None, *args, **kwargs):
        Computes the variance of a specified column or the entire data table.
    compute_std_dev(key=None, *args, **kwargs):
        Computes the standard deviation of a specified column or the entire data table.
    compute_correlation(key1, key2):
        Computes the correlation between two specified columns.
    summary():
        Provides a summary of the data table using C++ backend.
    describe():
        Describes the data table using Pandas DataFrame.
    head():
        Returns the first few rows of the data table using Pandas DataFrame.
    tail():
        Returns the last few rows of the data table using Pandas DataFrame.

    histogram(key)
    Generate a histogram or countplot using Seaborn for a given key in a dataset.

    scatterplot(key_1,key_2)
    Create a scatterplot using Seaborn for two keys in a dataset.
    """


    def __init__(self, file_path, file_name):
        """
        Initialize DataTable with file_path and file_name.

        Parameters
        ----------
        file_path : str
        The path to the file containing the data.
        file_name : str
        The name of the file containing the data.
        """

        self.file_name = file_name
        self.file_path = file_path



    @property
    def cpp_backend(self):
        """
        Property to access the C++ backend representation of the data table.
        """

        csv_handler = sa.CSV_Handler(self.file_path, self.file_name, True)
        data_table = sa.Convert_file_to_Table(csv_handler)
        return data_table



    @property
    def column_keys(self):
        """
        Property to access the list of column keys in the data table.
        """

        return self.cpp_backend.column_keys



    @property
    def n_rows(self):
        """
        Property to access the number of rows in the data table.
        """

        return self.cpp_backend.n_rows



    @property
    def dataframe(self):
        """
        Property to access the data through pandas
        """

        return CSV_to_panda(self.name,self.path)



    def __str__(self):
        """
        String representation of the data table.
        """

        return CSV_to_panda(self.file_name, self.file_path).__str__()



    def __repr__(self):
        """
        Representation of the data table.
        """

        return CSV_to_panda(self.file_name, self.file_path).__repr__()



    def __call__(self, row_number, key):
        """
        Call method to get the value at a specific row and column key.

         Parameters
        ----------
        row_number : int
        The row in which the object we are looking for is located
        key : str
        The name of the key the object we are looking for affers to
        """
        return self.cpp_backend.__call__(row_number, key)



    def __eq__(self, other):
        """
        Equality comparison between two DataTable objects.

        
        """

        return (self.file_name == other.file_name and self.file_path == other.file_path) or self.cpp_backend == other.cpp_backend



    def __ne__(self, other):
        """
        Inequality comparison between two DataTable objects.
        """

        return not (self == other)



    def __bool__(self):
        """
        Boolean evaluation of DataTable object.
        """

        return self.file_name is not None and self.file_path is not None



    def drop_column(self, key_to_drop):
        """
        Drop a specified column from the data table.

        Parameters
        ----------
  
        key_to_drop : str
        The name of the key we wish to remove
        """

        return self.cpp_backend.drop_column(key_to_drop)



    def drop_row(self, row_to_drop):
        """
        Drop a specified row from the data table.

        Parameters
        ----------
  
        row_to_drop : int
        The name of the row we wish to remove
        """

        return self.cpp_backend.drop_row(row_to_drop)



    def drop_NaNs(self):
        """
        Drop rows with NaN values from the data table.
        """

        return self.cpp_backend.drop_NaNs()



    def count_NaNs(self, key=None):
        """
        Count NaN values in the entire data table or a specific column.

        Parameters
        ----------
  
        key : str
        The name of the key the object we are looking for affers to
        """

        if key is None:
            return self.cpp_backend.count_NaNs()
        else:
            return self.cpp_backend.count_NaNs(key)



    def are_numbers(self, key):
        """
        Gives a vector containing all the numerical values affering to the given key.

        Parameters
        ----------
  
        key : str
        The name of the key the object we are looking for affers to
        """

        return self.cpp_backend.are_numbers(key)



    def are_categorical(self, key):
        """
        Gives a vector containing all the categorical values affering to the given key.

        Parameters
        ----------
  
        key : str
        The name of the key the object we are looking for affers to
        """

        return self.cpp_backend.are_categorical(key)



    def __getitem__(self, key):
        """
        Select values from the data table based on a condition.

        Parameters
        ----------
  
        key : str
        The name of the key the object we are looking for affers to
        """

        return sa.column_from_key(self.cpp_backend,key)



    def classification(self, row, key):
        """
        Classify a value in a specified row and column.

        Parameters
        ----------
        row : int
        The row in which the object we are looking for is located
        key : str
        The name of the key the object we are looking for affers to
        """

        return self.cpp_backend.classification(row, key)



    def frequency(self, key, value):
        """
        Compute the frequency of a value in a specified column.

        Parameters
        ----------
        value : str
        The value of which we want to compute the frequency of
        key : str
        The name of the key the object we are looking for affers to
        """

        return self.cpp_backend.frequency(key, value)



    def column_max(self, key):
        """
        Compute the maximum value in a specified column.

        Parameters
        ----------

        key : str
        The name of the key the object we are looking for affers to
        """

        return self.cpp_backend.column_max(key)



    def column_min(self, key):
        """
        Compute the minimum value in a specified column.
        """

        return self.cpp_backend.column_min(key)



    def compute_mean(self, key=None, *args, **kwargs):
        """
        Compute the mean of a specified column or the entire data table.

        Parameters
        ----------
        row_number : int
        The row in which the object we are looking for is located
        key : str
        The name of the key the object we are looking for affers toParameters
        ----------

        key : str
        The name of the key of whose objects we want to compute the std_dev of

        *args
        Accepts lists of arguments (optional, settings for the pandas function)

        **kwargs
        Accepts dictionaries of arguments (optional, settings for the pandas function)
        """

        if key is None:
            return self.dataframe.mean(*args, **kwargs) 
        
        return self.cpp_backend.compute_mean(key)



    def compute_median(self, key=None, *args, **kwargs):
        """
        Compute the median of a specified column or the entire data table.

        Parameters
        ----------

        key : str
        The name of the key of whose objects we want to compute the std_dev of

        *args
        Accepts lists of arguments (optional, settings for the pandas function)

        **kwargs
        Accepts dictionaries of arguments (optional, settings for the pandas function)
        """

        if key is None:
            return self.dataframe.median()
        
        return self.cpp_backend.compute_median(key)




    def compute_variance(self, key=None, *args, **kwargs):
        """
        Compute the variance of a specified column or the entire data table.

        Parameters
        ----------

        key : str
        The name of the key of whose objects we want to compute the std_dev of

        *args
        Accepts lists of arguments (optional, settings for the pandas function)

        **kwargs
        Accepts dictionaries of arguments (optional, settings for the pandas function)
        """

        if key is None:
            return self.dataframe.var(*args, **kwargs)
        
        return self.cpp_backend.compute_variance(key)




    def compute_std_dev(self, key=None, *args, **kwargs):
        """
        Compute the standard deviation of a specified column or the entire data table.

        Parameters
        ----------

        key : str
        The name of the key of whose objects we want to compute the std_dev of

        *args
        Accepts lists of arguments (optional, settings for the pandas function)

        **kwargs
        Accepts dictionaries of arguments (optional, settings for the pandas function)

        """

        if key is None:
            return self.dataframe.std(*args, **kwargs)
        
        return self.cpp_backend.compute_std_dev(key)




    def compute_correlation(self, key1, key2):
        """
        Compute the correlation between two specified columns.

        Parameters
        ----------
        key_1 : str
        The name of the key the first object affers to
        key_2 : str
        The name of the key the second object affers to
        """

        return self.cpp_backend.compute_correlation(key1, key2)




    def summary(self):
        """
        Provide a summary of the data table using C++ backend
        
        """

        return self.cpp_backend.summary()




    def describe(self):
        """
        Describe the data table using Pandas DataFrame.
        """

        return self.dataframe().describe()




    def head(self):
        """
        Return the first few rows of the data table using Pandas DataFrame.
        """

        return self.dataframe.head()



    def tail(self):
        """
        Return the last few rows of the data table using Pandas DataFrame.
        """

        return self.dataframe.tail()



    def histogram(self,key, Legend):
        """
         Generate a histogram or countplot using Seaborn for a given key in a dataset.

        Parameters
        ----------

        key : str
            The key or column in the dataset for which the histogram is generated.
        Legend : bool, optional
            Whether to show the legend in the plot (default is False).

        Returns
        -------
        None
            Displays the generated histogram using Seaborn.

        Notes
        -----
        See the histogram function for more precise documentation
        """

        histogram(self.cpp_backend, key, Legend)



    def scatterplot(self,key_1, key_2):
        """
        Create a scatterplot using Seaborn for two keys in a dataset.

        Parameters
        ----------

        key_1 : str
            The key representing the x-axis values.
        key_2 : str
            The key representing the y-axis values.

        Returns
        -------
        None
            Displays the generated scatterplot using Seaborn.

        Notes
        -----
        See the scatterplot function for more precise documentation
        """

        scatterplot(self.cpp_backend, key_1, key_2)
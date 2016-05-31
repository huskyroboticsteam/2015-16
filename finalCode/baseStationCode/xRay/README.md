# Husky Robotics Team XRF Spectroscopy Tool
## Last updated by Aakash Sethi
### Email: asethi77@cs.washington.edu
### Phone: 425-780-5274

This folder contains tools used to read sample data from an AmpTek X-Ray detector and identify elements contained within a sample.

## Required Software

In order to use the tools here, you must have the following already installed on your computer:

* [Anaconda](https://www.continuum.io/downloads)
    * Anaconda is a Python programming environment that has tools for plotting and numerical analysis built-in. I
      *highly* recommend using this to install all the packages you will need to run this tool, but specifically,
      you need to have NumPy, SciPy, and MatPlotLib. If there is any issue with installing Anaconda, contact me
      either by phone or email (listed above) and I will help you get the packages installed manually. **Note for
      installation: You should get two check boxes to select when installing: one about adding Python to the PATH,
      and another about making Python 3.5.1 the default interpreter. Make sure to select both check boxes.**
      
* [MinGW](http://www.mingw.org/)
    * MinGW is a tool used to compile C++ programs on Windows. **Note for Brandon: This tool should already be installed on
      your computer, so you don't have to worry about installing and setting this up.**
      
* Command Prompt
    * You can use Windows' built-in command prompt (just press Windows Key + R, then type in `cmd` in the prompt that shows up
      to run the command prompt). I personally recommend Git Bash (**Note for Brandon: This should already be installed on your
      computer**) because you can just open a terminal through Windows Explorer instead of manually navigating within the terminal.
      
## Downloading the X-Ray Code

To download all of the 2015-2016 code, [click this link](https://github.com/huskyroboticsteam/2015-16/archive/master.zip),
and extract the ZIP file to any location. You should have a 2015-2016 directory, which has finalCode -> baseStationCode -> xRay.
This directory has all the files you need in order to run the x-ray detector tools.

## Compiling and Running 

After collecting all data through the x-ray detector (a process which I will not describe here), you should have a .MCA file
that contains counts of samples detected per x-ray channel/energy emission level. There are two steps to using this tool:

1. Converting from an MCA file to a CSV file for analysis
2. Analyzing the CSV file via the python XRFSpectroscopy tool

### Converting from an MCA file to a CSV file

1. Within the xRay folder, navigate to the `mca_to_csv` subdirectory.
2. In Windows Explorer, click "File->Git Bash Here" to open a terminal in this directory.
3. Type `mingw32-make` to compile the code to convert MCA to CSV files.
4. You should now have a program called `mca_to_csv.exe`. Simply double-click to run
   the program, which should open a little console.
5. The program will prompt you for a file name. At this point, it is NOT looking for
   the name of the MCA file, but instead looks for a text file with the names of the
   MCA files you want to convert. I have added a demo `FileList.txt`, which contains
   the name of an MCA file; edit FileList.txt with the names of the MCA files you
   want to convert, then type in FileList.txt into the prompt and you should see
   new CSV files created in the directory with the same name as the MCA file they
   were created from.
   
### Analyzing the CSV file via the XRFSpectroscopy tool
1. Copy the CSV file(s) you created from the step above into the `xrf_analysis` directory
   (also within the xRay folder).
2. Navigate to the xrf_analysis directory within Windows Explorer if you are not already there,
   then again open a terminal using the "File->Git Bash Here" menu option.
3. For any given CSV file you want to analyze, type in `py -3 <Name of CSV File>` to execute the
   analysis program. **Note: If your CSV file name has spaces in it, you must surround the filename
   in quotes when using the above command**.
   
More details to come on the tool itself.
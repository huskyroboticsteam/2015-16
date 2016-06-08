# Makefile for AmpTek XRF detector code. To compile the code in this directory, you can now simply
# run a makefile tool.
#
# NOTE: This makefile has been tested on a Windows machine running a 32-bit MinGW build.
# In order to compile this program using MinGW, navigate to the directory containing all XRF code
# in the command prompt and run "mingw32-make". This command will change depending on which make
# tool you use (e.g. using the Visual C++ tools it may be Nmake, and on Linux/Mac OSX machines you
# would just use 'make'.)
#
# Last modified by Aakash Sethi (asethi77@cs.washington.edu) on May 7, 2016.

CC=g++

mca_to_csv: AmpTekFormatCSVMain.cpp spectrum_data borehole_read snip upper_trim amptek_read fit
	$(CC) -o mca_to_csv.exe AmpTekFormatCSVMain.cpp spectrum_data.o borehole.o snip.o trim.o amptek_read.o fit.o
	
amptek_read: AmpTekRead.cpp
	$(CC) -c -o amptek_read.o AmpTekRead.cpp

spectrum_data: SpectrumData.cpp
	$(CC) -c -o spectrum_data.o SpectrumData.cpp
	
borehole_read: borehole_read.cpp
	$(CC) -c -o borehole.o borehole_read.cpp
	
snip: snip.cpp
	$(CC) -c -o snip.o snip.cpp
	
upper_trim: upper_trim.cpp
	$(CC) -c -o trim.o upper_trim.cpp
	
fit: Fit.cpp
	$(CC) -c -o fit.o fit.cpp
	
clean:
	del *.o *.exe
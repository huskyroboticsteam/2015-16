import csv
import sys
import math
import numpy
import matplotlib.pyplot as pyplot
import scipy.signal
from matplotlib.widgets import Cursor

energy_kev = []
counts = []

def load_xrf_data(filename):
    print(filename)
    csv_file = open(filename, 'r')
    file_reader = csv.reader(csv_file)
    next(file_reader)
    next(file_reader)
    
    global energy_kev
    global counts

    for row in file_reader:
        energy_kev.append(float(row[0]))
        counts.append(int(row[1]))
        
    energy_kev = numpy.array(energy_kev)
    counts = numpy.array(counts)

def nearest_datapoint(x):
    energy = (numpy.abs(energy_kev - x)).argmin()
    return (energy_kev[energy], counts[energy], energy)
    
def nearest_maximum(energy):
    window_size = 4

    energy_len = len(energy_kev)
    min_energy = energy - window_size if energy > window_size else 0
    max_energy = energy + window_size if energy < energy_len - window_size else energy_len
    
    peak_counts_idx = min_energy
    for cur_energy in range(min_energy, max_energy):
        if counts[cur_energy] > counts[peak_counts_idx]:
            peak_counts_idx = cur_energy
            
    return (energy_kev[peak_counts_idx], counts[peak_counts_idx])

def onclick(event):
    if (event.button == 3):
        kev, counts, idx = nearest_datapoint(event.xdata)
        
        kev, counts = nearest_maximum(idx)
        
        print('nearest peak: %f, %d' % (kev, counts))

def main():
    counts_smoothed = scipy.signal.savgol_filter(counts, 11, 3)

    fig = pyplot.figure(figsize=(8, 6))
    ax = fig.add_subplot(111, axisbg='#FFFFFF')
    
    ax.plot(energy_kev, counts, marker="o", ms=2, mew=0, linestyle="None", color="red")
    pyplot.plot(energy_kev, counts_smoothed, color="blue")
    
    cid = fig.canvas.mpl_connect('button_press_event', onclick)
    pyplot.show()

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print('USAGE: py -3 XRFSpectroscopy.py <XRF CSV file>')
        exit(1)

    load_xrf_data(sys.argv[1])
    main()
    
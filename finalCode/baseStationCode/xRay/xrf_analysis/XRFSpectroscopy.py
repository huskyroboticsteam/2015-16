import csv
import sys
import math
import numpy
import matplotlib.pyplot as pyplot
import scipy.signal
from XRFDataTable import XRFDataTable

calibration_line_b = -0.282
calibration_line_m = 2.4097

energy_kev = []
counts = []
sigma = []
background_noise = []
peak_bounds = []

# State variables used to keep track of what peak the user
# is currently selecting within the GUI.
current_state = 'INITIALIZING'
xray_left_bound = -1
xray_right_bound = -1
curr_left_bound = -1
curr_right_bound = -1

# Load spectroscopy data from a CSV file and store them as arrays of
# emission energy levels in keV and the counts of each energy level,
# respectively.
def load_xrf_data(filename):
    csv_file = open(filename, 'r')
    file_reader = csv.reader(csv_file)
    next(file_reader)
    next(file_reader)
    
    global energy_kev
    global counts

    for row in file_reader:
        energy_kev.append(float(row[0]))
        counts.append(int(row[1]))
        sigma.append(float(row[2]))
        background_noise.append(float(row[3]))
        
    energy_kev = numpy.array(energy_kev)
    counts = numpy.array(counts)

def nearest_datapoint(x):
    energy = (numpy.abs(energy_kev - x)).argmin()
    return (energy_kev[energy], counts[energy], energy)
    
def nearest_minimum(energy):
    window_size = 4

    energy_len = len(energy_kev)
    min_energy = energy - window_size if energy > window_size else 0
    max_energy = energy + window_size if energy < energy_len - window_size else energy_len
    
    peak_counts_idx = min_energy
    for cur_energy in range(min_energy, max_energy):
        if counts[cur_energy] < counts[peak_counts_idx]:
            peak_counts_idx = cur_energy
            
    return (energy_kev[peak_counts_idx], counts[peak_counts_idx])
    
def get_peak_properties(peak_left, peak_right):
    global counts
    global background_noise
    
    peak_center = (peak_left + peak_right) / 2
    peak_max = peak_left
    peak_area = 0
    for channel in range(peak_left, peak_right + 1):
        peak_area += (counts[channel] - background_noise[channel])
        if counts[channel] > counts[peak_max]:
            peak_max = channel
            
    return (peak_area, peak_max, peak_center)

# Click handler that enables users to select edges of peaks within the x-ray plot.
# Tracks peak edges selected, which are used to compute abundance of elements and
# identify the elements themselves.
def onclick(event):
    # This is gross, but it does the job for now.
    global current_state
    global peak_bounds
    global curr_left_bound
    global curr_right_bound
    global xray_left_bound
    global xray_right_bound
    
    # Simple state machine implementation to handle right-clicking points on the x-ray plot.
    if (event.button == 3):
        kev, counts, idx = nearest_datapoint(event.xdata)
        if current_state == 'XRAY_SOURCE_LEFT_BOUND':
            kev, counts = nearest_minimum(idx)
            xray_left_bound = idx
            print('Selected left bound of source peak with energy %f keV, %d counts' % (kev, counts))
            current_state = 'XRAY_SOURCE_RIGHT_BOUND'
            print('Select right bound of source peak')
        
        elif current_state == 'XRAY_SOURCE_RIGHT_BOUND':
            kev, counts = nearest_minimum(idx)
            xray_right_bound = idx
            print('Selected right bound of source peak with energy %f keV, %d counts' % (kev, counts))
            current_state = 'SELECTING_LEFT_BOUND'
            print('\nSelect the edges of peaks starting with the left bound of the first peak (right click to select)')
            
        elif current_state == 'SELECTING_LEFT_BOUND':
            kev, counts = nearest_minimum(idx)
            curr_left_bound = idx
            print('Selected left bound with energy %f keV, %d counts' % (kev, counts))
            current_state = 'SELECTING_RIGHT_BOUND'
            print('Select right bound of peak')
            
        elif current_state == 'SELECTING_RIGHT_BOUND':
            kev, counts = nearest_minimum(idx)
            curr_right_bound = idx
            peak_bounds.append((curr_left_bound, curr_right_bound))
            curr_left_bound = -1
            curr_right_bound = -1
            print('Selected right bound with energy %f keV, %d counts' % (kev, counts))
            
            selection = ' '
            while selection not in ['y', 'n']:
                if selection != '\n':
                    print('Do you want to select more peaks? (Y/N)')
                selection = sys.stdin.read(1).lower()
                
            if selection == 'y':
                current_state = 'SELECTING_LEFT_BOUND'
                print('\nSelect left bound of peak')
            else:
                current_state = 'PROMPT_DONE'
                pyplot.close()

def search_by_emission_level(table, emission_level):
    results = table.lookup(emission_level)
    if results is None:
        print('   No potential matches for peak found.')
    else:
        print('   Possible element matches for peak:')
        for entry, error in results:
            print('      Element Name: %s (Atomic # %d)    Emission level (keV): %f    Shell Type: %s    Error: %f' % \
                    (entry.elem_name, entry.atomic_number, entry.emission_level, entry.shell, error))

def main():
    global current_state
    global xray_left_bound
    global xray_right_bound
    xrf_table = XRFDataTable('EmissionsTableFiltered.csv')
    
    counts_smoothed = scipy.signal.savgol_filter(counts, 11, 3)

    fig = pyplot.figure()
    ax = fig.add_subplot(111, axisbg='#FFFFFF')
    
    ax.plot(energy_kev, counts, marker="o", ms=2, mew=0, linestyle="None", color="red")
    pyplot.plot(energy_kev, counts_smoothed, color="blue")
    cid = fig.canvas.mpl_connect('button_press_event', onclick)
    
    current_state = 'XRAY_SOURCE_LEFT_BOUND'
    print('Select the left bound of the x-ray source peak (right click to select)')
    
    pyplot.show()
    
    source_area, source_center, source_max = get_peak_properties(xray_left_bound, xray_right_bound)
    print('\nSource peak: Centered around %f keV with area %d counts' % (energy_kev[source_center], source_area))
    
    peak_num = 1
    # search_by_emission_level(xrf_table, energy_kev[source_center])
    print('\nSelected peaks:')
    for peak_left, peak_right in peak_bounds:
        area, center, pmax = get_peak_properties(peak_left, peak_right)
        area_ratio = area / source_area
        print('%d. Peak centered around %f keV with area %f counts and source:area ratio of %f' % \
              (peak_num, energy_kev[pmax], area, area_ratio))
        print('   %% Abundance of material: %f' % (calibration_line_m * area_ratio + calibration_line_b))
        search_by_emission_level(xrf_table, energy_kev[center])
        peak_num += 1

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print('USAGE: python XRFSpectroscopy.py "<XRF CSV file>"')
        exit(1)

    load_xrf_data(sys.argv[1])
    main()
    
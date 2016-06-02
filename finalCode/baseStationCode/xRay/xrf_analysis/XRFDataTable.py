import csv

class EmissionTableEntry:
    def __init__(self, emission_level, shell, atomic_number, elem_name):
        self.emission_level = emission_level
        self.shell = shell
        self.atomic_number = atomic_number
        self.elem_name = elem_name

class XRFDataTable:
    def __init__(self, fname):
        self.emission_level_sorted = []
        self.emission_level_hashtable = {}
        self.element_name_hashtable = {}
        self.file_name = fname
        self.__LOOKUP_THRESHOLD_START = 0.01
        
        csv_file = open(self.file_name, 'r')
        file_reader = csv.reader(csv_file)
        next(file_reader)

        for row in file_reader:
            # Set up the emission level hash table first
            emission_level = float(row[0])
            self.emission_level_sorted.append(emission_level)
            self.emission_level_hashtable[emission_level] = \
                    EmissionTableEntry(emission_level, row[1], int(row[2]), row[3])
        
        print(self.emission_level_sorted[0:20])
    
    # Lookup helper using approximate binary search method with parameterized approximate threshold
    def __lookup(self, emission_energy, start, end, threshold):
        if start > end:
            return None
        else:
            mid = int((start + end) / 2)
            cur_energy = self.emission_level_sorted[mid]
            if abs(cur_energy - emission_energy) <= threshold:
                return self.emission_level_hashtable[cur_energy]
            elif cur_energy < emission_energy:
                return self.__lookup(emission_energy, mid, end, threshold)
            else:
                return self.__lookup(emission_energy, start, mid - 1, threshold)
                    
    # Lookup emission entries that approximately match
    # the target emission_energy. Returns the top 3 closest matches.
    def lookup(self, emission_energy):
        result = self.__lookup(emission_energy, 0, len(self.emission_level_sorted), self.__LOOKUP_THRESHOLD_START)
        if result is None:
            return self.__lookup(emission_energy, 0, len(self.emission_level_sorted), self.__LOOKUP_THRESHOLD_START * 2)
        else:
            return result
        
if __name__ == '__main__':
    table = XRFDataTable('EmissionsTableFiltered.csv')
    print(table.lookup(0.5113).shell)
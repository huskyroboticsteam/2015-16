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
            if emission_level not in self.emission_level_hashtable:
                self.emission_level_hashtable[emission_level] = []
            self.emission_level_hashtable[emission_level].append(\
                    EmissionTableEntry(emission_level, row[1], int(row[2]), row[3]))
    
    # Lookup helper using approximate binary search method with parameterized threshold
    def __lookup(self, emission_energy, start, end, threshold):
        if start > end:
            return None
        else:
            mid = int((start + end) / 2)
            cur_energy = self.emission_level_sorted[mid]
            if abs(cur_energy - emission_energy) <= threshold:
                # Found match. Return all matches that are within the threshold, sorted by error
                # ascending.
                matches = []
                for table_entry in self.emission_level_hashtable[cur_energy]:
                        matches.append((table_entry, abs(cur_energy - emission_energy)))

                cur_idx = mid - 1
                while cur_idx >= 0 and abs(self.emission_level_sorted[cur_idx] - emission_energy) <= threshold:
                    cur_energy = self.emission_level_sorted[cur_idx]
                    for table_entry in self.emission_level_hashtable[cur_energy]:
                        matches.append((table_entry, abs(cur_energy - emission_energy)))
                    cur_idx -= 1
                
                cur_idx = mid + 1
                while cur_idx < len(self.emission_level_sorted) and \
                        abs(self.emission_level_sorted[cur_idx] - emission_energy) <= threshold:
                    cur_energy = self.emission_level_sorted[cur_idx]
                    for table_entry in self.emission_level_hashtable[cur_energy]:
                        matches.append((table_entry, abs(cur_energy - emission_energy)))
                    cur_idx += 1
                
                # Do this to remove duplicates where shells with the same emission energy level
                # are "found" multiple times
                return sorted(list(set(matches)), key = lambda result: result[1])
            elif cur_energy < emission_energy:
                return self.__lookup(emission_energy, mid, end, threshold)
            else:
                return self.__lookup(emission_energy, start, mid - 1, threshold)
                    
    # Lookup emission entries that approximately match
    # the target emission_energy. Returns all matches within the threshold,
    # sorted by closest match to farthest match.
    def lookup(self, emission_energy):
        result = self.__lookup(emission_energy, 0, len(self.emission_level_sorted), self.__LOOKUP_THRESHOLD_START)
        if result is None:
            return self.__lookup(emission_energy, 0, len(self.emission_level_sorted), self.__LOOKUP_THRESHOLD_START * 2)
        else:
            return result
        
if __name__ == '__main__':
    table = XRFDataTable('EmissionsTableFiltered.csv')
    results = table.lookup(0.512)
    
    print('Results of lookup for emission level 0.5113 keV:')
    for entry, error in results:
        print("Element Name: %s (Atomic # %d)    Emission level (keV): %f    Shell Type: %s    Error: %f" % \
                (entry.elem_name, entry.atomic_number, entry.emission_level, entry.shell, error))
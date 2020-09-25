# M Hentz, edited by JYAP 2018

from __future__ import print_function

import numpy as np
import sys
import os
from operator import add

class DataPoint:

    def __init__(self, xbin, ybin, zbin, val):
        self.xbin = xbin
        self.ybin = ybin
        self.zbin = zbin
        self.val = val

    # returning xbin, etc. from other avoids problem where bins remain set to 0
    def __add__(self, other):
        newVal = self.val + other.val
        return DataPoint(other.xbin, other.ybin, other.zbin, newVal)

    def __str__(self):
        return str(self.xbin) + ' ' + str(self.ybin) + ' ' + str(self.zbin) + ' ' + str(self.val)

dir_out = 'combined/data/'
if not os.path.exists(dir_out):
    os.makedirs(dir_out)

print("Save all file names in list...")

# list of output file names
outputs = []

# populate list of file names from scoring file
outputs_src = open('score_dump.mac')

for line in outputs_src.readlines():
    if line[0] != "#":
        line = line.split()
        outputs.append(line[3])

outputs_src.close()

print("Done. " + str(np.size(outputs)) + " files to merge...")

print("Start merging files...")

# merge individual output files for each simulation
nSimulations = 200
nfile = 0

for filename in outputs:
    nfile += 1
    
    string = "  ({0:02d}/{1:d}) {2:s}".format(nfile, np.size(outputs), filename)
    sys.stdout.write(string)
    sys.stdout.flush()
        
    w = open('combined/data/' + filename, 'w')

    allDataPointsList = []

    count = 0
    first = True
    scientific = False
    for i in xrange(1, nSimulations + 1):
        if i%10 == 0:
            sys.stdout.write(".")
            sys.stdout.flush()


        outputFile = open('sims/' + str(i) + '/' + filename)

        dataPointsList = []

        for line in outputFile:

            if line[0] == "#" and count < 3:
                w.write(line)
                count += 1

            if line[0] != "#":
                line = line.split(',')

                xbin_i = float(line[0])
                ybin_i = float(line[1])
                zbin_i = float(line[2])
                val_i = float(line[3])

                # check for scientific notation
                if first and float(line[3]) != 0:
                    first = False
                    if "e" in line[3]:
                        scientific = True

                dp = DataPoint(xbin_i, ybin_i, zbin_i, val_i)
                dataPointsList.append(dp)

        allDataPointsList.append(dataPointsList)

    total_list = []

    # fill total list with empty DataPoints so map() allows addition
    for i in xrange(np.size(allDataPointsList[0])):
        dp_empty = DataPoint(0, 0, 0, 0)
        total_list.append(dp_empty)

    # add all lists element-wise
    for i in xrange(nSimulations):
        total_list = map(add, total_list, allDataPointsList[i])

    # set format
    if scientific:
        format_mod = "e"
    else:
        format_mod = "f"

    # write total to new file
    for dp in total_list:

        line = '{0:.0f},{1:.0f},{2:.0f},{3:.15' + format_mod + '}\n'
        line_fmt = line.format(dp.xbin, dp.ybin, dp.zbin, dp.val)

        w.write(line_fmt)
    w.close()

#sys.stdout.write(" Done")
#sys.stdout.flush()
    print(" Done")

print("All files merged.")
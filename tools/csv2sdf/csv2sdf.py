#
#  Copyright (c) 2019 Matthew Evans
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in all
#  copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE.
#

#
#  csv2sdf.py
#  Script for converting csv files to binary data files
#  For use with the WTEngine
#
#  Read in each column from the CSV file
#  Store the timer in long long int format
#  Store the remaining items as null terminated strings
#

import sys
import csv
import os.path
import struct

print("Convert CSV file to SDF file for use with wip engine")

#  Set variables to store the filename of the input & output files
if len(sys.argv) > 1:
    infilename = sys.argv[1]
else:
    infilename = input("Enter CSV filename:  ")

outfilename = os.path.splitext(infilename)[0] + ".sdf"

#  Open the input & output files, converting into binary format
print("Opening", infilename, "for conversion")

outfile = open(outfilename, "wb")

with open(infilename, newline='') as csvfile:
    linereader = csv.reader(csvfile, delimiter=',')
    for row in linereader:
        outfile.write(struct.pack('q', int(row[0])))    #  Write the timer interval
        outdata = row[1] + "\x00"
        outfile.write(outdata.encode("utf-8"))          #  Write the system name
        outdata = row[2] + "\x00"
        outfile.write(outdata.encode("utf-8"))          #  Write the command name
        outdata = row[3] + "\x00"
        outfile.write(outdata.encode("utf-8"))          #  Write the parameters

print("Wrote file", outfilename)

#  Clean-up
outfile.close()
csvfile.close()

print("Done!")
#  End script

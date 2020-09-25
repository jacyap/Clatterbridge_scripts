# This script concatenates the output from a given number of simulations
# into one file. Call this script in the base directory of the simulation, ie
# the directory containing the sims directory.
# Usage:
#  ./combine_all.sh
# ------------------------------------------------------------------------------
# Author: 07/07/2017, M. Hentz
# ------------------------------------------------------------------------------

# -----------------------------------
#									#
#         combine_edep.sh			#
#									#
# -----------------------------------
# Modified:
#   22/10/18, J. Yap
#
# This script concatenates the energy deposited per layer for a given number of simulations
# into one file. Call this in the directory of the simulation, works in the same way and is 
# adapted from ./ combine_all.sh
# ------------------------------------------------------------------------------
# ------------------------------------------------------------------------------
#!/bin/bash


# Get total number of simulations
nsimulations=$(echo sims/* | wc -w)

file=(edep.txt)

# Iterate over the output files in first directory
for file in edep.txt;

do
    # Construct output file name and save path to it
    output="edep.txt"
    write_to_path=combined/${output}

    # Boolean used to read in header from first file and write to output file
    first=1

    # Iterate through the directory and write files to combined output file
    for i in $(ls -v sims)
    do
        # If first file
        if [[ ${first} -eq 1 ]]
        then
            # Write the header of the first file to the output file
            # Currently this is the top line in the output, prepended with "#"
            head -n 1 combined/${output} > ${write_to_path}
            first=0
        fi
	      path_to_file=sims/${i}/data/${output}
	      echo -ne " Collating output files... ${i}/${nsimulations}\r"

        # Write all lines except the first to the combined file
        # Assuming a one-line header
	      tail -n +2 ${path_to_file} >> ${write_to_path}
    done

echo " Collating output files... ${nsimulations}/${nsimulations}"

done

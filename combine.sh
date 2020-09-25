# This script concatenates the output from a given number of simulations
# into one file. It could be extended to automatically combine all output
# by iterating over an array of positions rather than passing a single
# tracking positon.
# Usage:
#  ./combine.sh
# ------------------------------------------------------------------------------
# Author: 07/07/2017, M. Hentz
# Modified:
#   6/8/18, J. Yap
# ------------------------------------------------------------------------------
#!/bin/bash

# Prompt user for tracking position to be combined
read -p 'Tracking pos: ' tracking_pos
echo

# Construct output file name and save path to it
output="psf_z${tracking_pos}.txt"
write_to_path=$(pwd)/${output}

# Write the header of the first file to the output file
# Currently this is the top line in the output, prepended with "#"
head -n 1 sims/1/data/${output} > ${write_to_path}

# Get total number of simulations
nsimulations=$(echo sims/* | wc -w)

# Iterate through the directory and write files to combined output file
for i in $(ls -v sims); do
	path_to_file=sims/${i}/data/${output}
	echo -ne " Collating output files... ${i}/${nsimulations}\r"

  # Write all lines except the first to the combined file
  # Assuming a one-line header
	tail -n +3 ${path_to_file} >> ${write_to_path}
done

echo " Collating output files... ${nsimulations}/${nsimulations}"

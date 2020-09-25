#J YAP 22 Oct 2018
# Script to extract the eDep for each layer

import os
from time import sleep
import sys
import numpy as np
from decimal import Decimal

n=0
maxfilenumb=40
output = []

#always create new file, overwrite old
outputf = open("BPsim.txt","w+")

while n<maxfilenumb:
    
    for filename in os.listdir("/"):
        filename = open("edep.txt","r")    
        outputf = open("BPsim.txt","a")        
        count = 0       
    
    #for each line    
        for line in filename.readlines():            
            currentline = line.split()                    
            #name = str(currentline[1])
            layer_i = int(currentline[0])
            singleEdep_i = float(currentline[1])           
            energydep=format(count, '.8f')
            
            if layer_i == n:
                count += singleEdep_i                                   
    
    #layer number, summed eDep
    output=[n,energydep]    
    
    #remove square brackets
    s = ", ".join(map(str, output))
    outputf.write(s+'\n')                    
        
    n += 1
    outputf.close() 
                 
outputf.close()
         
print(" Done")


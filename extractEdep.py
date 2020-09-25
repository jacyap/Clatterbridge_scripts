#J YAP 11 Sept 2018
# Script to extract the eDep for each z slice

import os
from time import sleep
import sys
import numpy as np
from decimal import Decimal

n=0
maxfilenumb=40
output = []

#always create new file, overwrite old
outputf = open("eDeps.txt","w+")

while n<maxfilenumb:
    
    for filename in os.listdir("/"):
        filename = open("psf_z" +str(n)+ ".txt", "r")
        
    outputf = open("eDeps.txt","a")        
    count = 0       
    
    #for each line    
    for line in filename:            
        currentline = line.split(",")                    
        layer_i = float(currentline[4])
        singleEdep_i = float(currentline[8])           
        count += singleEdep_i                                   
                
    layer=format(layer_i, '.2f')
    #int(layer_i)
    #convert to 8dp
    energydep=format(count, '.8f')
        
    #print(layer_i)
    #print(count)
    output=[layer,energydep]    
    
    #remove square brackets
    s = ", ".join(map(str, output))
    outputf.write(s+'\n')                    
        
    n += 1
    #print(n) 
    outputf.close() 
        
outputf.close()
         
print(" Done")


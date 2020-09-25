#J YAP 11 Sept 2018
# Script to extract the eDep for each z slice

import os
from time import sleep
import sys
import numpy as np
from decimal import Decimal

n=0
maxfilenumb=1
output = []
m=676

#always create new file, overwrite old
outputf = open("ProfXY"+str(m)+".txt","w+")

while n<maxfilenumb:
    
    for filename in os.listdir("/"):
        filename = open("psf_z"+str(m)+".txt", "r")
        #filename = open("psf.txt", "r")
        
    outputf = open("ProfXY"+str(m)+".txt","a")        
    count = 0       
    
    #for each line    
    for line in filename:            
        currentline = line.split(",")                    
        x_i = float(currentline[2])
        y_i = float(currentline[3])           
        x=format(x_i, '.8f')
        y=format(y_i, '.8f')
                             
        output=[x,y]    
                
        #remove square brackets
        s = ", ".join(map(str, output))
        outputf.write(s+'\n')   
        count += 1                 
        
    n += 1
    #print(n) 
    outputf.close() 
        
outputf.close()
         
print(" Done")


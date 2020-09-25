# Jacinta Yap, 2018
# To merge tracking files for single z distance

import os
from time import sleep
import sys


#type here selected Z distance
z=676

dir_out = 'combined/data/'
if not os.path.exists(dir_out):
    os.makedirs(dir_out)

startf = open("sims/1/data/output_z"+str(z)+".txt","r")
contentsf=startf.read()
outputf = open("combined/data/output_z"+str(z)+".txt","w+")
#contentsf.format('{0:1d},{1:.10f},{2:.5f},{3:.1f},{4:.5f},{5:.5f}')
#contentsf1="{0:1d} {1:.10f} {2:.5f} {3:.1f} {4:.5f} {5:.5f}".format(contentsf)
outputf.write(contentsf)


nSims=200
i=2

for i in range(i,nSims+1) :
    secondf= open("sims/"+str(i)+"/data/output_z"+str(z)+".txt","r")
    nextcontents=secondf.read()
    nextcontents = nextcontents.split('\n',2)[2]
    #nextcontents.format("%.1d","%.7f",'{2:.5f},{3:.1f},{4:.5f},{5:.5f}')
    #nextcontents.format('{0:1d} {1:.10f} {2:.5f} {3:.1f} {4:.5f} {5:.5f}')
    outputf = open("combined/data/output_z"+str(z)+".txt","a")
    outputf.write(nextcontents)
    i+1

if i == nSims:
    outputf.close()

sys.stdout.write("Combining the output files for z="+str(z)+" ...\n")

for i in range(21):
    sys.stdout.write('\r')
    sys.stdout.write("[%-20s] %d%%" % ('='*i, 5*i))
    sys.stdout.flush()
    sleep(0.25)
print(" Done")

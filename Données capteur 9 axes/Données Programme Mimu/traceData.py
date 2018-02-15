#!/usr/bin/python3

import sys
import numpy as np
import matplotlib.pyplot as plt

#if len(sys.argv) != 2:
 #   print("Bad number of argument")
  #  sys.exit(0)

#dataFileName = sys.argv[1]
AccX = []
AccY = []
AccZ = []
GyroX = []
GyroY = []
GyroZ = []
MagnetoX = []
MagnetoY = []
MagnetoZ = []
file = open("Avec perturbation.txt", "r") 
for line in file:
    if line != "\n":
        lineClean = line.replace("\n", "")
        data = lineClean.split("\t")
        Acc = data[0].split(":")[1].split(",")[0:3]
        AccX = AccX + [Acc[0]]
        AccY = AccY + [Acc[1]]
        AccZ = AccZ + [Acc[2]]
        Gyro = data[0].split(":")[1].split(",")[0:3]
        GyroX = GyroX + [Gyro[0]]
        GyroY = GyroY + [Gyro[1]]
        GyroZ = GyroZ + [Gyro[2]]
        Magneto = data[0].split(":")[1].split(",")[0:3]
        MagnetoX = MagnetoX +[Magneto[0]]
        MagnetoY = MagnetoY +[Magneto[1]]
        MagnetoZ = MagnetoZ +[Magneto[2]]


plt.plot(range(len(AccX)), AccX, label='Acc X')

plt.plot(range(len(AccY)), AccY, label='Acc Y')

plt.show()

plt.plot(range(len(GyroX)), GyroX, label='Gyro X')

plt.plot(range(len(GyroY)), GyroY, label='Gyro Y')

plt.show()

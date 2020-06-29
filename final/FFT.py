import time
import matplotlib.pyplot as plt
import numpy as np
import serial
serdev = '/dev/ttyUSB0'
s = serial.Serial(serdev, 9600)
x=[]

for i in range(30):
    line=s.readline()
    x.append(line)

# draw
t = np.arange(0,30,1)
plt.plot(t,x)
plt.show()
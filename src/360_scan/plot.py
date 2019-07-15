import matplotlib.pyplot as plt
import decimal
import numpy as np

#Part 2
#Creates diagram of the 360 degree scan

r = np.power(np.array([30.43, 30.33, 30.81, 31.03, 31.47, 31.59, 32.26, 32.79, 54.41, 53.50, 54.55, 54.36, 53.41, 53.26, 48.12, 42.93, 41.74, 41.83, 40.72, 40.05, 39.90, 39.29, 39.09, 38.83, 38.81, 38.33, 38.24, 38.10, 38.21, 38.64, 38.33, 38.74, 38.98, 39.53, 39.74, 40.40, 41.41, 42.16, 43.71, 52.53, 51.36, 28.71, 31.60, 26.69, 25.93, 24.41, 23.72, 23.00, 22.31, 22.10, 21.76, 21.34, 21.62, 21.14, 21.05, 20.53, 20.60, 21.05, 21.10, 21.17, 21.31, 21.83, 22.43, 22.53, 23.62, 24.26, 25.00, 48.76, 47.22, 48.52, 46.14, 44.12, 42.95, 42.22, 42.41, 41.74, 41.90, 41.40, 40.83, 40.31, 40.19, 39.71, 39.59, 39.34, 39.41, 39.88, 40.02, 40.09, 40.69, 41.33, 41.60, 41.93, 43.47, 44.90, 48.48, 48.84, 50.64, 50.26, 50.05, 49.59, 34.12, 33.43, 32.79, 32.14, 31.55, 31.48, 30.84, 30.74]),1.5)

size = len(r)

theta = np.linspace(0,np.pi*2,size)

plt.plot(r*np.cos(theta), r*np.sin(theta))
plt.show()
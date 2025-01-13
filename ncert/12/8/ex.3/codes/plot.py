import numpy as np
import matplotlib.pyplot as plt
import os
from ctypes import *

# Get the directory where the current code is located
script_dir = os.path.dirname(os.path.abspath(__file__))
# path we want to save this figure 
save_directory = os.path.join(script_dir, "../fig")

if not os.path.exists(save_directory):
    os.makedirs(save_directory)  # creates directory if not exists

solve = CDLL('./test.so')  # loads all the functions that are in test.c
solve.area.restype = c_double  # return datatype of generate_xpoints


x_parab = np.linspace(-3,3,400)
y_parab = x_parab**2
plt.plot(x_parab,y_parab,color='red',label='y= x**2')
x_line = np.linspace(-3,3,400)
y_line = np.full_like(x_line, 4)
plt.plot(x_line,y_line,color='blue',label='y=4')
plt.fill_between(x_parab, y_parab, y_line, where=(y_parab <= y_line), color='gray', alpha=0.5, label='Shaded Area')

a =-2
b =2
n = 100
h = (b-a)/n

def f(x):
    return 4 - x**2

area = (h/2)*(f(a)+f(b))
for i in range (1,n):
    area += h * solve.area(c_double(a),c_int(i),c_double(h))

print(f"area = {area}")
plt.legend()
plt.axis('equal')
plt.grid()
save_path = os.path.join(save_directory, 'fig.jpg')  # saves as combined_fig.jpg
plt.savefig(save_path)
plt.show()

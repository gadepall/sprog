import ctypes
import numpy as np
import matplotlib.pyplot as plt

# Load the shared library
lib = ctypes.CDLL('./code.so')

# Define function prototypes
lib.f1.argtypes = [ctypes.c_double]
lib.f1.restype = ctypes.c_double
lib.f1_prime.argtypes = [ctypes.c_double]
lib.f1_prime.restype = ctypes.c_double
lib.f2.argtypes = [ctypes.c_double]
lib.f2.restype = ctypes.c_double
lib.f2_prime.argtypes = [ctypes.c_double]
lib.f2_prime.restype = ctypes.c_double

lib.newton.argtypes = [ctypes.c_double, ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double),
                       ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double), ctypes.POINTER(ctypes.c_double)]
lib.newton.restype = ctypes.c_double

# Wrapping C functions
def f1(x):
    return lib.f1(x)

def f1_prime(x):
    return lib.f1_prime(x)

def f2(x):
    return lib.f2(x)

def f2_prime(x):
    return lib.f2_prime(x)

def newton(x0, f, f_prime):
    iterations = (ctypes.c_double * 5)()
    f_c = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)(f)
    f_prime_c = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)(f_prime)
    root = lib.newton(x0, f_c, f_prime_c, iterations)
    return root, [iterations[i] for i in range(5)]

# Plotting the functions and Newton iterations
x1_vals = np.linspace(-3.1, 0.1, 500)
y1_vals = [f1(x) for x in x1_vals]
x2_vals = np.linspace(-0.1, 3.1, 500)
y2_vals = [f2(x) for x in x2_vals]

root1_neg3, iterations1_neg3 = newton(-3, f1, f1_prime)
root1_0, iterations1_0 = newton(0, f1, f1_prime)
root2_3, iterations2_3 = newton(3, f2, f2_prime)
root2_0, iterations2_0 = newton(0, f2, f2_prime)

#print(iterations1_0)
#print(root2_0)

# Plot the first quadratic for both guesses
plt.figure()
plt.plot(x1_vals, y1_vals, label=r'$f_1(x) = 2x^2 + 2\sqrt{10}x + 5$')
for i, x in enumerate(iterations1_neg3):
    plt.scatter(x, f1(x), color="red", label=f"Iter {i+1} (-3)" if i == 0 else None)
    #plt.annotate(f"Iter {i+1} (-3)", (x, f1(x)), textcoords="offset points", xytext=(-15, 10), ha='center', fontsize=8, color="blue")
    if i > 0:
        plt.plot([iterations1_neg3[i-1], x], [f1(iterations1_neg3[i-1]), f1(x)], 'r--', linewidth=1)
for i, x in enumerate(iterations1_0):
    plt.scatter(x, f1(x), color="purple", label=f"Iter {i+1} (0)" if i == 0 else None)
    #plt.annotate(f"Iter {i+1} (0)", (x, f1(x)), textcoords="offset points", xytext=(-15, -10), ha='center', fontsize=8, color="green")
    if i > 0:
        plt.plot([iterations1_0[i-1], x], [f1(iterations1_0[i-1]), f1(x)], 'g--', linewidth=1)
plt.scatter(root1_neg3, f1(root1_neg3), color="green", label="Root (-3)")
plt.scatter(root1_0, f1(root1_0), color="orange", label="Root (0)")
plt.axhline(0, color="black", linestyle="--")
plt.legend()
plt.grid()
plt.savefig('../../figs/fig1.png')

#print(iterations2_3)
# Plot the second quadratic for both guesses
plt.figure()
plt.plot(x2_vals, y2_vals, label=r'$f_2(x) = 2x^2 - 2\sqrt{10}x + 5$')
for i, x in enumerate(iterations2_3):
    plt.scatter(x, f2(x), color="red", label=f"Iter {i+1} (3)" if i == 0 else None)
    #plt.annotate(f"Iter {i+1} (3)", (x, f2(x)), textcoords="offset points", xytext=(-15, 10), ha='center', fontsize=8, color="blue")
    if i > 0:
        plt.plot([iterations2_3[i-1], x], [f2(iterations2_3[i-1]), f2(x)], 'r--', linewidth=1)
for i, x in enumerate(iterations2_0):
    plt.scatter(x, f2(x), color="purple", label=f"Iter {i+1} (0)" if i == 0 else None)
    #plt.annotate(f"Iter {i+1} (0)", (x, f2(x)), textcoords="offset points", xytext=(-15, -10), ha='center', fontsize=8, color="green")
    if i > 0:
        plt.plot([iterations2_0[i-1], x], [f2(iterations2_0[i-1]), f2(x)], 'g--', linewidth=1)
plt.scatter(root2_3, f2(root2_3), color="green", label="Root (3)")
plt.scatter(root2_0, f2(root2_0), color="orange", label="Root (0)")
plt.axhline(0, color="black", linestyle="--")
plt.legend()
plt.grid()
plt.savefig('../../figs/fig2.png')

plt.show()


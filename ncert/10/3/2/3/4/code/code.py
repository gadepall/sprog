import numpy as np
import matplotlib.pyplot as plt

A = np.array([[5, -3],[-10, 6]])
B = np.array([11, -22]).reshape(-1, 1)

n = 2

U1 = np.zeros((2, 2))
L1 = np.eye(2)

#Gaussian Elimination

for i in range(n):
    for j in range(n-1, i, -1):
        l_ij = A[j][i]/A[i][i]

        U1[j,:] = A[j,:] - l_ij * A[i,:]
        L1[j,i] = l_ij

U1[0,:] = A[0, :]

print(L1, U1)

#Doolittle's Algorithm

L2 = np.zeros((n,n))
U2 = np.zeros((n,n))

for i in range(n):
    for k in range(i,n):
        sum = 0
        for j in range(i):
            sum += L2[i,j] * U2[j,k]

        U2[i,k] = A[i,k] - sum
    for k in range(i,n):
        if(i==k): 
            L2[i,k] = 1
        else:
            sum = 0
            for j in range(i):
                sum += L2[k,j] * U2[j,i]
        
            L2[k,i] = (A[k,i] - sum) / U2[i,i]

print(L2, U2)

y = np.zeros((n,1))

for i in range(n):
    # if not i: y[i] = B[i,0]/L1[i,i]
    # else:
    #     y[i] = sum(L1[0:i,i])
    sum = 0
    for j in range(i):
        sum+= L1[i,j] * y[j,0]
    y[i,0] = (B[i,0] - sum)/L1[i,i]

print(y)

x = np.linspace(-5,5,100)
y = (5*x-11)/3

plt.plot(x, y, label = "$5x - 3y = 11$")
plt.plot(x, y, label = "-10x + 6y = -22", color = "red", ls = ":")

ax = plt.gca()
ax.spines['top'].set_color('none')
ax.spines['left'].set_position('zero')
ax.spines['right'].set_color('none')
ax.spines['bottom'].set_position('zero')

# plt.axis('equal')
plt.grid()
plt.legend(loc='best')

plt.savefig("../figs/fig1.png")
plt.show()
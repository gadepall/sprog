import numpy as np
import matplotlib.pyplot as plt
import cvxpy as cv

#fucntions
def func(x):
    return (x-1)**2 + 3

def diff(x):
    return 2 * (x-1)

x = np.linspace(-3, 1, 1000)


#gradient decent
x_init = -1.5
x_curr = x_init
step  = 0.0001
c = 0

x_1 = cv.Variable()
x_2 = cv.Variable()

while(np.abs(diff(x_curr))>1.0e-10):
    print(c)
    c+=1
    x_curr = x_curr - diff(x_curr) * step


while(x_init > -3):
    x_init = x_init + diff(x_init) * step
    print(x_init)


#quadratic optimization
constraints_1 = [x_1<=1, x_1 >= -3]

x_v = cv.Variable(2)
P = np.array([[1,0],[0,0]])
u = np.array([-2, -1]).reshape(-1, 1)
f = 4

# constraints = [[1, 0]@x_v>=-3, [1, 0] @ x_v <= 1]
constraints = [cv.quad_form(x_v, P) + u.T@x_v <= 0]
# objective_1 = cv.Minimize((x_1-1)**2 + 3)
objective_2 = cv.Minimize([0,1]@x_v)
prob_2 = cv.Problem(objective_2, constraints)
prob_2.solve()

# objective_2 = cv.Maximize((x_2)**2 + 3)
# prob_2 = cv.Problem(objective_2, constraints)
# prob_2.solve()


plt.plot(x, func(x), label = "function")
plt.scatter(x_v.value[0], func(x_v.value[0]), label = "Quadratic programming")
plt.scatter(x_init, func(x_init), label = "Gradient Decent")
print(x_v.value[0])
# print(prob_1.value)

# plt.scatter(x_2.value, func(x_2.value))
plt.savefig("../figs/fig2.png")
plt.show()
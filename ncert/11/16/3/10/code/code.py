import numpy as np
import matplotlib.pyplot as plt


iterations = 5000

def LCG():
    m = 2**31
    a = 1103515245
    c = 123545
    seed = 123423
    while(True):
        seed = (a*seed+c)%m
        yield seed

word = 'ASSASSINATION'

vowel = 'AEIOU'

P = np.zeros((iterations,2))

c = 1

# while(c<100000):
for i in LCG():
    if c==iterations:
        break
    
    n = i%13
    if(word[n] in vowel):
        P[c,0]=P[c-1,0]+1
        P[c,1]=P[c-1,1]
    else:
        P[c,0]=P[c-1,0]
        P[c,1]=P[c-1,1]+1
    print(c)
    c+=1
# for i in range(1, 100000):
#     n = LCG()
    # if(word[n] in vowel):
    #     P[i,0]=P[i-1,0]+1
    #     P[i,1]=P[i-1,1]
    # else:
    #     P[i,0]=P[i-1,0]
    #     P[i,1]=P[i-1,1]+1

# plt.scatter(P[:,0],P[:,1])
p1 = [[P[i,0]/i for i in range(1,iterations)],[i for i in range(1,iterations)]]
p1T = [[6/13 for i in range(1,iterations)],[i for i in range(1,iterations)]]

p2 = [[P[i,1]/i for i in range(1,iterations)],[i for i in range(1,iterations)]]
p2T = [[7/13 for i in range(1,iterations)],[i for i in range(1,iterations)]]

plt.scatter(p1[1],p1[0],color = "lightsteelblue", label="Practical1", s=1)
plt.plot(p1T[1],p1T[0],label="Theory1",color = "blue")

plt.scatter(p2[1],p2[0],color = "lightcoral",label="Practical2",s=1)
plt.plot(p2T[1],p2T[0],color = "red",label="Theory2")


ax = plt.gca()
ax.spines['top'].set_color('none')
ax.spines['left'].set_position('zero')
ax.spines['right'].set_color('none')
ax.spines['bottom'].set_position('zero')

# plt.axis('equal')
plt.grid()
plt.legend(loc='best')


plt.savefig("../figs/fig1.png")
plt.close()
# plt.show()

x = np.array([0, 1])
y = np.array([p1T[0][-1], p2T[0][-1]])


plt.stem(x, y, linefmt='--')
plt.grid()
plt.axis('equal')
plt.savefig("../figs/fig2.png")
plt.show()

import numpy as np
def calculate_radius(r_0, r_3, t):
    t_0 = 3.0  #Time at which r_3 is given (in seconds)
    rate = 4*(np.pi)*(r_3**3 - r_0**3)/(3*t_0) #Calculate the rate of change of volume
    r_t = ((r_0**3) + (3*rate*t)/(4*np.pi))**(1/3) #Calculate the radius at time t
    return r_t
try:
    r_0 = float(input("Enter initial radius (r_0): "))
    r_3 = float(input("Enter radius at 3 seconds (r_3): "))
    t = float(input("Enter the time (t) after which the radius is to be found: "))
    if r_0 <= 0 or r_3 <= 0 or t < 0:
        print("All radii must be positive, and time must be non-negative.")
    else:
        r_t = calculate_radius(r_0, r_3, t)
        print(f"The radius of the sphere after {t} seconds is {r_t:.3f} units.")
except ValueError:
    print("Invalid input. Please enter numeric values.")

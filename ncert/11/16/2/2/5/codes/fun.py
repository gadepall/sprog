import random

def roll_die():
  return random.randint(1, 6)

num_trials = 100000
num_sixes = 0

for _ in range(num_trials):
  if roll_die() == 6:
    num_sixes += 1

estimated_probability = num_sixes / num_trials
print(f"Estimated probability of rolling a 6: {estimated_probability}")

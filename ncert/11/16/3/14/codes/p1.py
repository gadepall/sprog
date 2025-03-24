import random

# Assign the probability values
P_a = 3/5  # Probability of event A
P_b = 1/5  # Probability of event B
total_trials = 100000  # Number of trials

# Variables to count the occurrences of correct events
correct_count = 0

for _ in range(total_trials):
    # Generate a random number between 0 and 1
    random_number = random.random()

    # Determine the event based on the random number
    if random_number < P_a:
        event = 'A'  # Event A occurs
        correct_count += 1  # Count it as correct if event A happens within its probability
    elif random_number < P_a + P_b:
        event = 'B'  # Event B occurs
        correct_count += 1  # Count it as correct if event B happens within its probability
    else:
        event = 'None'  # Neither event A nor B occurs

# Calculate the probability of correctly predicting the event
probability_correct = correct_count / total_trials

print(f"Probability of correct event prediction: {probability_correct}")


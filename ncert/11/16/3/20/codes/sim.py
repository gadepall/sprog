# Given probabilities
P_english_and_hindi = 0.5  # P(English ∩ Hindi)
P_neither = 0.1            # P(Neither English nor Hindi)
P_english = 0.75           # P(English)

# Step 1: Calculate P(English ∪ Hindi)
P_english_or_hindi = 1 - P_neither

# Step 2: Use the union formula to solve for P(Hindi)
# P(English ∪ Hindi) = P(English) + P(Hindi) - P(English ∩ Hindi)
# Rearrange to solve for P(Hindi):
P_hindi = P_english_or_hindi - P_english + P_english_and_hindi

# Output the result
print(f"Probability of passing Hindi: {P_hindi:.2f}")

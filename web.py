import sys

# Reading input without pressing Enter
print("Please type your input:")

input_data = sys.stdin.read().strip()  # Read input without needing 'Enter'

print("You entered:", input_data)

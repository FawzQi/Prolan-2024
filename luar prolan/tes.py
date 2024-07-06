# Define the dataset
X = [1, 2, 3, 4, 5]  # Input features
y = [2, 4, 6, 8, 10]  # Output labels

# Calculate the mean of X and y
mean_x = sum(X) / len(X)
mean_y = sum(y) / len(y)

# Calculate the slope (m) and y-intercept (c)
numerator = sum([(X[i] - mean_x) * (y[i] - mean_y) for i in range(len(X))])
denominator = sum([(X[i] - mean_x) ** 2 for i in range(len(X))])
m = numerator / denominator
c = mean_y - (m * mean_x)

# Predict the output for a new input
new_x = 6
predicted_y = m * new_x + c

# Print the slope, y-intercept, and predicted output
print("Slope (m):", m)
print("Y-intercept (c):", c)
print("Predicted output for x =", new_x, ":", predicted_y)

# Calculate the root mean square error (RMSE)
rmse = (sum([(y[i] - (m * X[i] + c)) ** 2 for i in range(len(X))]) / len(X)) ** 0.5

# Print the root mean square error
print("Root Mean Square Error (RMSE):", rmse)
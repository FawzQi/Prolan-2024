def f(x):
    # Define your function here
    return x**2 - 4

def secant_method(x0, x1, tol, max_iter):
    i = 0
    while abs(x1 - x0) > tol and i < max_iter:
        x2 = x1 - (f(x1) * (x1 - x0)) / (f(x1) - f(x0))
        x0 = x1
        x1 = x2
        i += 1
    return x1

# Initial guesses
x0 = 1
x1 = 2

# Tolerance and maximum number of iterations
tolerance = 1e-6
max_iterations = 100

# Solve for the root using the Secant method
root = secant_method(x0, x1, tolerance, max_iterations)

print("Root:", root)

def false_position_method(x0, x1, tol, max_iter):
    i = 0
    while abs(x1 - x0) > tol and i < max_iter:
        x2 = (x0 * f(x1) - x1 * f(x0)) / (f(x1) - f(x0))
        if f(x2) == 0:
            return x2
        elif f(x0) * f(x2) < 0:
            x1 = x2
        else:
            x0 = x2
        i += 1
    return x2

# Solve for the root using the False Position method
root_false_position = false_position_method(x0, x1, tolerance, max_iterations)

#rint("Root (Secant Method):", root)
print("Root (False Position Method):", root_false_position)
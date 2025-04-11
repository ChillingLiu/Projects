#!usr/bin/python3
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.spines
import sympy

# least square method with error analysis
def least_square(x_data, y_data, degree):
	tmp = np.ones(len(x_data))
	A = [tmp]
	for i in range(1, degree + 1):
		A.append(x_data**i)
	A = np.array(A).T

	AtA = A.T@A
	Atb = A.T@y_data.T
	sol = np.linalg.solve(AtA, Atb)
	print("sol = ", sol)

	# construct the function using sympy
	x = sympy.symbols("x")
	f = 0
	for i in range(0, degree + 1):
		f += x**i * sol[i]
	print("construct f = ", f)
	print()

	# use polyfit to check my answer.
	sol2 = np.polyfit(x_data, y_data, degree)
	f2 = np.poly1d(sol2)
	print("polyfit to verify my answer: ")
	print(f2)
	print()

	# error analysis
	# error 1: Ax - b
	print("---------- Error Analysis ----------")
	error_list = A@sol - y_data
	print("A*x - b = ", error_list)
	error = 0
	for i in error_list:
		error += i**2
	error = np.sqrt(error)
	print("Error 1: ||Ax - b|| = ", error)

	# error 2: f(x) - b
	error2 = 0
	for i in range(len(x_data)):
		error2 += (y_data[i] - f.evalf(subs = {x: x_data[i]}))**2
	# the returned value of f.evalf is not float, sqrt can't support
	error2 = np.sqrt(float(error2))
	print("Error 2: ||f(x) - y|| = ", error2)

	# prepare list to plot
	distance = np.max(x_data) - np.min(x_data)
	x_plot = np.linspace(x_data[0] - distance/len(x_data), x_data[len(x_data)-1] + distance/len(x_data), 200)
	y_plot = len(x_plot)*[0]
	for i in range(0, len(x_plot)):
		y_plot[i] = f.evalf(subs = {x: x_plot[i]})

	plt.figure()
	plot1 = plt.plot(x_data, y_data, '*', color = "r", label='Original Data')
	plot2 = plt.plot(x_plot, y_plot, label='Constructed Polynomial')
	plt.xlabel('x')
	plt.ylabel('f(x)')
	plt.legend(loc=4)
	plt.title("Least Squares Method with degree " + str(degree) + "\n" + str(f))
	ax = plt.gca()
	ax.spines["top"].set_color("none")
	ax.spines["right"].set_color("none")
	ax.spines["bottom"].set_position(("data", 0))
	ax.spines["left"].set_position(("data", x_data[0]))

	for i in range(len(x_data)):
		for j in range(len(x_data)):
			point = "("+str(round(x_data[i], 3))+", "+str(round(y_data[i], 3))+")"
			plt.text(x_data[i]+0.03, y_data[i]-0.03, point, color = "r")
	plt.show()
	return f


#filename = "Least_Square_data_points.txt"
filename = "Least_Square_moore.txt"
data = np.loadtxt(filename)
x_data = data[:, 0]
y_data = data[:, 1]
degree = 3

# the moore's law goes through in a logarithm way
# the idea is use np.log to convert the data first
# get the predicted value and convert back
if filename == "Least_Square_moore.txt":
	y_data = np.log10(y_data)

f = least_square(x_data, y_data, degree)

if filename == "Least_Square_moore.txt":
	print("\n---------- Prediction of Moore's Law ----------")
	x = sympy.symbols("x")
	prediction1 = f.evalf(subs = {x: 2022})
	prediction1 = 10**(float(prediction1))
	print("Prediction of year 2022 is " + str(prediction1) + ", compare to 5.3e12 in real world")
	prediction2 = f.evalf(subs = {x: 2024})
	prediction2 = 10**(float(prediction2))
	print("And the prediction of year 2024 is", prediction2)


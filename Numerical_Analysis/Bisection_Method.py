#!usr/bin/python3
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.spines

def bisection(a, b, f, tol):
	xlist = np.linspace(a, b, 100)
	ylist = f(xlist)
	x = a + (b - a)/2
	FA = f(a)
	FB = f(b)
	FX = f(x)
	plt.figure()
	plt.plot(xlist, ylist)
	plt.xlabel("x")
	plt.ylabel("f(x)")
	ax = plt.gca()
	plt.title("Bisection Method")
	ax.spines["top"].set_color("none")
	ax.spines["right"].set_color("none")
	ax.spines["bottom"].set_position(("data", 0))
	ax.spines["left"].set_position(("data", x))
	plt.scatter(x, 0, color = "red")
	plt.annotate("x0", (x, 0.03), color = "red")
	plt.annotate("a", (a, 0.03), color = "red")
	plt.annotate("b", (b, 0.03), color = "red")
	i = 0
	while FX != 0 and (b - a)/2 > tol:
		i = i + 1
		x = a + (b - a)/2
		print("x" + str(i) + " =", x)
		FX = f(x)
		plt.scatter(x, 0, color = "red")
		if FA*FX > 0:
			a = x
			FA = FX
		else:
			b = x
	plt.annotate("Final x" + str(i) + " = " + str(x), (x, 0.1), color = "purple")
	plt.show()
	if abs(f(x) - tol) < 0.001:
		print("The solution x =", x)
		print("The number of iterations =", i)
		print("")
	else:
		print("We didn't find the solution, Bisection Method failed!")
		print("The final x =", x, "While it's not the solution.")
		print("The number of iterations =", i)
		print("")

def f1(x):
	return 2*x**3 - 2*x -5
def f2(x):
	return x**6 + 6*x**5 + 9*x**4 - 2*x**3 - 6*x**2 + 1
def f3(x):
	return np.e**x - x**2 + 3*x - 2
def f4(x):
	return -3.55*x**3 + 1.1*x**2 + 0.756*x - 0.74

bisection(1,2,f1,10**-5)
bisection(-3,-2,f2,10**-5)
bisection(0,1,f3,10**-5)
bisection(-1,1,f4,10**-5)

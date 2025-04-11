#!usr/bin/python3
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.spines

from sympy import diff, exp
from sympy.abc import x

def newton(p0, f, tol, max_iter):
	print("p0 =", p0)
	df = diff(f,x)
	ddf = diff(df,x)
	a = p0 - 1
	b = p0 + 1
	xlist = np.linspace(a, b, 100)
	ylist = 100*[0]
	for i in range(0,100):
		ylist[i] = f.evalf(subs={x:xlist[i]})
	plt.figure()
	plt.plot(xlist, ylist)
	plt.xlabel("x")
	plt.ylabel("f(x)")
	plt.title("Modified Newton Method on page 83")
	ax = plt.gca()
	ax.spines["top"].set_color("none")
	ax.spines["right"].set_color("none")
	ax.spines["bottom"].set_position(("data", 0))
	ax.spines["left"].set_position(("data", p0))
	plt.scatter(p0, 0, color = "red")
	plt.annotate("p0", (p0, 0.03), color = "red")
	i = 1
	while i <= max_iter:
		p = p0 - (f.evalf(subs={x:p0}) * df.evalf(subs={x:p0})) / (df.evalf(subs={x:p0})**2 - f.evalf(subs={x:p0}) * ddf.evalf(subs={x:p0}))
		FP = f.evalf(subs={x:p})
		plt.scatter(p, 0, color = "red")
		print("p" + str(i) + " =", p)
		if abs(p - p0) < tol:
			break
		else:
			i = i + 1
			p0 = p
	if i > max_iter:
		print("The method failed after", i - 1, "iterations")
	else:
		print("The solution p =", p)
		print("The number pf iterations =", i)
		print("")
		plt.annotate("Final p" + str(i) + " = " + str(p), (p, 0.1), color = "purple")
		plt.show()

f1 = 2*x**3 - 2*x - 5
f2 = x**6 + 6*x**5 + 9*x**4 - 2*x**3 - 6*x**2 + 1
f3 = exp(x) - x**2 + 3*x - 2
f4 = -3.55*x**3 + 1.1*x**2 + 0.756*x - 0.74

newton(1, f1, 10**-5, 1000)
newton(-3, f2, 10**-5, 1000)
newton(0, f3, 10**-5, 1000)
newton(1, f4, 10**-5, 1000)

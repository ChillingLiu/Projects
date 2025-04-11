#!usr/bin/python3
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.spines
import sympy

def interpolate(x_list, fx_list):
	x = sympy.symbols("x")
	L = []
	for i in range(len(x_list)):
		tmp = 1
		for j in range(len(x_list)):
			if j != i:
				tmp *= (x - x_list[j])
				tmp /= (x_list[i] - x_list[j])
		L.append(tmp)
	p = 0
	for i in range(len(L)):
		p += L[i] * fx_list[i]
	p = sympy.expand(p)
	print(p)

	xlist = np.linspace(x_list[0]-1, x_list[len(x_list)-1]+1, 200)
	ylist = 200*[0]
	for i in range(0,200):
		ylist[i] = p.evalf(subs={x:xlist[i]})

	plt.figure()
	plt.plot(xlist, ylist, label = "lagrange intepolation")
	plt.plot(x_list, fx_list, "*", color = "r", label = "original data")
	plt.legend(loc = 4)
	plt.xlabel("x")
	plt.ylabel("f(x)")
	plt.title("Lagrange Intepolation of ")
	ax = plt.gca()
	ax.spines["top"].set_color("none")
	ax.spines["right"].set_color("none")
	ax.spines["bottom"].set_position(("data", 0))
	ax.spines["left"].set_position(("data", x_list[0]))

	for i in range(len(x_list)):
		for j in range(len(x_list)):
			point = "("+str(x_list[i])+", "+str(fx_list[i])+")"
			plt.text(x_list[i]+0.1, fx_list[i]-0.1, point, color = "r")
	plt.show()

x_list = np.array([0, 1, 3, 4, 5])
fx_list = np.array([-1, -0.5, 1, 2, 3])

interpolate(x_list, fx_list)

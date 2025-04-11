#!usr/bin/python3
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.spines

# This function only calculate a SINGLE RK4 STEP!
def RK4_step(func, dt, t_curr, w_curr):
	# dt - time step
	# t_curr - current time value
	# w_curr - current function state
	h = dt
	t = t_curr
	w = w_curr

	K1 = h * func(t, w)
	K2 = h * func(t + h/2, w + K1/2)
	K3 = h * func(t + h/2, w + K2/2)
	K4 = h * func(t + h, w + K3)

	w = w + (K1 + 2*K2 + 2*K3 + K4) / 6
	t += h
	return (t, w)


# Runge-Kutta Method (Order 4)
# To approxiamte the solution of the initial-value problem
# y' = f(t, y), y(a) = alpha
# at N+1 equally spaced numbers in the interval [a, b]
# Inputs: endpoints a, b; integer N+1; initial condition alpha
# Outputs: approximation w to y at the N+1 values of t
def Runge_Kutta_4(func_description, func, a, b, N, alpha, func_sol):
	h = (b - a) / N
	# we need total N+1 points to cover all [1, b]
	N += 1
	t = N*[0]
	y = N*[0]
	t[0] = a
	y[0] = alpha

	for i in range(0, N - 1):
		(t[i + 1], y[i + 1]) = RK4_step(func, h, t[i], y[i])

	fig = plt.figure()
	plt.plot(t, y, color = "purple", label = "ODE constructed solution")
	plt.plot(t, y, '*', color = "r", label='every step')

	x_plot = np.linspace(a, b, 100)
	y_plot = func_sol(x_plot)
	plt.plot(x_plot, y_plot, label='ODE real solution')

	plt.xlabel('t')
	plt.ylabel('y(t)')
	plt.legend(loc = 4)
	plt.title("Runge Kutta Order 4 with h = " + str(h) + "\n" + str(func_description))

	ax = plt.gca()
	ax.spines["top"].set_color("none")
	ax.spines["right"].set_color("none")
	ax.spines["bottom"].set_position(("data", y[0]))
	ax.spines["left"].set_position(("data", 0))
	fig.set_size_inches(11, 7)
	plt.savefig(str(func_description) + "_" + str(N-1) + ".png", bbox_inches = "tight")
	plt.show()

# y = exp(t)
def f1_dt(t, y):
	return y
def f1_sol(t):
	return np.exp(t)

# y = x^2
def f2_dt(t, y):
	return 2*t
def f2_sol(t):
	return t**2

# y = exp(-3t) + 2t + 3
def f3_dt(t, y):
	return -3*y + 6*t + 11
def f3_sol(t):
	return np.exp(-3*t) + 2*t + 3

N = 10
func_description = "y' = exp(t), Sol: y = exp(t)"
Runge_Kutta_4(func_description, f1_dt, 0, 1, N, f1_sol(0), f1_sol)

func_description = "y' = 2t, Sol: y = t^2"
Runge_Kutta_4(func_description, f2_dt, 0, 1, N, f2_sol(0), f2_sol)

func_description = "y' = -3y + 6t + 11, Sol: y = exp(-3t) + 2t + 3"
Runge_Kutta_4(func_description, f3_dt, 0, 1, N, f3_sol(0), f3_sol)

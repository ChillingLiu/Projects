#!usr/bin/python3
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.spines

# This function only calculate a SINGLE RK4 STEP!
def lorenz_RK4_step(func, dt, t_curr, w_curr, d):
	# dt - time step
	# t_curr - current time value
	# w_curr - current state = (xi, yi, zi)
	h = dt
	t = t_curr
	w = w_curr

	K1 = h * func(t, w)
	K2 = h * func(t + h/2, w + K1/2)
	K3 = h * func(t + h/2, w + K2/2)
	K4 = h * func(t + h, w + K3)
	w = w + (K1 + 2*K2 + 2*K3 + K4) / 6

	# d is the index we want for x, y, z respectively
	# d[0] = x, d[1] = y, d[2] = z
	return w[d]


# Use Runge-Kutta Method (Order 4)
# to approxiamte the lorenz equations
def lorenz_RK4(func_description, func_x, func_y, func_z, a, b, N, x0, y0, z0):
	h = (b - a) / N
	# we need total N+1 points to cover all [1, b]
	N += 1
	t = N*[0]
	x = N*[0]
	y = N*[0]
	z = N*[0]
	t[0] = a
	x[0] = x0
	y[0] = y0
	z[0] = z0

	for i in range(0, N - 1):
		# current function state
		w_curr = np.array([x[i], y[i], z[i]])
		x[i + 1] = lorenz_RK4_step(func_x, h, t[i], w_curr, 0)
		y[i + 1] = lorenz_RK4_step(func_y, h, t[i], w_curr, 1)
		z[i + 1] = lorenz_RK4_step(func_z, h, t[i], w_curr, 2)
		t[i + 1] = t[i] + h

	fig = plt.figure()
	ax = plt.axes(projection = "3d")
	ax.plot3D(x, y, z)
	plt.xlabel('x axis')
	plt.ylabel('y axis')
	plt.ylabel('z axis')
	plt.title("Lorenz Equation with t = " + str(b) + "\n" + str(func_description))

	fig.set_size_inches(11, 7)
	plt.savefig("lorenz_" + str(b) + ".png", bbox_inches = "tight")
	plt.show()

sigma = 10
beta = 8/3
rho = 28
def fx_dt(t, w_curr):
	x = w_curr[0]
	y = w_curr[1]
	z = w_curr[2]
	return sigma*(y - x)
def fy_dt(t, w_curr):
	x = w_curr[0]
	y = w_curr[1]
	z = w_curr[2]
	return x*(rho - z) - y
def fz_dt(t, w_curr):
	x = w_curr[0]
	y = w_curr[1]
	z = w_curr[2]
	return x*y - beta*z

N = 4000
(a, b) = (0, 10)
(x0, y0, z0) = (1, 1, 1)
func_description = "sigma = 10, beta = 8/3, rho = 28"
lorenz_RK4(func_description, fx_dt, fy_dt, fz_dt, a, b, N, x0, y0, z0)

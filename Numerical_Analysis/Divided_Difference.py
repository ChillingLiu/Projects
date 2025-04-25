import numpy as np
import matplotlib.pyplot as plt
import matplotlib.spines
import sympy

# follows the algorithm on page 124, chapter 3.
def divided_difference(x_list, fx_list):
	# the number of steps of divided differences
	n = len(x_list) - 1

	F = [0]*(n+1)
	for i in range(n+1):
		F[i] = [0]*(n+1)

	# set initial values.
	for i in range(n+1):
		F[i][0] = fx_list[i]
	
	# F[i][j] is f[x(i-j), ..., xi]
	for i in range(1, n+1):
		for j in range(1, i+1):
			F[i][j] = (F[i][j-1] - F[i-1][j-1]) / (x_list[i] - x_list[i-j])

	# show the matrix F
	print("Show the matrix F, such that F[i][j] = f[x(i-j), ..., xi]")
	for i in range(len(F)):
		for j in range(len(F)):
			print(F[i][j], end = '\t')
		print('')

	# start construct Pn(x)
	x = sympy.symbols("x")
	p = F[0][0]
	
	for i in range(1, n+1):
		tmp = 1
		for j in range(0, i):
			tmp *= (x - x_list[j])
		tmp *= F[i][i] 
		p += tmp
	p = sympy.expand(p)
	print("\nThe constructed polynomial P" + str(n) + "(x) = ")
	print(p)
	return p

# example on the page 125
#x_list = [1.0, 1.3, 1.6, 1.9, 2.2]
#fx_list = [0.76519, 0.62008, 0.45540, 0.28181, 0.11036]
#divided_difference(x_list, fx_list)





def on_click(event):
	# This function will run everytime the event is triggered
	print('\n\n------------------------------------------------------------------')
	print(f'CLICK! Add one point to the Divided Difference algorithm: x={event.xdata}, y={event.ydata}')
	x_list.append(event.xdata)
	fx_list.append(event.ydata)

	plt.scatter(x_list, fx_list)

	if(len(x_list) >= 2):
		x = sympy.symbols("x")
		p = divided_difference(x_list, fx_list)

		# prepare the list for plot
		x_plot = np.linspace(-1, 1, 200)
		y_plot = len(x_plot)*[0]
		for i in range(0, len(x_plot)):
			y_plot[i] = p.evalf(subs = {x: x_plot[i]})
		plot = plt.plot(x_plot, y_plot, label='P' + str(len(x_list) - 1) + "(x)")
		plt.legend(loc=4)
		plt.axis([-1, 1, -1, 1])
	plt.show()

x_list = []
fx_list = []

fig = plt.figure()
plt.title('Click the graph to add points for the Divided Difference Algorithm.')
plt.xlabel('x')
plt.ylabel('f(x)')

fig.canvas.mpl_connect('button_press_event', on_click) # connect figure to event
plt.axis([-1, 1, -1, 1])
fig.set_figwidth(11)
fig.set_figheight(7)
plt.show() # update the figure
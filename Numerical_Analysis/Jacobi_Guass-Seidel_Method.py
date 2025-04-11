#!user/bin/python3
import numpy as np

def norm(x, x0):
	tmp = 0
	for i in range(len(x)):
		tmp += (x[i] - x0[i])**2
	return(np.sqrt(tmp))

def jacobi(A, b, x0, tol, max_iter):
	n = len(A)
	x = np.zeros(n)

	k = 1
	while k <= iter:
		for i in range(n):
			tmp = 0
			#j from 0 to n-1 and j not equal to i
			for j in range(n):
				tmp += A[i][j]*x0[j]
			tmp -= A[i][i]*x0[i]
			x[i] = 1/A[i][i] * (-tmp + b[i])

		if norm(x, x0) < tol:
			print(f"Iterations: {k}, sol = {x}")
			print(f"Check: A@x - b = {A@x-b}")
			return(x)
		k += 1
		#must use .copy() here, or it will only copy the address of x.
		x0 = x.copy()
	print("Maximum number of iterations exceeded!")

def gauss_seidel(A, b, x0, tol, max_iter):
	n = len(A)
	x = np.zeros(n)

	k = 1
	while k <= iter:
		for i in range(n):
			tmp = 0
			#j from 1 to i - 1
			for j in range(i):
				tmp += A[i][j]*x[j]
			#j from i + 1 to n
			for j in range(i + 1, n):
				tmp += A[i][j]*x0[j]
			x[i] = 1/A[i][i] * (-tmp + b[i])

		if norm(x, x0) < tol:
			print(f"Iterations: {k}, sol = {x}")
			print(f"Check: A@x - b = {A@x-b}")
			return(x)
		k += 1
		#must use .copy() here, or it will only copy the address of x.
		x0 = x.copy()
	print("Maximum number of iterations exceeded!")



A1 = np.array([[4, 1, -1], [-1, 3, 1], [2, 2, 5]])
b1 = [5, -4, 1]
A2 = np.array([[5, -2, 3], [-3, 9, 1], [2, -1, -7]])
b2 = [10, -6, 5]

tol = 10**-5
iter = 20
x0 = np.array([0, 0, 0])

print("-------------------- Jacobi Method --------------------\n")
jacobi(A1, b1, x0, tol, iter)
print("")
jacobi(A2, b2, x0, tol, iter)

print("\n----------------- Guass-Seidel Method -----------------\n")
gauss_seidel(A1, b1, x0, tol, iter)
print("")
gauss_seidel(A2, b2, x0, tol, iter)

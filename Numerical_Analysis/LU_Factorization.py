#!user/bin/python3
import numpy as np

def gauss_elim(A, b):
	#Store the original A and b in order to check the error
	print(f"Original Matrix = \n{A}\n")
	origin_A = A.copy()
	origin_b = b.copy()

	n = len(A)
	s = np.zeros(n)
	for i in range(n):
		s[i] = max(abs(A[i]))
		if s[i] == 0:
			sys.exit("No Unique Solution Exists!")
	nrow = list(range(n))

	#Steps 3 - 6
	for i in range(n):
		#Get the smallest integer p every time
		p = i
		tmp = abs(A[nrow[i]][i])/s[i]
		for j in range(i + 1, n):
			if abs(A[nrow[j]][i])/s[nrow[j]] > tmp:
				p = j
				tmp = abs(A[nrow[j]][i])/s[nrow[j]]
		if A[p][i] == 0:
			sys.exit("No Unique Solution Exists!")
		if nrow[i] != nrow[p]:
			print(f"Interchange row {i} and row {p}")
			ncopy = nrow[i]
			nrow[i] = nrow[p]
			nrow[p] = ncopy

		#Steps 7 - 8
		for j in range(i + 1, n):
			m = A[nrow[j]][i]/A[nrow[i]][i]
			for k in range(n):
				A[nrow[j]][k] -= m*A[nrow[i]][k]
			b[nrow[j]] -= m*b[nrow[i]]

	if A[nrow[n - 1]][n - 1] == 0:
		sys.exit("No Unique Solution Exists!")
	print(f"nrow = {nrow}")
	print(f"\n(Applying the order in nrow, you can get an upper triangle)\
		\nConverted Matrix =\n{np.round(A, decimals = 3)}")

	#Start backward substitution
	x = np.zeros(n)
	x[n - 1] = b[nrow[n - 1]]/A[nrow[n - 1]][n - 1]
	for i in range(n - 2, -1, -1):
		sum = 0
		for j in range(i + 1, n):
			sum += x[j]*A[nrow[i]][j]
		x[i] = (b[nrow[i]] - sum)/A[nrow[i]][i]

	print(f"\nAnswer x = {np.round(x, decimals = 3)}")
	print(f"Test Error: A*x - b = {origin_A@x - origin_b}")


A1 = np.array([[1.19, 2.11, -100, 1], [14.2, -0.122, 12.2, -1], [0, 100, -99.9, 1], [15.3, 0.11, -13.1, -1]])
b1 = np.array([1.12, 3.44, 2.15, 4.16])
gauss_elim(A1, b1)

print("\n-------------------------------------------------------------------------------------------\n")

A2 = np.array([[3.333, 15920, 10.333], [2.222, 16.71, 9.612], [-1.5611, 5.1792, -1.6855]])
b2 = np.array([7953, 0.965, 2.714])
gauss_elim(A2, b2)


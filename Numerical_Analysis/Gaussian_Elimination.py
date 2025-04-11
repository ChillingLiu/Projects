#!user/bin/python3
import numpy as np

def lu_decomp(A):
	n = len(A)
	U = np.diag(np.diag(A))
	L = np.identity(n)
	#u11*l11 = a11
	print("The type of U is: ", U.dtype, "\nWe need to change it to float64 before we decompose")
	U = U.astype(float)
	print(f"U = U.astype(float), then U.dtype = {U.dtype}\n")

	if A[0][0] == 0:
		sys.exit("Factorization impossible")
	for j in range(1, n):
		U[0][j] = A[0][j]/L[0][0]
		L[j][0] = A[j][0]/U[0][0]

	#Steps 4 and 5
	for i in range(1, n - 1):
		if A[i][i] == 0:
			sys.exit("Factorization impossible")
		for k in range(i):
			#k from 1 to i - 1
			U[i][i] -= L[i][k]*U[k][i]

		#uii = aii, lii = 1, such that uii*lii = aii - Sum(lik*uki)
		for j in range(i + 1, n):
			tmpu = tmpl = 0
			for k in range(i):
				tmpu += L[i][k]*U[k][j]
				tmpl += L[j][k]*U[k][i]
			U[i][j] = (A[i][j] - tmpu)/L[i][i]
			L[j][i] = (A[j][i] - tmpl)/U[i][i]

	for k in range(n - 1):
		#k from 1 to n - 1
		U[n - 1][n - 1] -= L[n - 1][k]*U[k][n - 1]
	if U[n - 1][n - 1] == 0:
		print("A = L*U, but A is singular.")
	print(f"A = \n{A}\n")
	print(f"Lower Triangle: L = \n{L}")
	print(f"\nUpper Triangle: U = \n{U}")
	print(f"\nCheck L*U = \n{L@U}")
	print(F"\nCheck Error: L*U - A = \n{L@U - A}")



A1 = np.array([[2, 1, 0, 0], [-1, 3, 3, 0], [2, -2, 1, 4], [-2, 2, 2, 5]])
lu_decomp(A1)

print("\n---------------------------------------------------------------\n")

A2 = np.array([[12, 22, 23, 4], [3, 2, 1, -1], [5, 4, 5, 5], [3, 3, 3, 3]])
lu_decomp(A2)

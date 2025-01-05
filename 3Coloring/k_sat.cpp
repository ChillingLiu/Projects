#include <iostream>
#include <time.h>
using namespace std;

int* br(int x, int n);
int ibr(int* ls, int n);
int* psc(int* ls, int n, int e);

//Evaluation of a phrase in an assignment.
//A phrase is false if one of its components is false.
int vlueph(int* a, int* p, int n);

//Evaluation of a clause in an assignment.
//A clause is true if one of its components is true.
int vluecl(int* a, int* c, int n);

//Evaluation of a disjunctive(logic or) form in an assignment.
//A disjunctive form is a list of phrases, interpreted
//as the dijunctive of its components.
int vluedf(int* a, int** df, int k, int n);

//Evaluation of a conjunctive(logic and) form in an assignment.
//A conjunctive form is a list of clauses, interpreted
//as the conjunction of its componnents.
int vluecf(int* a, int** cf, int k, int n);

//Total evaluation of a conjuctive form in a list.
int* teval(int** cf, int k, int n);

//Generate a random instance of k-SAT over n variables with m clauses.
int** rndksgen(int n, int m, int k);

//Convert a random instance of 3SAT in the conjunctive form
//representation given in the previous function rndksgen().
int** cvrt(int n, int m, int k, int** kcf);

//Start an experiment of K-SAT using above functions.
void exper(int n, int m, int k);

int main() {
	srand(time(NULL));
	cout << "--------------------exper(6, 8, 2)--------------------" << endl;
	exper(4, 4, 3);
	
	//Example: A conjunctive form 2-CF with 6 rows and 8 lines.
	cout << "--------------------exper(6, 8, 2)--------------------" << endl;
	exper(6, 8, 2);
	//Example: A conjunctive form 3-CF with 6 rows and 8 lines.
	cout << endl << "--------------------exper(6, 8, 3)--------------------" << endl;
	exper(6, 8, 3);
	//Example: A conjunctive form 4-CF with 6 rows and 8 lines.
	cout << endl << "--------------------exper(6, 8, 4)--------------------" << endl;
	exper(6, 8, 4);
	//Example: A conjunctive form 5-CF with 7 rows and 12 lines.
	cout << endl << "--------------------exper(6, 8, 5)--------------------" << endl;
	exper(6, 8, 5);
	return 0;
}

//Start an experiment of K-SAT.
void exper(int n, int m, int k) {
	int** kins = rndksgen(n, m, k);
	cout << "The " << k << "-CF instance is: " << endl;
	for (int i = 0; i < m; i++) {
		cout << "| ";
		for (int j = 0; j < k; j++) {
			printf("%2d ", kins[i][j]);
		}
		cout << "|" << endl;
	}

	int** cf = cvrt(n, m, k, kins);
	cout << "Written as a CF it is: " << endl;
	for (int i = 0; i < m; i++) {
		cout << "| ";
		for (int j = 0; j < n; j++) {
			printf("%2d ", cf[i][j]);
		}
		cout << "|" << endl;
	}

	int* vev = teval(cf, m, n);
	int n_vev = 1;
	for (int i = 0; i < n; i++) {
		n_vev *= 2;
	}
	n_vev--;

	int* sta = psc(vev, n_vev, 1);
	cout << "The satisfying assignmnets correspond to numbers: " << endl << "| ";
	int n_sta = 0;
	for (int i = 0; sta[i] != '\0'; i++) {
		n_sta++;
	}
	if (n_sta != 0) {
		n_sta--;
	}
	for (int i = 0; i < n_sta; i++) {
		cout << sta[i] << " ";
	}
	cout << "|" << endl;
	cout << "And they are " << n_sta << endl;

	delete[] kins;
	delete[] cf;
	delete[] vev;
	delete[] sta;
}

//--------------------Implementation of functions--------------------//

int* br(int x, int n) {
	int* lsn = new int[n];
	int x0;
	int x1 = x;
	for (int i = 1; i <= n; i++) {
		x0 = x1 % 2;
		lsn[n - i] = 2 * x0 - 1;
		x1 = (x1 - x0) / 2;
	}
	return lsn;
}

int ibr(int* ls, int n) {
	int x = (ls[0] + 1) / 2;
	for (int i = 1; i < n; i++) {
		x = 2 * x + (ls[i] + 1) / 2;
	}
	return x;
}

int* psc(int* ls, int n, int e) {
	int* temp = new int[n] ();
	int j = 0;
	for (int i = 0; i < n; i++) {
		if (ls[i] == e) {
			temp[j] = i + 1; //The resulting array start from 1, because I want to be the same with the notebook.
			j++;
		}
	}
	int* lp = new int[j];
	for (int i = 0; i < j; i++) {
		lp[i] = temp[i];
	}
	delete[] temp;
	return lp;
}

//Evaluation of a phrase in an assignment.
//A phrase is false if one of its components is false.
int vlueph(int* a, int* p, int n) {
	int nz = 0;
	int j = 0;
	bool fl = true;
	while (fl == true && j < n) {
		if (a[j] != 0 && p[j] != 0) {
			nz++;
			if (a[j] == p[j]) {
				fl = true;
			}
			else {
				fl = false;
			}
		}
		j++;
	}
	if (nz == 0) {
		return 0;
	}
	else if (fl) {
		return 1;
	}
	else {
		return -1;
	}
}

//Evaluation of a clause in an assignment.
//A clause is true if one of its components is true.
int vluecl(int* a, int* c, int n) {
	int nz = 0;
	int j = 0;
	bool fl = false;
	while (fl == false && j < n) {
		if (a[j] != 0 && c[j] != 0) {
			nz++;
			if (a[j] == c[j]) {
				fl = true;
			}
			else {
				fl = false;
			}
		}
		j++;
	}
	if (nz == 0) {
		return 0;
	}
	else if (fl) {
		return 1;
	}
	else {
		return -1;
	}
}

//Evaluation of a disjunctive(logic or) form in an assignment.
//A disjunctive form is a list of phrases, interpreted
//as the dijunctive of its components.
int vluedf(int* a, int** df, int k, int n) {
	//k is the lenghth of df.
	int j = 0;
	bool fl = false;
	int ct = 0;
	int vlph;
	while (fl == false && j < k) {
		vlph = vlueph(a, df[j], n);

		if (vlph != 0) {
			ct++;
			if (vlph == 1) {
				fl = true;
			}
			else {
				fl = false;
			}
		}
		j++;
	}
	if (ct == 0) {
		return 0;
	}
	else if (fl) {
		return 1;
	}
	else {
		return -1;
	}
}

//Evaluation of a conjunctive(logic and) form in an assignment.
//A conjunctive form is a list of clauses, interpreted
//as the conjunction of its componnents.
int vluecf(int* a, int** cf, int k, int n) {
	//k is the length of cf.
	int j = 0;
	bool fl = true;
	int ct = 0;
	int vlcl;
	while (fl == true && j < k) {
		vlcl = vluecl(a, cf[j], n);
		if (vlcl != 0) {
			ct++;
			if (vlcl == 1) {
				fl = true;
			}
			else {
				fl = false;
			}
		}
		j++;
	}
	if (ct == 0) {
		return 0;
	}
	else if (fl) {
		return 1;
	}
	else {
		return -1;
	}
}

//Total evaluation of a conjuctive form in a list.
int* teval(int** cf, int k, int n) {
	//k is the length of cf, n is the length of each element of cf.
	//nl is the length of the list, namely, nl = 2^n - 1.
	int nl = 1;
	for (int i = 0; i < n; i++) {
		nl *= 2;
	}
	int* list = new int[nl];
	for (int i = 0; i < nl; i++) {
		list[i] = vluecf(br(i, n), cf, k, n);
	}
	return list;
}

//Generate a random instance of k-SAT over n variables with m clauses.
int** rndksgen(int n, int m, int k) {
	//Neccessary memory allocation for 2D array using new.
	int** cf = new int*[m];
	for (int i = 0; i < m; i++) {
		cf[i] = new int[k];
	}
	for (int i = 0; i < m; i++) {
		int* uv = new int[k] (); //Initialized to 0.
		int nuv = 0;
		while (nuv < k) {
			int v = rand() % n + 1;
			bool check = false;
			for (int i = 0; i < k; i++) {
				if (uv[i] == v) {
					check = true;
					break;
				}
			}
			if (check == false) {
				int ran = rand() % 2;
				cf[i][nuv] = (2 * ran - 1) * v;
				uv[nuv] = v;
				nuv++;
			}
		}
		delete[] uv;
	}
	return cf;
}

//Convert a random instance of KSAT in the conjunctive form
//representation given in the previous function rndksgen().
int** cvrt(int n, int m, int k, int** kcf) {
	//m is the length of the row of kcf.
	//k is the length of the line of kcf.
	//n is the length of the line we want to convert kcf to.
	int** cf = new int*[m];
	for (int i = 0; i < m; i++) {
		cf[i] = new int[n] ();
	}
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < k; j++) {
			int v = abs(kcf[i][j]);
			int sg = kcf[i][j]/v;
			cf[i][v - 1] = sg;
		}
	}
	return cf;
}

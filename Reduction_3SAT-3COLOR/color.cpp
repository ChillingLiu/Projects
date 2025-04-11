#include <iostream>
#include <string>
using namespace std;

//A struct that represents a vertex in graph.
typedef struct vertex {
	string name;
	string color;
	int edge_number;
	int* edges;
} Vertex;

//A class that represents an undirected graph.
class Graph {
public:
	int vertex_number;	//Number of vertices.
	int edge_number;	//Number/Maximum of edges.
	Vertex** vertices;	//Each vertex has its individual information.

	void setName(int v, string name);
	void setColor(int v, string color);
	void addEdge(int v, int w);
	int findVertex(string name);
	string findColor(string name);
	void display_graph();

	//Constructor of a k-Colorable Graph.
	//n = number of variables, m = number of clauses.
	//vertex_number = 2*n + 5*m + 3.
	//edge_number = 3*n + 10*m + 3.
	Graph(int n, int m) {
		vertex_number = 2*n + 5*m + 3;
		edge_number = 3*n + 10*m + 3;
		vertices = new Vertex*[vertex_number];
		for (int i = 0; i < vertex_number; i++) {
			vertices[i] = new Vertex;
			vertices[i]->edge_number = 0;
			vertices[i]->edges = new int[vertex_number]();
		}
		cout << "Graph created with number of verteices = "<< vertex_number
			<< ", and number of edges = " << edge_number << endl;
	}

	//Destructor of the class, free all memories that have been allocated.
	~Graph() {
		for (int i = 0; i < vertex_number; i++) {
			delete[] vertices[i]->edges;
			delete vertices[i];
		}
		cout << "Graph destroyed!" << endl;
	}
};

void Graph::setName(int v, string name) {
	vertices[v]->name = name;
}

void Graph::setColor(int v, string color) {
	vertices[v]->color = color;
}

void Graph::addEdge(int v, int w) {
	vertices[v]->edges[w] = 1;
	vertices[w]->edges[v] = 1;
	vertices[v]->edge_number++;
	vertices[w]->edge_number++;
}

int Graph::findVertex(string name) {
	for (int i = 0; i < vertex_number; i++) {
		if (vertices[i]->name == name) {
			return i;
		}
	}
	cout << "Find vertex failed! Program aborted!" << endl;
	exit(-1);
}

string Graph::findColor(string name) {
	for (int i = 0; i < vertex_number; i++) {
		if (vertices[i]->name == name) {
			return vertices[i]->color;
		}
	}
	cout << "Find color failed! Program aborted!" << endl;
	exit(-1);
}

void Graph::display_graph() {
	cout << "Display the converted Graph from Conjunctive Form: " << endl;
	cout << "        Graph Vertices: " << endl << "        ";
	for (int i = 0; i < vertex_number; i++) {
		if (vertices[i]->name.length() == 1) {
			cout << " ";
		}
		cout << vertices[i]->name << " ";
	}
	cout << endl;

	cout << "        Vertices Color: " << endl << "        ";
	for (int i = 0; i < vertex_number; i++) {
		cout << " " <<  vertices[i]->color << " ";
	}
	cout << endl;

	cout << "        Edges: " << endl;
	for (int i = 0; i < vertex_number; i++) {
		if(vertices[i]->name.length() == 1) {
			cout << " ";
		}
		cout << "        " << vertices[i]->name << ": {";
		for (int j = i; j < vertex_number; j++) {
			if (vertices[i]->edges[j] == 1) {
				cout << "(" << vertices[i]->name << "," << vertices[j]->name << ") ";
			}
		}
		cout << "}" << endl;
	}
}

//A clause is true if one of its components is true.
bool clauseAnswer(int* clasue, int n) {
	bool tmp = false;
	for (int i = 0; i < n; i++) {
		if (clasue[i] == 1) {
			tmp = true;
		}
	}
	return tmp;
}

//A conjunctive form is a list of clauses,
//it is true if every clause is true.
bool cfAnswer(int** cf, int n, int m) {
	bool tmp = true;
	for (int i = 0; i < m; i++) {
		if (!clauseAnswer(cf[i], n)) {
			tmp = false;
		}
	}
	return tmp;
}

//Generate a random instance of k-SAT over n variables with m clauses.
int** randomKSAT(int n, int m, int k) {
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
//representation given in the previous function randomKSAT().
int** ksat2CF(int n, int m, int k, int** kcf) {
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

//Display the random CF we created.
void display_cf(int** cf, int n, int m, int k, string** cf_alphabet) {
	cout << "Random Conjunctive Form: " << endl;
	for (int i = 0; i < m; i++) {
		cout << "| ";
		for (int j = 0; j < n; j++) {
			printf("%2d ", cf[i][j]);
		}
		cout << "|" << endl;
	}

	for (int i = 0; i < m; i++) {
		cout << "(";
		for (int j = 0; j < k; j++) {
			cout << cf_alphabet[i][j];
			if (j != k - 1) {
				cout << " v ";
			}
		}
		cout << ")";
		if (i != m - 1) {
			cout << " ^ ";
		}
	}
	cout << endl;
}

//Convert cf from form [0 1 -1 1] to [b -c -d].
string** convert_cf(int** cf, int n, int m, int k, string* alphabet) {
	string** tmp = new string*[m];
	for (int i = 0; i < m; i++) {
		tmp[i] = new string[k];
	}
	int cursor = 0;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (cf[i][j] == 0) {
				continue;
			}
			else if (cf[i][j] == 1) {
				tmp[i][cursor++] = alphabet[j];
			}
			else if (cf[i][j] == -1) {
				tmp[i][cursor++] = "-" + alphabet[j];
			}
		}
		cursor = 0;
	}
	return tmp;
}

//Return a random color except "F" for nodes x2 when first 2
//components have different color, if it returns a "F" for x2,
//it will has the same effect with "the first 2 components are "F",
//then if the third component are also "F", it will be like 3 "F",
//and the result will be "False" even we have a component of "T".
string randomColor() {
	int tmp = rand() % 2;
	if (tmp == 0) {
		return "X";
	}
	else {
		return "T";
	}
}

string diffColor1(string color) {
	int tmp = rand() % 2;
	if (color == "X") {
		if (tmp == 0) {
			return "T";
		}
		else {
			return "F";
		}
	}
	else if (color == "T") {
		if (tmp == 0) {
			return "X";
		}
		else {
			return "F";
		}
	}
	else {
		if (tmp == 0) {
			return "X";
		}
		else {
			return "T";
		}
	}
}

string diffColor2(string color1, string color2) {
	if (color1 == "X" && color2 == "T") {
		return "F";
	}
	else if (color1 == "T" && color2 == "X") {
		return "F";
	}
	else if (color1 == "X" && color2 == "F") {
		return "T";
	}
	else if (color1 == "F" && color2 == "X") {
		return "T";
	}
	else if (color1 == "T" && color2 == "F") {
		return "X";
	}
	else if (color1 == "F" && color2 == "T") {
		return "X";
	}
	else if (color1 == "T" && color2 == "T") {
		return diffColor1("T");
	}
	else if (color1 == "F" && color2 == "F") {
		return diffColor1("F");
	}
	else if (color1 == "X" && color2 == "X") {
		return diffColor1("X");
	}
	else {
		cout << "diffColor2() failed!" << endl;
		return "T";
	}
}

string diffColor3(string color1, string color2, string color3) {
	if (color1 == color2) {
		return diffColor2(color1, color3);
	}
	else if (color1 == color3) {
		return diffColor2(color1, color2);
	}
	else if (color2 == color3) {
		return diffColor2(color1, color2);
	}
	else {
		cout << "A situation adjacent vertices have the same color occurs!" << endl;
		return "T";
	}
}

//Transformation of a 3CNF to a Graph.
void transformation(int n, int m, int k, string** cf_alphabet, Graph* graph, string* alphabet) {
	//Vertices 0 - 2 are T, F, X. T for true, F for false, X for Other
	graph->setName(0, "X");
	graph->setName(1, "T");
	graph->setName(2, "F");
	graph->setColor(0, "X");
	graph->setColor(1, "T");
	graph->setColor(2, "F");
	graph->addEdge(0, 1);
	graph->addEdge(0, 2);
	graph->addEdge(1, 2);

	//Vertices 3 - 2n + 2 for each variables and its negation based on cf.
	for (int i = 2; i < n + 2; i++) {
		graph->setName(2*i - 1, alphabet[i - 2]);
		graph->setName(2*i, "-" + alphabet[i - 2]);
		graph->setColor(2*i - 1, "T");
		graph->setColor(2*i, "F");
		graph->addEdge(2*i - 1, 0);
		graph->addEdge(2*i, 0);
		graph->addEdge(2*i - 1, 2*i);
	}

	//m clauses, each clauses need 5 nodes: x1 - x5.
	//Vertices 2n + 3 - 2n + 3 + 5m.
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < 5; j++) {
			graph->setName(2*n + 3 + 5*i + j, to_string(i) + to_string(j));
		}
	}

	//A clause a v b v -c:
	//first 2 elements in the clause have the same color,
	//means x2 have the same color,
	//otherwise, x2 choose an arbitrary color.
	for (int i = 0; i < m; i++) {
		//For nodes x0, x1, x2.
		if (cf_alphabet[i][0].length() == cf_alphabet[i][1].length()) {
			graph->setColor(graph->findVertex(to_string(i) + to_string(2)), graph->findColor(cf_alphabet[i][0]));
		}
		else {
			graph->setColor(graph->findVertex(to_string(i) + to_string(2)), randomColor());
		}
		graph->setColor(graph->findVertex(to_string(i) + to_string(0)), diffColor1(graph->findColor(to_string(i) + to_string(2))));
		graph->setColor(graph->findVertex(to_string(i) + to_string(1)), diffColor2(graph->findColor(to_string(i) + to_string(2)), graph->findColor(to_string(i) + to_string(0))));
		graph->addEdge(graph->findVertex(cf_alphabet[i][0]), graph->findVertex(to_string(i) + to_string(0)));
		graph->addEdge(graph->findVertex(cf_alphabet[i][1]), graph->findVertex(to_string(i) + to_string(1)));
		graph->addEdge(graph->findVertex(to_string(i) + to_string(0)), graph->findVertex(to_string(i) + to_string(1)));
		graph->addEdge(graph->findVertex(to_string(i) + to_string(0)), graph->findVertex(to_string(i) + to_string(2)));
		graph->addEdge(graph->findVertex(to_string(i) + to_string(1)), graph->findVertex(to_string(i) + to_string(2)));

		//For nodes x3, x4.
		//Try to let x4 be the same color with x2, in order to let x3 choose color easier.
		if (graph->findColor(to_string(i) + to_string(2)) != graph->findColor(cf_alphabet[i][2]) && graph->findColor(to_string(i) + to_string(2)) != "T") {
		graph->setColor(graph->findVertex(to_string(i) + to_string(4)), graph->findColor(to_string(i) + to_string(2)));
		}
		else {
			graph->setColor(graph->findVertex(to_string(i) + to_string(4)), diffColor2(graph->findColor(cf_alphabet[i][2]), "T"));
		}
		graph->setColor(graph->findVertex(to_string(i) + to_string(3)), diffColor3(graph->findColor(to_string(i) + to_string(2)), graph->findColor(to_string(i) + to_string(4)), "T"));
		graph->addEdge(graph->findVertex(cf_alphabet[i][2]), graph->findVertex(to_string(i) + to_string(4)));
		graph->addEdge(graph->findVertex(to_string(i) + to_string(3)), graph->findVertex(to_string(i) + to_string(2)));
		graph->addEdge(graph->findVertex(to_string(i) + to_string(3)), graph->findVertex(to_string(i) + to_string(4)));
		graph->addEdge(graph->findVertex(to_string(i) + to_string(3)), 1);
		graph->addEdge(graph->findVertex(to_string(i) + to_string(4)), 1);
	}
}

bool checkColor(Graph* graph, int m) {
	bool tmp = true;
	for (int i = 0; i < m; i++) {
		if (graph->findColor(to_string(i) + to_string(3)) == graph->findColor(to_string(i) + to_string(4))) {
			tmp = false;
			cout << "Vertex " << i << 3 << " has the same color with Vertex " << i << 4 << endl;
		}
		if (graph->findColor(to_string(i) + to_string(3)) == "T") {
			tmp = false;
			cout << "Vertex " << i << 3 << " has the same color with Vertex T" << endl;
		}
		if (graph->findColor(to_string(i) + to_string(4)) == "T") {
			tmp = false;
			cout << "Vertex " << i << 4 << " has the same color with Vertex T" << endl;
		}
	}
	return tmp;
}

int main() {
	//n = number of variables.
	//m = number of clauses.
	//k = length of clauses.
	int n = 4;
	int m = 4;
	int k = 3;
	srand(time(NULL));
	int** kins = randomKSAT(n, m, k);
	int** cf = ksat2CF(n, m, k, kins);

	string alphabet[10] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};
	string** cf_alphabet = convert_cf(cf, n, m, k, alphabet);
	display_cf(cf, n, m, k, cf_alphabet);
	cout << "The assignment of the random 3CNF is: ";
	if (cfAnswer(cf, n, m)) {
		cout << "True!" << endl << endl;
	}
	else {
		cout << "False!" << endl << endl;
	}

	Graph* graph = new Graph(n, m);
	transformation(n, m, k, cf_alphabet, graph, alphabet);
	graph->display_graph();
	cout << "The assignment of the converted Graph is: " << endl;
	if (checkColor(graph, m)) {
		cout << "True, it is 3-Colorable!" << endl << endl;
	}
	else {
		cout << "False, it is not 3-Colorable!" << endl << endl;
	}

	//Free memories.
	for (int i = 0; i < m; i++) {
		delete[] cf_alphabet[i];
	}
	delete[] cf_alphabet;
	delete graph;
	return 0;
}

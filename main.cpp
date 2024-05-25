/*
main: Rima Ali
jens: Rima Ali
dfs (gabow): Karam Ghanem & Nagham Laila
gabow: Rimaz Salman & Nazha Hammad
transformToUndirected: Al-Hasan Issa
dfs (jens): Rima Ali
dfs (chains): Rima Ali & Fouad Sabbouh
transformToDirected: Mohammed Dawwa
erase: Wissam Skeif
checkCharacter: Yamama Al-Hejri
read: Rima Ali & Sawsan Kasem & Hala Othman
*/

#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include <fstream>
#include <string>
#include <map>
using namespace std;

//gabow dfs
void dfs(vector<vector<int>>& graph, int vertex, int& dfs_counter, int& finish_counter, int* dfs_number,
	int* finish_number, string* state, stack<int>& ostack, stack<int>& rstack, int* comp, bool* vINo) {
	//Karam Ghanem
	state[vertex] = "active";
	dfs_number[vertex] = dfs_counter++;
	ostack.push(vertex); 
	rstack.push(vertex);
	vINo[vertex] = true;
	for (int i = 0; i < graph[vertex].size(); i++) {
		int w = graph[vertex][i];
		if (state[w] == "new") dfs(graph, w, dfs_counter, finish_counter, dfs_number, finish_number, state, ostack, rstack, comp, vINo);
		else {
			if (vINo[w] == true) {
				while (dfs_number[w] < dfs_number[rstack.top()]) rstack.pop();
			}
		}
	}
	//Nagham Laila
	state[vertex] = "finished";
	finish_number[vertex] = finish_counter++;
	if (vertex == rstack.top()) {
		rstack.pop();
		while (ostack.top() != vertex) {
			int w = ostack.top();
			ostack.pop();
			vINo[w] = false;
			comp[w] = vertex;
		}
		comp[vertex] = vertex;
		ostack.pop();
		vINo[vertex] = false;
	}
}

//strongly connected
bool gabow(vector<vector<int>>graph, int n) {
	//Nazha Hammad
	int dfs_counter = 0, finish_counter = 0;
	int* dfs_number = new int[n] {};
	int* finish_number = new int[n] {};
	stack<int> rstack, ostack;
	string* state = new string[n];
	bool* vINo = new bool[n] {};
	for (int i = 0; i < n; i++) state[i] = "new";
	int* components = new int[n] {};
	for (int i = 0; i < n; i++) {
		if (state[i] == "new") {
			dfs(graph, i, dfs_counter, finish_counter, dfs_number, finish_number, state, ostack, rstack, components, vINo);
		}
	}
	//Rimaz Salman
	set<int>s;
	for (int i = 0; i < n; i++) {
		s.insert(components[i]);
	}
	if (s.size() == 1) return true;
	else return false;
}

//convert graph from directed to undirected
void transformToUndirected(vector<vector<int>>& graph, int n) {
	for (int i = 0; i < n; i++) {
		int len = graph[i].size();
		for (int j = 0; j < len; j++) {
			int from = i;
			int to = graph[i][j];
			graph[to].push_back(from);
		}
	}
	for (int i = 0; i < n; i++) {
		int len = graph[i].size();
		set<int> s;
		vector<int> v;
		for (int j = 0; j < len; j++) {
			int size1 = s.size();
			s.insert(graph[i][j]);
			int size2 = s.size();
			if (size1 != size2) v.push_back(graph[i][j]);
		}
		graph[i] = v;
	}
}

//jens dfs
void dfs(vector<vector<int>>& graph, int vertex, bool*& visited, set<pair<int, int>>& TE, 
	     int& dfs_counter, int*& dfs_number, int*& backward, stack<int>& s) {
	visited[vertex] = true;
	s.push(vertex);
	dfs_number[vertex] = dfs_counter++;
	int len = graph[vertex].size();
	for (int i = 0; i < len; i++) {
		int to = graph[vertex][i];
		if (!visited[to]) {
			TE.insert({ vertex,to });
			dfs(graph, to, visited, TE, dfs_counter, dfs_number, backward, s);
		}
		else {
			if (TE.find({ to,vertex }) != TE.end() || backward[to] != 0) continue;
			else backward[to] = backward[to] + 1;
		}
	}
}

//chain dfs
void dfs(vector<vector<int>>& graph, int vertex, bool*& visited, bool& q, vector<int>& chain) {
	if (q == true) return; //Rima Ali
	if (visited[vertex] == true) { q = true; chain.push_back(vertex); return; } //Rima Ali
	visited[vertex] = true; //Fouad Sabbouh
	chain.push_back(vertex); //Fouad Sabbouh
	int len = graph[vertex].size(); //Fouad Sabbouh
	for (int i = 0; i < len; i++) {
		if (q == true) { return; } //Rima Ali
		int v = graph[vertex][i]; //Fouad Sabbouh
		if (!visited[v]) dfs(graph, v, visited, q, chain); //Fouad Sabbouh
		else { q = true; chain.push_back(v); return; } //Rima Ali
	}
}

//convert graph from undirected to directed
void transformToDirected(vector<vector<int>>& graph, int n, set<pair<int, int>> TE, int* dfs_number, int* backward) {
	vector<vector<int>> graph2(n);
	for (int i = 0; i < n; i++) {
		int len = graph[i].size();
		for (int j = 0; j < len; j++) {
			int from = i, to = graph[i][j];
			if (TE.find({ from,to }) != TE.end()) graph2[to].push_back(from);
			else if (TE.find({ to,from }) != TE.end()) continue;
			else if (dfs_number[from] > dfs_number[to]) {
				if (backward[from] > 0) { backward[from]--; backward[to]++; }
				graph2[to].push_back(from);
			}
			else {
				if (backward[to] > 0) backward[to]--; backward[from]++;
				graph2[from].push_back(to);
			}
		}
	}
	for (int i = 0; i < n; i++) {
		int len = graph2[i].size();
		set<int> s;
		vector<int> v;
		for (int j = 0; j < len; j++) {
			int size1 = s.size();
			s.insert(graph2[i][j]);
			int size2 = s.size();
			if (size1 != size2) v.push_back(graph2[i][j]);
		}
		graph2[i] = v;
	}
	graph = graph2;
}

//biconnected
bool jens(vector<vector<int>>graph, int n) {
	transformToUndirected(graph, n);
	bool* visited = new bool[n] {};
	set<pair<int, int>> TE;
	int dfs_counter = 0;
	int* dfs_number = new int[n] {};
	int* backward = new int[n] {};
	stack<int>s;
	dfs(graph, 0, visited, TE, dfs_counter, dfs_number, backward, s);
	for (int i = 0; i < n; i++) {
		if (visited[i] == false) return false;
	}
	transformToDirected(graph, n, TE, dfs_number, backward);
	stack<int>s2;
	while (!s.empty()) {
		int e = s.top();
		s.pop();
		if(backward[e] > 0) s2.push(e);
	}
	visited = new bool[n] {};
	int k = 0;
	int edges = 0;
	int total = 0;
	while(!s2.empty()) {
		int e = s2.top();
		s2.pop();
		visited[e] = true;
		int len = graph[e].size();
		for (int i = 0; i < len; i++) {
			int to = graph[e][i];
			if (dfs_number[e] > dfs_number[to]) continue;
			bool q = false;
			vector<int> chain;
			chain.push_back(e);
			dfs(graph, to, visited, q, chain);
			int l = chain.size();
			if (k != 0 && chain[0] == chain[l - 1]) return false;
			edges += (l - 1);
			k++;
		}
	}
	for (int i = 0; i < n; i++) {
		total += graph[i].size();
	}
	if (total == edges) return true;
	else return false;
}

//delete vertex
void erase(vector<vector<int>>& graph, int& n, int v) {
	vector<vector<int>>graph2(n - 1);
	for (int i = 0; i < n; i++) {
		if (i == v) continue;
		int len = graph[i].size();
		for (int j = 0; j < len; j++) {
			int to = graph[i][j];
			if (to == v) continue;
			if (to > v) to--;
			if (i < v) graph2[i].push_back(to);
			else graph2[i - 1].push_back(to);
		}
	}
	graph = graph2;
	n -= 1;
}

//checks if character is a number
bool checkCharacter(char x) {
	for (char i = '0'; i <= '9'; i++) {
		if (x == i) return true;
	}
	return false;
}

//reads SNAP file
void read(string fileName, vector<vector<int>>& graph, int& n) {
	//Sawsan Kasem
	fstream myfile;
	map<int, int>m;
	int number = 0;
	set<int> unique;
	vector<pair<int, int>> edges;
	//Rima Ali
	myfile.open(fileName, ios::in);
	if (myfile.is_open()) {
		string s;
		while (getline(myfile, s)) {
			if (s[0] == '#') continue;
			string from = "", to = "";
			int len = s.length();
			int space;
			for (int i = 0; i < len; i++) {
				if (checkCharacter(s[i])) from += s[i];
				else { space = i; break; };
			}
			for (int i = space; i < len; i++) {
				if (checkCharacter(s[i]) == false) continue;
				else { space = i; break; }
			}
			for (int i = space; i < len; i++) to += s[i];
			int v1 = stoi(from), v2 = stoi(to);
			int before = unique.size();
			unique.insert(v1);
			int after = unique.size();
			if (before != after) m[v1] = number++;
			before = unique.size();
			unique.insert(v2);
			after = unique.size();
			if (before != after) m[v2] = number++;
			edges.push_back({ m[v1],m[v2] });
		}
		//Hala Othman
		n = m.size();
		vector<vector<int>> graph2(n);
		int lenn = edges.size();
		for (int i = 0; i < lenn; i++) {
			graph2[edges[i].first].push_back(edges[i].second);
		}
		graph = graph2;
	}
	else cout << "error opening file\n";
}

int main() {
	vector<vector<int>> graph; int n;
	string graphs[6] = { "p2p-Gnutella04.txt", "p2p-Gnutella05.txt", "p2p-Gnutella06.txt", "p2p-Gnutella08.txt", "p2p-Gnutella09.txt", "p2p-Gnutella24.txt" };
	for (int i = 0; i < 6; i++) {
		cout << graphs[i] << '\n';
		read(graphs[i], graph, n);
		cout << "Doing Gabow\n";
		bool v1 = gabow(graph, n);
		cout << "Doing Jens\n";
		bool v2 = jens(graph, n);
		if (v1 == true && v2 == true) {
			cout << "Strongly Biconnected\n";
			bool t = true;
			for (int i = 0; i < n; i++) {
				vector<vector<int>>g2 = graph;
				int n2 = n;
				erase(g2, n2, i);
				if (jens(g2, n2) == false) { t = false; break; }
			}
			if (t) cout << "2-Vertex Strongly Biconnected\n";
			else cout << "Not 2-Vertex Strongly Biconnected\n";
		}
		else {
			cout << "Not Strongly Biconnected\n";
		}
	}
	return 0;
}
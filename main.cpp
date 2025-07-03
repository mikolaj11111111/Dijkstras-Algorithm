#include<iostream>
#include<fstream>
#include<vector>
#include<list>

using namespace std;

//Macierz Sasiedztwa
vector<vector<int>> AdjacencyMatrix(int rows, int columns) {
	ifstream adjacency_matrix_data("data.txt");
	if (adjacency_matrix_data) {
		cout << "Plik otwarty\n";
		vector<vector<int>> adjacency_matrix(rows, vector<int>(columns, 0));

		//Insert Values
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				adjacency_matrix_data >> adjacency_matrix[i][j];
			}
		}

		return adjacency_matrix;

	}
	else {
		cout << "Nie udalo sie otworzyc pliku\n";
		return {};
	}

}


struct Edge {
	int from, to, weight;
};

list<Edge> EdgeListBasedOnAM(vector<vector<int>>& adjacency_matrix, int rows, int columns) {
	list<Edge> edge_list;
	//Insert Values
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			if (adjacency_matrix[i][j] != 0) {
				edge_list.push_back({ i + 1, j + 1, adjacency_matrix[i][j] });
			}
		}
	}
	//Show Values
	for (auto& edge : edge_list) {
		cout << "od: " << edge.from << " do: " << edge.to << " waga: " << edge.weight << endl;
	}
	return edge_list;
}


vector<vector<pair<int, int>>> AdjacencyListBasedOnEL(list<Edge> edge_list, int rows, int columns) {
	vector<vector<pair<int, int>>> adjacency_list(rows +1);
	//Get Values From Edge List
	for (auto& edge : edge_list) {
		int from = edge.from;
		int to = edge.to;
		int weight = edge.weight;

		adjacency_list[from].emplace_back(to, weight);

	}

	//Show Values
	cout << "Lista sasiedztwa:\n";
	for (int i = 0; i < rows; i++) {
		cout << i << ": ";
		for (auto& neighbor : adjacency_list[i]) {
			cout << "from: " << neighbor.first << " weight: " << neighbor.second << ", ";
		}
		cout << endl;
	}
	return adjacency_list;
}

struct NodesInfo {
	int end = 0;
	int start = -1;
	int distance = -1;
};

void Dijkstry(vector<vector<pair<int, int>>> adjacency_list, int vertices, int start_vertex) {
	vector<int> distances(vertices + 1, INT_MAX); // Tablica odleg³oœci
	distances[start_vertex] = 0;

	vector<bool> visited(vertices + 1, false); // Tablica odwiedzonych wierzcho³ków
	vector<struct NodesInfo> nodes; // Lista wêz³ów z informacjami

	// Inicjalizacja listy wêz³ów
	for (int i = 1; i <= vertices; i++) {
		if (i == start_vertex) {
			nodes.push_back({ i, NULL, distances[i] });
		}
		else {
			nodes.push_back({ i, -1, distances[i] });
		}
	}

	while (true) {
		// ZnajdŸ wêze³ z najmniejsz¹ odleg³oœci¹
		int min_distance = INT_MAX;
		int current_index = -1;

		for (int i = 0; i < nodes.size(); i++) {
			if (!visited[nodes[i].end] && nodes[i].distance < min_distance) {
				min_distance = nodes[i].distance;
				current_index = i;
			}
		}

		// Jeœli nie znaleziono wierzcho³ka, zakoñcz
		if (current_index == -1) break;

		// Przetwarzaj bie¿¹cy wierzcho³ek
		int current_vertex = nodes[current_index].end;
		visited[current_vertex] = true;

		for (auto& neighbor : adjacency_list[current_vertex]) {
			int next_vertex = neighbor.first;
			int weight = neighbor.second;

			if (!visited[next_vertex] && distances[current_vertex] + weight < distances[next_vertex]) {
				distances[next_vertex] = distances[current_vertex] + weight;

				// Aktualizuj wêze³ w liœcie `nodes`
				for (auto& node : nodes) {
					if (node.end == next_vertex) {
						node.distance = distances[next_vertex];
						node.start = current_vertex;
					}
				}
			}
		}
	}

	// Wyœwietl wyniki
	cout << "Najkrotsze odleglosci od wierzcholka " << start_vertex << ":\n";
	for (int i = 1; i <= vertices; i++) {
		if (distances[i] == INT_MAX) {
			cout << "Do wierzcholka " << i << ": brak polaczenia\n";
		}
		else {
			cout << "Do wierzcholka " << i << ": " << " dystans: " << distances[i] << "\n";
		}
	}
}




int main() {
	int rows = 8;
	int vertices = 8;
	int columns = 8;

	//Lista krawedzi na podstawie macierzy sasiedztwa
	vector<vector<int>> adjacency_matrix = AdjacencyMatrix(rows, columns);
	list<Edge> edge_list = EdgeListBasedOnAM(adjacency_matrix, rows, columns);

	//Lista sasiedztwa na podstawie listy krawedzi
	vector<vector<pair<int, int>>> adjency_list = AdjacencyListBasedOnEL(edge_list, rows, columns);
	
	int start_vertex = 1; // Wierzcho³ek startowy
	Dijkstry(adjency_list, vertices, start_vertex);

	return 0;

}
// Dijkstra's algorithm on the Lake Malawi graph (20 nodes, 32 edges)
// Weights are stored in km. Distances are shown in drawing units (km / 10)
// so they match the scale used on the drawn graph.

#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>
using namespace std;

const int N = 20;          // number of nodes A..T
const int INF = 1000000;   // "infinity"

string names[N] = {
    "Monkey Bay", "Cape Maclear", "Makanjila", "Domwe Island", "Mumbo Island",
    "Chipoka", "Meponda", "Nankoma Island", "Senga Bay", "Nkhotakota",
    "Metangula", "Cobue", "Chizumulu Island", "Likoma Island", "Nkhata Bay",
    "Usisya", "Mbamba Bay", "Chilumba", "Matema", "Niambi"
};

int graph[N][N];           // graph[a][b] = distance in km, 0 means no edge

int dist[N];               // shortest distance found so far
int prevNode[N];           // node we came from
bool done[N];              // true when the node is finished

// the graph is undirected so the edge goes both ways
void addEdge(char a, char b, int km) {
    graph[a - 'A'][b - 'A'] = km;
    graph[b - 'A'][a - 'A'] = km;
}

void setupGraph() {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            graph[i][j] = 0;

    addEdge('A', 'B', 10);   addEdge('A', 'C', 50);   addEdge('B', 'D', 5);
    addEdge('D', 'C', 45);   addEdge('D', 'E', 8);    addEdge('D', 'F', 33);
    addEdge('E', 'G', 64);   addEdge('E', 'F', 25);   addEdge('C', 'G', 20);
    addEdge('F', 'H', 18);   addEdge('F', 'I', 40);   addEdge('H', 'G', 55);
    addEdge('H', 'I', 20);   addEdge('G', 'K', 50);   addEdge('I', 'K', 130);
    addEdge('I', 'J', 65);   addEdge('K', 'O', 110);  addEdge('K', 'L', 35);
    addEdge('L', 'M', 20);   addEdge('L', 'N', 8);    addEdge('M', 'N', 13);
    addEdge('J', 'N', 100);  addEdge('J', 'O', 50);   addEdge('N', 'O', 70);
    addEdge('O', 'P', 35);   addEdge('P', 'Q', 100);  addEdge('N', 'Q', 100);
    addEdge('Q', 'T', 70);   addEdge('P', 'R', 75);   addEdge('R', 'T', 100);
    addEdge('R', 'S', 107);  addEdge('S', 'T', 48);
}

int numberofstopes(int v){
    int stops = 0;
    while (prevNode[v] != -1){
        stops++;
        v = prevNode[v];
    }
    return stops;
}

// print a distance as drawing units (km divided by 10) and also in km
void printDistance(int km) {
    cout << fixed << setprecision(1) << km / 10.0 << " units (" << km << " km)";
}

void dijkstra(int source, bool showSteps) {
    for (int i = 0; i < N; i++) {
        dist[i] = INF;
        prevNode[i] = -1;
        done[i] = false;
    }
    dist[source] = 0;

    for (int count = 0; count < N; count++) {
        // find the unfinished node with the smallest distance
        int u = -1;
        for (int i = 0; i < N; i++) {
            if (!done[i] && (u == -1 || dist[i] < dist[u]))
                u = i;
        }
        if (u == -1 || dist[u] == INF) break;   // nothing left that we can reach
        done[u] = true;

        if (showSteps) {
            cout << "Step " << count + 1 << ": pick " << char('A' + u)
                 << " (" << names[u] << "), distance ";
            printDistance(dist[u]);
            cout << "\n";
        }

        // look at every neighbour of u
        for (int v = 0; v < N; v++) {
            if (graph[u][v] == 0 || done[v]) continue;

            int newDist = dist[u] + graph[u][v];
            if (newDist < dist[v]) {
                if (showSteps) {
                    cout << "    " << char('A' + v) << ": ";
                    if (dist[v] == INF) cout << "inf";
                    else cout << dist[v] / 10.0;
                    cout << " -> " << newDist / 10.0 << " (through "
                         << char('A' + u) << ")\n";
                }
                dist[v] = newDist;
                prevNode[v] = u;
            }
        }
    }
}

// prints the route by going back through prevNode
void printPath(int v) {
    if (prevNode[v] != -1) {
        printPath(prevNode[v]);
        cout << " -> ";
    }
    cout << char('A' + v);
}

void printPathNames(int v) {
    if (prevNode[v] != -1) {
        printPathNames(prevNode[v]);
        cout << " -> ";
    }
    cout << names[v];
}

void showNodes() {
    cout << "\nNodes:\n";
    for (int i = 0; i < N; i++)
        cout << "  " << char('A' + i) << " = " << names[i] << "\n";
}

void showEdges() {
    cout << "\nEdges (distance in drawing units, km in brackets):\n";
    int count = 0;
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            if (graph[i][j] != 0) {
                count++;
                cout << "  " << char('A' + i) << "-" << char('A' + j) << "  "
                     << names[i] << " - " << names[j] << "  ";
                printDistance(graph[i][j]);
                cout << "\n";
            }
        }
    }
    cout << "Total edges: " << count << "\n";
}

// reads a whole line, removes spaces at the start and end
// returns false when the input has finished
bool readLine(string message, string &text) {
    cout << message;
    if (!getline(cin, text)) return false;

    size_t first = text.find_first_not_of(" \t\r");
    if (first == string::npos) {
        text = "";                      // nothing but spaces
    } else {
        size_t last = text.find_last_not_of(" \t\r");
        text = text.substr(first, last - first + 1);
    }
    return true;
}

// ask the user for a node letter, keeps asking until it is valid
int readNode(string message) {
    string input;
    while (true) {
        if (!readLine(message, input)) return -1;      // input finished
        if (input == "") {
            cout << "You entered nothing. Please enter a single letter from A to T.\n";
        } else if (input.length() == 1 && toupper(input[0]) >= 'A' && toupper(input[0]) <= 'T') {
            return toupper(input[0]) - 'A';
        } else {
            cout << "Please enter a single letter from A to T.\n";
        }
    }
}

void showAllFrom(int source) {
    cout << "\nShortest distances from " << char('A' + source) << " ("
         << names[source] << "):\n";
    for (int i = 0; i < N; i++) {
        if (i == source) continue;
        cout << "  " << char('A' + i) << " " << left << setw(17) << names[i];
        if (dist[i] == INF) {
            cout << "no route\n";
        } else {
            cout << right << setw(6) << fixed << setprecision(1) << dist[i] / 10.0
                 << " units  (" << setw(3) << dist[i] << " km)   ";
            printPath(i);
            cout << "\n";
        }
    }
}

int main() {
    setupGraph();

    cout << "Lake Malawi network - Dijkstra's shortest path\n";
    cout << "Distances are shown in drawing units (1 unit = 10 km).\n";
    showNodes();

    string choice = "";
    while (choice != "5") {
        cout << "\n1. Shortest route between two nodes\n";
        cout << "2. Shortest distances from one node to all the others\n";
        cout << "3. Show the steps of the algorithm\n";
        cout << "4. Show all edges\n";
        cout << "5. Exit\n";
        if (!readLine("Choose: ", choice)) break;     // end of input

        if (choice == "") {
            cout << "You entered nothing. Please enter a number from 1 to 5.\n";
        } else if (choice == "1") {
            int s = readNode("Start node (A-T): ");
            int t = readNode("End node (A-T): ");
            if (s < 0 || t < 0) break;
            dijkstra(s, false);
            cout << "\nFrom " << names[s] << " to " << names[t] << ":\n";
            if (dist[t] == INF) {
                cout << "  no route\n";
            } else {
                cout << "  Distance: ";
                printDistance(dist[t]);
                cout << "\n  Route: ";
                printPath(t);
                cout << "\n  Places: ";
                printPathNames(t);
                cout << "\n  Number of stops made: " << numberofstopes(t) << "\n";
            }
        } else if (choice == "2") {
            int s = readNode("Start node (A-T): ");
            if (s < 0) break;
            dijkstra(s, false);
            showAllFrom(s);
        } else if (choice == "3") {
            int s = readNode("Start node (A-T): ");
            if (s < 0) break;
            cout << "\n";
            dijkstra(s, true);
            showAllFrom(s);
        } else if (choice == "4") {
            showEdges();
        } else if (choice != "5") {
            cout << "Please enter a number from 1 to 5.\n";
        }
    }

    cout << "Goodbye.\n";
    return 0;
}

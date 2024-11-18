/*
* Actividad integradora 2
*
* Autores: 
* * Fernando Morán        A01284623
* * Julen Hoppenstedt     A01174098
*
* Fecha: 16/11/2024
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <unordered_map>
#include <fstream>

#define MAX 1000
#define INF INT_MAX

using namespace std;

struct colonia {
    string nombre;
    int x, y;
    bool central;

    colonia() {
        this->nombre = "";
        this->x = 0;
        this->y = 0;
    }

    colonia(string _nombre, int _x, int _y, int _central) {
        this->nombre = _nombre;
        this->x = _x;
        this->y = _y;
        if(_central==0){
            this->central=false;
        }else{
            this->central=true;
        }
    }
};

struct Edge {
    int u, v, cost;
    bool isNew; // Identifica si es una conexión existente con nuevo cableado

    Edge(int _u, int _v, int _cost, bool _isNew){
        this->u=_u;
        this->v=_v;
        this->cost=_cost;
        this->isNew=_isNew;
        
    }
        
};

struct DisjointSets {
    int *parent, *rank;
    int n;
    int find(int u){
            if(u!=parent[u]){
                parent[u]=find(parent[u]);
            }
            return parent[u];
        }

    void merge(int x,int y){
            x=find(x);
            y=find(y);
            if(rank[x]>rank[y]){
                parent[y]=x;
            }else{
                parent[x]=y;
            }
            if(rank[x]==rank[y]){
                rank[y]++;
            }
        }

    DisjointSets(int n){
        this->n=n;
        parent = new int[n+1];
        rank= new int[n+1];
        for (int i=0;i<=n;i++){
            rank[i]=0;
            parent[i]=i;
        }
        //para encontrar el parent
    }
};

struct Graph {
    int V;
    vector<Edge> edges;
    int matAdj[MAX][MAX];

    Graph(int V) {
        this->V = V;
        iniciaMat(this->matAdj);
    }

    void addEdge(int u, int v, int cost, bool isNew = false) {
        edges.emplace_back(u, v, cost, isNew);
        matAdj[u + 1][v + 1] = cost;
        matAdj[v + 1][u + 1] = cost;
    }

    vector<Edge> KruskalMST() {
        sort(edges.begin(), edges.end(), [](Edge a, Edge b) { return a.cost < b.cost; });
        DisjointSets ds(V);

        vector<Edge> mstEdges;
        for (auto &edge : edges) {
            int setU = ds.find(edge.u);
            int setV = ds.find(edge.v);
            if (setU != setV) {
                mstEdges.push_back(edge);
                ds.merge(setU, setV);
            }
        }
        return mstEdges;
    }

    void iniciaMat(int matAdj[MAX][MAX]) {
        for (int i = 0; i < MAX; i++) {
            matAdj[i][i] = 0;

            for (int j = i + 1; j < MAX; j++) {
                matAdj[i][j] = matAdj[j][i] = INF;
            }
        }
    }

    void print() {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                cout <<(matAdj[i][j] != INF ? matAdj[i][j] : -1) << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
};

// distancia
int getDistancia(const colonia &a, const colonia &b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

struct tspNode {
    int niv;                        // Nivel del árbol de búsqueda
    int costoAcum;                  // Costo acumulado
    int costoPos;                   // Estimación del costo posible
    int verticeActual;              // Nodo actual
    vector<bool> visitados;         // Nodos visitados
    vector<string> ruta;            // Ruta hasta el nodo actual

    tspNode(int n) : visitados(n, false), costoAcum(0), costoPos(0), verticeActual(0) {}

    // Orden de prioridad en la cola: menor costoPos tiene mayor prioridad
    bool operator<(const tspNode& otro) const {
        return costoPos > otro.costoPos;
    }
};

void calculaCostoPosible(tspNode& nodoActual, int matAdj[MAX][MAX], int n) {
    nodoActual.costoPos = nodoActual.costoAcum;

    // Añadir el costo estimado mínimo desde cada nodo no visitado
    for (int i = 1; i <= n; i++) {
        if (!nodoActual.visitados[i]) {
            int costoMin = INF;
            for (int j = 1; j <= n; j++) {
                if (!nodoActual.visitados[j] && matAdj[i][j] != INF) {
                    costoMin = min(costoMin, matAdj[i][j]);
                }
            }
            if (costoMin != INF) {
                nodoActual.costoPos += costoMin;
            }
        }
    }
}

pair<int, vector<string>> tsp(int matAdj[MAX][MAX], int n, const vector<colonia>& colonias) {
    int costoOptimo = INF;
    vector<string> rutaOptima;

    // Encontrar el primer nodo no central
    int nodoInicial = -1;
    for (int i = 1; i <= n; i++) {
        if (!colonias[i - 1].central) {
            nodoInicial = i;
            break;
        }
    }

    if (nodoInicial == -1) {
        cerr << "No hay nodos no centrales disponibles" << endl;
        return {INF, {}};
    }

    // Inicializar el nodo raíz
    tspNode raiz(n + 1);
    raiz.niv = 1;
    raiz.costoAcum = 0;
    raiz.verticeActual = nodoInicial;
    raiz.visitados[nodoInicial] = true;
    raiz.ruta.push_back(colonias[nodoInicial - 1].nombre);

    calculaCostoPosible(raiz, matAdj, n);

    priority_queue<tspNode> pq;
    pq.push(raiz);

    while (!pq.empty()) {
        tspNode current = pq.top();
        pq.pop();

        // Podar ramas con costos posibles mayores al óptimo actual
        if (current.costoPos >= costoOptimo) {
            continue;
        }

        // Verificar si todas las colonias no centrales han sido visitadas
        bool todasVisitadas = true;
        for (int i = 1; i <= n; i++) {
            if (!colonias[i - 1].central && !current.visitados[i]) {
                todasVisitadas = false;
                break;
            }
        }

        if (todasVisitadas) {
            // Calcular el costo para regresar al nodo inicial
            int costoDeVuelta = matAdj[current.verticeActual][nodoInicial];
            if (costoDeVuelta != INF) {
                int costoTotal = current.costoAcum + costoDeVuelta;
                if (costoTotal < costoOptimo) {
                    costoOptimo = costoTotal;
                    rutaOptima = current.ruta;
                    rutaOptima.push_back(colonias[nodoInicial - 1].nombre); // Regresar al nodo inicial
                }
            }
            continue;
        }

        // Expandir nodos hijos
        for (int i = 1; i <= n; i++) {
            if (!current.visitados[i] && matAdj[current.verticeActual][i] != INF) {
                tspNode newNode = current;
                newNode.niv++;
                newNode.verticeActual = i;
                newNode.costoAcum += matAdj[current.verticeActual][i];
                newNode.visitados[i] = true;
                newNode.ruta.push_back(colonias[i - 1].nombre);

                calculaCostoPosible(newNode, matAdj, n);

                pq.push(newNode);
            }
        }
    }

    // Retornar el costo óptimo y la ruta
    return {costoOptimo, rutaOptima};
}

// Complejidad O(n^3)
pair<vector<vector<int>>, vector<vector<int>>> floyd(int matAdj[MAX][MAX], int nodes) {
    vector<vector<int>> dist(nodes + 1, vector<int>(nodes + 1, INF)); // Copia matriz con valores originales

    vector<vector<int>> rutas(nodes + 1, vector<int>(nodes + 1, -1)); // Matriz de rutas

    // Inicializando matrices
    for (int i = 1; i <= nodes; i++) {
        for (int j = 1; j <= nodes; j++) {
            dist[i][j] = matAdj[i][j];
            if (matAdj[i][j] != INF && i != j) {
                rutas[i][j] = j; // Inicialmente, el siguiente nodo es el destino directo
            }
        }
    }

    // Algoritmo de Floyd-Warshall
    for (int k = 1; k <= nodes; k++) {
        for (int i = 1; i <= nodes; i++) {
            for (int j = 1; j <= nodes; j++) {
                if (dist[i][k] != INF && 
                    dist[k][j] != INF &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {

                    dist[i][j] = dist[i][k] + dist[k][j];
                    rutas[i][j] = rutas[i][k]; // Actualizar el siguiente nodo
                }
            }
        }
    }

    return {dist, rutas};
}

vector<int> reconstruirCamino(int start, int end, const vector<vector<int>>& rutas) {
    vector<int> path;

    if (rutas[start][end] == -1) {
        // No hay camino entre los nodos
        return path;
    }

    int current = start;
    path.push_back(current);

    while (current != end) {
        current = rutas[current][end];
        path.push_back(current);
    }

    return path;
}

string coloniaMasCercana(colonia nuevaColonia, const vector<colonia>& colonias) {
    if (colonias.size() == 0) {
        return "nuevaColonia";
    }

    pair<string, int> coloniaMasCercana = {"", INT_MAX};

    for (const auto& c : colonias) {
        if (coloniaMasCercana.second > getDistancia(nuevaColonia, c)) {
            coloniaMasCercana = {c.nombre, getDistancia(nuevaColonia, c)};
        }
    }

    return coloniaMasCercana.first;
}

int main() {
    int n, m, k, q;
    cin >> n >> m >> k >> q;
    vector<colonia> colonias(n);
    unordered_map<string, int> indiceColonias; //para encontrar rapido por nombre indice
    Graph g(n);
    
    // Leer colonias
    for (int i = 0; i < n; ++i) {
        string nombre;
        int x, y, central;
        cin >> nombre >> x >> y >> central;
        colonias[i] = colonia(nombre, x, y,central);
        indiceColonias[nombre] = i;
    }
    
    // conexiones existentes
    for (int i = 0; i < m; ++i) {
        string uName, vName;
        int cost;
        cin >> uName >> vName >> cost;
        g.addEdge(indiceColonias[uName], indiceColonias[vName], cost);
    }
    
    //  conexiones con nuevo cableado 
    string uName, vName, nombre;
    for (int i = 0; i < k; ++i) {
        cin >> uName >> vName;
        int costo = getDistancia(colonias[indiceColonias[uName]], colonias[indiceColonias[vName]]);

        // Buscar si ya existe la conexión
        bool found = false;
        for (auto &edge : g.edges) {
            // Si la conexión ya existe, actualizamos el `isNew` a true
            if ((edge.u == indiceColonias[uName] && edge.v == indiceColonias[vName]) ||
                (edge.u == indiceColonias[vName] && edge.v == indiceColonias[uName])) {
                edge.isNew = true;
                break;
            }
        }

        // // Si no la encontramos, la agregamos como nueva
        // if (!found) {
        //     g.addEdge(indiceColonias[uName], indiceColonias[vName], costo, true);
        // }
    }

    // Leer nuevas colonias
    vector<colonia> nuevasColonias(q);
    for (int i = 0; i < q; ++i) {
        int x, y;
        cin >> nombre >> x >> y;
        nuevasColonias[i] = colonia(nombre, x, y, false);
    }

    // Punto 1: Cableado óptimo
    ofstream out("output.txt");
    out <<"-------------------"<<endl;
    out << "1 - Cableado óptimo de nueva conexión.\n"<<endl; //doble espacio creo
    
    // MST
    vector<Edge> mstEdges = g.KruskalMST();
    
    // Mostrar las aristas del MST y calcular el costo total
    int costoTotal = 0;
    for (const auto &edge : mstEdges) {
        if (!edge.isNew) { // nocon  nuevo cableado
            costoTotal += edge.cost;
            out << colonias[edge.u].nombre << " - " << colonias[edge.v].nombre << " " << edge.cost <<endl;
        }
    }
    out << endl << "Costo Total: " << costoTotal<<endl;
    out<<endl<<"-------------------"<<endl;

    //pregunta2
    out << "2 - La ruta óptima." << endl << endl;

    pair<int, vector<string>> resultNoCentrales = tsp(g.matAdj, n, colonias);

    if (resultNoCentrales.first != INF) {
        for (int i = 0; i < resultNoCentrales.second.size(); i++) {
            out << resultNoCentrales.second[i];
            if (i != resultNoCentrales.second.size() - 1)
                out << " - ";
        }
        out << endl << endl;
        out << "La Ruta Óptima tiene un costo total de: " << resultNoCentrales.first << endl << endl;
    } else {
        out << "No se encontró una solución factible." << endl << endl;
    }

    out << "-------------------" << endl;


    // // Pregunta 3
    out << "3 - Caminos más cortos entre centrales." << endl << endl;

    pair<vector<vector<int>>, vector<vector<int>>> conexiones = floyd(g.matAdj, n);

    for (int i = 0; i <= n; i++) { // Ajustar índices: matriz inicia en 1
        for (int j = i; j <= n; j++) {
            if (i == j) continue;
            if (!colonias[i].central || !colonias[j].central) continue;
            if (conexiones.first[i + 1][j + 1] == INF) continue;

            vector<int> camino = reconstruirCamino(i + 1, j + 1, conexiones.second);
            string resultado;

            for (size_t k = 0; k < camino.size(); k++) {
                resultado += colonias[camino[k] - 1].nombre;
                if (k != camino.size() - 1) {
                    resultado += " - ";
                }
            }

            out << resultado << " (" << conexiones.first[i + 1][j + 1] << ")" << endl;
        }
    }

    out << endl << "-------------------" << endl;

    // Pregunta 4
    out << "4 - Conexión de nuevas colonias." << endl;

    for (int i = 0; i < nuevasColonias.size(); i++) {
        out << nuevasColonias[i].nombre << " debe conectarse con " << coloniaMasCercana(nuevasColonias[i], colonias) << endl << endl;
    }

    out << "-------------------" << endl;

    out.close();
    return 0;
}

/*

5 8 1 2
LindaVista 200 120 1
Purisima 150 75 0
Tecnologico -50 20 1
Roma -75 50 0
AltaVista -50 40 0
LindaVista Purisima 48
LindaVista Roma 17
Purisima Tecnologico 40
Purisima Roma 50
Purisima AltaVista 80
Tecnologico Roma 55
Tecnologico AltaVista 15
Roma AltaVista 18
Purisima Tecnologico
Independencia 180 -15
Roble 45 68

5 8 1 2
LindaVista 200 120 1
Purisima 150 75 0
Tecnologico -50 20 1
Roma -75 50 0
AltaVista -50 40 0
LindaVista Purisima 48
LindaVista Roma 17
Purisima Tecnologico 40
Purisima Roma 50
Purisima AltaVista 80
Tecnologico Roma 55
Tecnologico AltaVista 15
Roma AltaVista 18
Purisima Tecnologico
Independencia 180 -15
Roble 45 68

10 19 0 4
Condesa -193 -151 1
DelValle -142 -70 0
Polanco 144 -122 1
RomaNorte -91 -160 1
Pantitlán -108 42 0
Juárez -97 127 1
Tlatelolco -160 12 0
SantaFe 169 -91 1
Cosmopolita 160 -67 1
Merced 78 16 0
Condesa Polanco 27
Condesa Juárez 68
Condesa Cosmopolita 34
DelValle RomaNorte 80
DelValle Pantitlán 68
DelValle SantaFe 94
DelValle Cosmopolita 86
Polanco Juárez 92
Polanco Tlatelolco 58
Polanco Merced 54
RomaNorte Juárez 69
RomaNorte Cosmopolita 25
Pantitlán Juárez 4
Pantitlán SantaFe 67
Pantitlán Cosmopolita 25
Juárez SantaFe 78
Juárez Cosmopolita 29
Tlatelolco Cosmopolita 82
SantaFe Merced 66
FelipeAngeles -64 25
Tlazintla -85 -106
Penitenciaria -99 -83
Porvenir -95 -196


*/
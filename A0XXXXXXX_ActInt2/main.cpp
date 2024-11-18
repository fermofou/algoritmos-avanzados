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

    Graph(int V) : V(V) {}

    void addEdge(int u, int v, int cost, bool isNew = false) {
        edges.emplace_back(u, v, cost, isNew);
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
};

// distancia
int getDistancia(const colonia &a, const colonia &b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
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
    out << "1 – Cableado óptimo de nueva conexión.\n"<<endl; //doble espacio creo
    
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
    out << "Costo Total: " << costoTotal<<endl;
    out<<"-------------------"<<endl;
    //pregunta2
    
    out.close();
    return 0;
}

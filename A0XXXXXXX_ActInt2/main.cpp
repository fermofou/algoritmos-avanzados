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
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <stack>
using namespace std;


struct colonia{
    string nombre;
    int x;
    int y;
    bool central;

    colonia(){
        this->nombre = "";
        this->x = 0;
        this->y = 0;
    }

    colonia(string _nombre, int _x, int _y, bool _central) {
        this->nombre = _nombre;
        this->x = _x;
        this->y = _y;
        this->central = _central;
    }
};


//Distamcia entre 2 puntos
int distSq(colonia &p1, colonia &p2){
return ((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
}

vector<colonia>  leerDatos(int n){
    vector<colonia> colonias;
    string nombre;
    int x, y, central;
    for(int i=0;i<n;i++){
        cin>>nombre>>x>>y>>central;
        colonias.push_back(colonia(nombre,x,y,central));
    }
}

int main() {
    int n, m, k, q;
    cin >> n >> m >> k >> q;
    vector<colonia> colonias=leerDatos(n);
    //


    return 0;
}
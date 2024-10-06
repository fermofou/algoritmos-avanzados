#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

//Fernando Morán        A01284623
//Julen Hoppenstedt

//Complejidad Manachers: O(n)
vector<int> manachers(const string &s) {

    string t = "#";
    for (char c : s) {
        t += c;
        t += '#';
    }
    int n = t.size();
    vector<int> P(n, 0);  
    int center = 0, right = 0;  

    int max_len = 0;
    int max_center = 0;  

    for (int i = 0; i < n; i++) {
        int mirror = 2 * center - i;  
        if (i < right)
            P[i] = min(right - i, P[mirror]);

        while (i + P[i] + 1 < n && i - P[i] - 1 >= 0 && t[i + P[i] + 1] == t[i - P[i] - 1])
            P[i]++;

        if (i + P[i] > right) {
            center = i;
            right = i + P[i];
        }

        if (P[i] > max_len) {
            max_len = P[i];
            max_center = i;
        }
    }

    int start = (max_center - max_len) / 2;
    vector<int> ans = {start, max_len};
    return ans;
}



//Complejidad Longest Common Substring: O(n*m)
string longCommSub(const string& text1, const string& text2) {
    int n = text1.size();
    int m = text2.size();
    
    // Inicializa una matriz LCS de tamaño n x m
    vector<vector<int>> LCS(n, vector<int>(m, 0));
    
    int maxLength = 0;         
    int endIndexText1 = 0;     // index final de subst, inicio = endIndexText1 - maxLength

    // Para i desde 0 hasta n - 1: string1
    for (int i = 0; i < n; ++i) {
        if (text1[i] == text2[0]) {
            LCS[i][0] = 1;
            if (1 > maxLength) {
                maxLength = 1;
                endIndexText1 = i + 1;
            }
        } else {
            LCS[i][0] = 0;
        }
    }

    // Para j desde 0 hasta m - 1 (string2)
    for (int j = 0; j < m; ++j) {
        if (text1[0] == text2[j]) {
            LCS[0][j] = 1;
            if (1 > maxLength) {
                maxLength = 1;
                endIndexText1 = 1;
            }
        } else {
            LCS[0][j] = 0;
        }
    }

    // Para i desde 1 hasta n - 1 (longitud string1)
    for (int i = 1; i < n; ++i) {
        // Para j desde 1 hasta m - 1 (m es la longitud string2)
        for (int j = 1; j < m; ++j) {
            if (text1[i] == text2[j]) {
                LCS[i][j] = LCS[i - 1][j - 1] + 1;
                if (LCS[i][j] > maxLength) {
                    maxLength = LCS[i][j];
                    endIndexText1 = i + 1;
                }
            } else {
                LCS[i][j] = 0;
            }
        }
    }

    string lcsString = "";
    for (int i = endIndexText1 - maxLength; i < endIndexText1; ++i) {
        lcsString += text1[i];
    }

    return lcsString;
}


int main(){
    vector<string> texts={"transmission1.txt","transmission2.txt","transmission3.txt"};
    vector<string> textSaved; 
    for (string txt : texts){ //leo los textos y guardo en textSaved
     //creo que quitare despues el folder pq en instrucciones pide q esten en misma ruta
        ifstream file("texts\\"+txt);
        string line,t;
        while (getline(file, line)) {
            t+= line;
        } 
        textSaved.push_back(t);
        file.close();
    }
    
    ofstream outFile("output\\checking.txt" ); //Creo el archivo de salida
    //kmp
    string anskmp="1234";  //aqui pon lo tuyo
    outFile << "Código: "+anskmp<< endl;
    outFile<<"============== \nPalíndromo más grande:" <<endl;

    for(int i=0;i<textSaved.size();i++){
        vector<int> ans = manachers(textSaved[i]);
        outFile<<texts[i]<<"==> Posición: "<<ans[0]<<endl;
        outFile<<textSaved[i].substr(ans[0], ans[1])<<endl;
        outFile<<"----"<<endl;
        
    }
    outFile<<"============== \nLos Substring más largos son:" <<endl;
    
    outFile<<"T1-T2 ==> "<<longCommSub(textSaved[0],textSaved[1])<<endl;
    outFile<<"T1-T3 ==> "<<longCommSub(textSaved[0],textSaved[2])<<endl;
    outFile<<"T2-T3 ==> "<<longCommSub(textSaved[1],textSaved[2])<<endl;
    cout<<"Archivo de salida creado"<<endl;
   
    outFile.close();   
    
    return 0;
}
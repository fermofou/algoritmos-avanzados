#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>

using namespace std;

// Fernando Morán        A01284623
// Julen Hoppenstedt     A01174098

// Tiempo: O(n) n siendo la longitud de p
// Espacio: O(n)
vector<int> lps(string p) {
    int n = p.size();

    vector<int> lps(n, 0);

    int len = 0;
    int i = 1;
    while (i < n) {
        if (p[i] == p[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }

    return lps;
}

// Complejidad Manachers: O(n)
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

// Complejidad Longest Common Substring: O(n*m)
string longCommSub(const string& text1, const string& text2) {
    int n = text1.size();
    int m = text2.size();

    // Inicializa una matriz LCS de tamaño n x m
    vector<vector<int>> LCS(n, vector<int>(m, 0));

    int maxLength = 0;
    int endIndexText1 = 0;     // index final de substr, inicio = endIndexText1 - maxLength

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

// O(m + n) m siendo la longuitud de t y n siendo la longuitud de p
// Espacio(m + n) m siendo la longuitud de t y n siendo la longuitud de p
vector<int> kmp(const string &t, const string &p) {
    vector<int> lpsArray = lps(p);
    vector<int> positions;

    int n = t.size();
    int m = p.size();

    int i = 0;
    int j = 0;

    while (i < n) {
        if (t[i] == p[j]) {
            i++;
            j++;
        }

        if (j == m) {
            positions.push_back(i - j);
            j = lpsArray[j - 1];
        } else if (i < n && t[i] != p[j]) {
            if (j != 0) {
                j = lpsArray[j - 1];
            } else {
                i++;
            }
        }
    }

    return positions;
}

int main(){
    vector<string> texts = {"transmission1.txt", "transmission2.txt", "transmission3.txt"};
    vector<string> textSaved;
    for (string txt : texts){ // Read the transmission files
        ifstream file(txt);
        string line, t;
        while (getline(file, line)) {
            t += line;
        }
        textSaved.push_back(t);
        file.close();
    }

    // Read mcode.txt
    vector<string> mcodeList;
    ifstream mcodeFile("mcode.txt");
    string codeLine;
    while (getline(mcodeFile, codeLine)) {
        mcodeList.push_back(codeLine);
    }
    mcodeFile.close();

    ofstream outFile("checking.txt" ); // Create the output file

    // Checando cada uno de los codigos malicionsos
    for (string code : mcodeList) {
        outFile << "Código: " << code << endl;
        
        vector<int> codeCounts(3, 0);
        vector<vector<int>> codePositions(3);
        for (int idx = 0; idx < 3; idx++) {
            vector<int> positions = kmp(textSaved[idx], code);
            codeCounts[idx] = positions.size();
            codePositions[idx] = positions;
            outFile << texts[idx] << " ==> " << codeCounts[idx] << " veces" << endl;
            if (!positions.empty()) {
                for (size_t i = 0; i < positions.size(); ++i) {
                    outFile << positions[i];
                    if (i != positions.size() - 1) {
                        outFile << ", ";
                    }
                }
                outFile << endl;
            }
        }
        // Generando subsequencias con una letra menos
        vector<string> subsequences;
        for (int i = 0; i < code.size(); i++) {
            string subseq = code.substr(0, i) + code.substr(i + 1);
            subsequences.push_back(subseq);
        }
        // Checando cada una de las subsequencias y checando el maximo numero de apariciones por archivo
        int maxCount = 0;
        string mostFoundSubseq;
        string fileWithMostSubseq;
        for (string subseq : subsequences) {
            for (int idx = 0; idx < 3; idx++) {
                vector<int> positions = kmp(textSaved[idx], subseq);
                int count = positions.size();
                if (count > maxCount) {
                    maxCount = count;
                    mostFoundSubseq = subseq;
                    fileWithMostSubseq = texts[idx];
                }
            }
        }
        outFile << "La subsecuencia más encontrada es: " << mostFoundSubseq << " con " << maxCount << " veces en el archivo " << fileWithMostSubseq << endl;
        outFile << "--------------" << endl;
    }

    outFile << "==============" << endl;
    outFile << "Palíndromo más grande:" << endl;
    for(int i = 0; i < textSaved.size(); i++){
        vector<int> ans = manachers(textSaved[i]);
        outFile << texts[i] << " ==> Posición: " << ans[0] << endl;
        outFile << textSaved[i].substr(ans[0], ans[1]) << endl;
        outFile << "----" << endl;
    }

    outFile << "============== \nLos Substring más largos son:" << endl;
    outFile << "T1-T2 ==> " << longCommSub(textSaved[0], textSaved[1]) << endl;
    outFile << "T1-T3 ==> " << longCommSub(textSaved[0], textSaved[2]) << endl;
    outFile << "T2-T3 ==> " << longCommSub(textSaved[1], textSaved[2]) << endl;

    cout << "Archivo de salida creado" << endl;

    outFile.close();

    return 0;
}
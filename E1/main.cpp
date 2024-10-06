#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

//Fernando Morán        A01284623
//Julen Hoppenstedt

//Complejidad Manachers: O(n)
vector<int> longest_palindromic_substring(const string &s) {
    // Transform the input string to handle even-length palindromes
    string t = "#";
    for (char c : s) {
        t += c;
        t += '#';
    }
    int n = t.size();
    vector<int> P(n, 0);  // Array to store the radius of the palindrome at each center
    int center = 0, right = 0;  // Variables to track the current center and the right boundary

    int max_len = 0;  // To store the maximum length of palindrome found
    int max_center = 0;  // To store the center of the longest palindrome

    for (int i = 0; i < n; i++) {
        int mirror = 2 * center - i;  // Mirror position of `i` with respect to the current center

        // Use the previously computed palindrome information to initialize P[i]
        if (i < right)
            P[i] = min(right - i, P[mirror]);

        // Try to expand the palindrome centered at `i`
        while (i + P[i] + 1 < n && i - P[i] - 1 >= 0 && t[i + P[i] + 1] == t[i - P[i] - 1])
            P[i]++;

        // If the expanded palindrome is beyond the current right boundary, update center and right
        if (i + P[i] > right) {
            center = i;
            right = i + P[i];
        }

        // Update the maximum palindrome length and its center
        if (P[i] > max_len) {
            max_len = P[i];
            max_center = i;
        }
    }

    // Extract the longest palindromic substring from the original string
    int start = (max_center - max_len) / 2;  // Compute the starting index in the original string
    vector<int> ans = {start, max_len};
    return ans;
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
    //Manachers
    string ansMan="1234";  //aqui pon lo tuyo
    outFile << "Código: "+ansMan<< endl;
    outFile<<"============== \nPalíndromo más grande:" <<endl;

    for(int i=0;i<textSaved.size();i++){
        vector<int> ans = longest_palindromic_substring(textSaved[i]);
        outFile<<texts[i]<<"==> Posición: "<<ans[0]<<endl;
        outFile<<textSaved[i].substr(ans[0], ans[1])<<endl;
        outFile<<"----"<<endl;
        
    }
    outFile<<"============== \nLos Substring más largos son:" <<endl;

    // for (const string &text : textSaved) {
 
    //     outFile << text << endl;  // Write each text into the output file
    // }

    outFile.close();   
    
    return 0;
}
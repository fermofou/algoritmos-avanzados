#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

//Fernando Morán        A01284623
//Julen Hoppenstedt     

int main(){
    cout<<"Hello World"<<endl;
    vector<string> texts={"transmission1.txt","transmission2.txt","transmission3.txt"};
    vector<string> textSaved;
    for (string txt : texts){
        ifstream file("texts\\"+txt);
        string line,t;
        while (getline(file, line)) {
            t+= line;
        }
        textSaved.push_back(t);
        file.close();
    }

    ofstream outFile("output\\checking.txt" );
    string ansMan="1234";
    outFile << "Código: "+ansMan<< endl;
    outFile<<"============== \nPalíndromo más grande:" <<endl;
    for( string s: texts){
        outFile<<s+"==> Posición: "<<endl;
    }
    outFile<<"============== \nLos Substring más largos son:" <<endl;

    // for (const string &text : textSaved) {
 
    //     outFile << text << endl;  // Write each text into the output file
    // }

    outFile.close();   
    
    return 0;
}
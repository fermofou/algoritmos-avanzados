#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

int main(){
    cout<<"Hello World"<<endl;
    string op="texts\\";
    vector<string> texts={"transmission1.txt","transmission2.txt","transmission3.txt"};
    vector<string> textSaved;
    for (string txt : texts){
        ifstream file(op+txt);
        string line,t;
        while (getline(file, line)) {
            t+= line;
        }
        textSaved.push_back(t);
        file.close();
    }
    
    return 0;
}
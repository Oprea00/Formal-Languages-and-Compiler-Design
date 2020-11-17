#include <bits/stdc++.h>

using namespace std;

string alphabet;
vector <string> NonTerminals;
map <string , vector <string> > productions;
string startingNonTerminal;
int main()
{
    string line;
    ifstream fin("g1.in");
    getline(fin,line);

    while( line.find(" ") != -1 )
        {

            NonTerminals.push_back( line.substr(0,line.find(" ")));
            line.erase(0,line.find(" ")+1);
        }

    NonTerminals.push_back(line);

    fin >> startingNonTerminal;
    fin >> alphabet;

    cout << "NonTerminals are: ";
    for(int i = 0 ; i < NonTerminals.size() ; ++i)
        cout << NonTerminals[i] << " ";
    cout << "\n";

    cout << "Terminals are: " <<alphabet << "\n";
    getline(fin,line);

    cout << "All productions are:\n";
    while(getline(fin,line))
    {
        string NonTerminal = line.substr(0,line.find(" "));
        line.erase(0,line.find(" ")+1);
        productions [ NonTerminal ] .push_back( line );

        cout << NonTerminal << " -> " << line << "\n";
    }

    while(true)
    {
        string Symbol;
        cout << "Enter nonTerminal : ";
        cin >> Symbol;


        for( int i = 0 ; i < productions[Symbol].size();++i)
            cout << productions[Symbol][i] << " | ";
        cout << "\n";
    }
    fin.close();
    fin.open("g2.in");
    fin.close();


    return 0;
}

//#include <bits/stdc++.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <iomanip>


using namespace std;

string alphabet;
vector <string> NonTerminals;
map <string, vector <string> > productions;
string startingNonTerminal;
map <string, vector < pair<string, int >  > > productionNumbers;

map <string, vector <string> > Fi, Fj, Fz;

map < string, map <string, vector < pair< string, int > > > >parseTable;

vector <string> result;
// space is epsilon
// concatenation of length 1 function


string sequence, inputStack;
int nodeCounter = 0;

vector< pair < pair < int, string>, pair < int, int > > > tree;




void concat(string a, string b)
{
    result.clear();
    if (Fz[a].size() == 0 || Fz[b].size() == 0)
        return;

    result.clear();

    vector <string>::iterator it;
    for (int i = 0; i < Fz[a].size(); ++i)
        for (int j = 0; j < Fz[b].size(); ++j)
            if (Fz[a][i] != " ")
            {
                it = find(result.begin(), result.end(), Fz[a][i]);
                if (it == result.end())
                    result.push_back(Fz[a][i]);
            }
            else
            {
                it = find(result.begin(), result.end(), Fz[b][j]);
                if (it == result.end())
                    result.push_back(Fz[b][j]);
            }
}

// FIRST algorithm

void FIRST()
{
    for (int i = 0; i < alphabet.size(); ++i)
    {
        Fi[string(1, alphabet[i])].push_back(string(1, alphabet[i]));
        Fz[string(1, alphabet[i])].push_back(string(1, alphabet[i]));
    }
    vector <string>::iterator it;
    for (int i = 0; i < NonTerminals.size(); ++i)
        for (int j = 0; j < productions[NonTerminals[i]].size(); ++j)
        {
            int pos = alphabet.find(productions[NonTerminals[i]][j][0]);
            if (pos != -1)
                Fi[NonTerminals[i]].push_back(string(1, productions[NonTerminals[i]][j][0]));
        }


    while (true)
    {
        Fj.clear();
        Fz.clear();

        for (int i = 0; i < alphabet.size(); ++i)
        {
            Fj[string(1, alphabet[i])].push_back(string(1, alphabet[i]));
            Fz[string(1, alphabet[i])].push_back(string(1, alphabet[i]));
        }
        for (int i = 0; i < NonTerminals.size(); ++i)
            for (int j = 0; j < productions[NonTerminals[i]].size(); ++j)
            {
                Fz["last"].clear();
                for (int z = 0; z < Fi[string(1, productions[NonTerminals[i]][j][0])].size(); ++z)
                {
                    Fz["last"].push_back(Fi[string(1, productions[NonTerminals[i]][j][0])][z]);
                }

                for (int z = 1; z < productions[NonTerminals[i]][j].size(); ++z)
                {
                    Fz["current"].clear();

                    for (int x = 0; x < Fi[string(1, productions[NonTerminals[i]][j][z])].size(); ++x)
                        Fz["current"].push_back(Fi[string(1, productions[NonTerminals[i]][j][z])][x]);

                    concat("last", "current");
                    Fz["last"].clear();
                    Fz["last"] = result;
                }
                //scout << "\n";
                vector <string>::iterator it;

                for (int z = 0; z < Fi[NonTerminals[i]].size(); ++z)
                {
                    it = find(Fz["last"].begin(), Fz["last"].end(), Fi[NonTerminals[i]][z]);

                    if (it == Fz["last"].end())
                        Fz["last"].push_back(Fi[NonTerminals[i]][z]);

                }

                for (int z = 0; z < Fz["last"].size(); ++z)
                {
                    //cout << Fz["last"][z];
                    it = find(Fj[NonTerminals[i]].begin(), Fj[NonTerminals[i]].end(), Fz["last"][z]);

                    if (it == Fj[NonTerminals[i]].end())
                        Fj[NonTerminals[i]].push_back(Fz["last"][z]);

                }
                //cout << "\n";

            }

        int found = 1;

        for (int i = 0; i < NonTerminals.size(); ++i)
        {
            if (Fi[NonTerminals[i]].size() != Fj[NonTerminals[i]].size())
            {
                found = 0;
                break;
            }
            for (int j = 0; j < Fi[NonTerminals[i]].size(); ++j)
            {
                vector <string>::iterator it;
                it = find(Fj[NonTerminals[i]].begin(), Fj[NonTerminals[i]].end(), Fi[NonTerminals[i]][j]);
                if (it == Fj[NonTerminals[i]].end())
                {
                    found = 0;
                    break;
                }
            }
            if (found == 0)
                break;
        }
        if (found == 1)
            break;

        Fi.clear();
        Fi = Fj;


    }
}

map <string, vector<string> > followSet;

void FOLLOW() {
    //initialize follow
    for (int i = 0; i < NonTerminals.size(); ++i) {
        followSet.insert({ NonTerminals[i], {} });
    }
    followSet[startingNonTerminal].push_back(" ");


    vector<string> values;
    bool foundChange = false;
    int beforeStep = 0;
    int alreadyExists;
    //do until no change was made
    do {
        foundChange = false;
        for (auto nonTerminal : NonTerminals) {
            beforeStep = followSet[nonTerminal].size();
            for (auto production : productions) {
                string leftNonTerminal = production.first;
                //looking if the current nonterminal is in current production
                bool foundInProduction = false;
                for (int i = 0; i < production.second.size(); i++) {
                    for (int index = 0; index < production.second[i].size(); ++index) {
                        if (production.second[i][index] == nonTerminal[0])
                            foundInProduction = true;
                    }
                }
                if (foundInProduction) {
                    int position1 = 0;
                    int position2 = 0;
                    //search the position in production
                    for (int i = 0; i < production.second.size(); i++) {
                        for (int j = 0; j < production.second[i].size(); j++) {
                            if (production.second[i][j] == nonTerminal[0]) {
                                position1 = i;
                                position2 = j;
                            }
                        }
                    }
                    string epsilon = " ";
                    //if it's a production like :  S->BA , put follow of S in follow A
                    if (production.second[position1].size() == 2 && position2 == 1) {
                        values.clear();
                        for (int j = 0; j < followSet[leftNonTerminal].size(); j++) {
                            values.push_back(followSet[leftNonTerminal][j]);
                        }
                        for (auto value : values) {
                            alreadyExists = 0;
                            for (int i = 0; i < followSet[nonTerminal].size(); i++) {
                                if (followSet[nonTerminal][i] == value)
                                    alreadyExists = 1;
                            }
                            if (alreadyExists == 0)
                                followSet[nonTerminal].push_back(value);
                        }
                    }

                    if (production.second[position1][position2 + 1] != epsilon[0]) {
                        auto next = production.second[position1][position2 + 1];
                        string nextElement = string(1, next);
                        //in productions like abc, in follow(b) we put first(c)
                        for (auto a : Fi[nextElement]) {
                            //search for epsilon
                            for (int index = 0; index < Fi[a].size(); index++)
                                if (Fi[a][index] == " ") {
                                    values.clear();
                                    for (int j = 0; j < followSet[leftNonTerminal].size(); j++) {
                                        values.push_back(followSet[leftNonTerminal][j]);
                                    }
                                    for (auto value : values) {
                                        alreadyExists = 0;
                                        for (int i = 0; i < followSet[nonTerminal].size(); i++) {
                                            if (followSet[nonTerminal][i] == value)
                                                alreadyExists = 1;
                                        }
                                        if (alreadyExists == 0)
                                            followSet[nonTerminal].push_back(value);
                                    }
                                }
                                else {
                                    values.clear();
                                    int terminal = true;
                                    for (int index = 0; index < NonTerminals.size(); index++) {
                                        if (NonTerminals[index] == a)
                                            terminal = false;
                                    }
                                    if (terminal) {
                                        alreadyExists = 0;
                                        for (int i = 0; i < followSet[nonTerminal].size(); i++) {
                                            if (followSet[nonTerminal][i] == a)
                                                alreadyExists = 1;
                                        }
                                        if (alreadyExists == 0)
                                            followSet[nonTerminal].push_back(a);
                                    }
                                    else {
                                        for (int index = 0; index < Fi[a].size(); index++) {
                                            values.push_back(Fi[a][index]);
                                        }
                                        for (auto value : values) {
                                            alreadyExists = 0;
                                            for (int i = 0; i < followSet[nonTerminal].size(); i++) {
                                                if (followSet[nonTerminal][i] == a)
                                                    alreadyExists = 1;
                                            }
                                            if (alreadyExists == 0)
                                                followSet[nonTerminal].push_back(value);
                                        }
                                    }
                                }
                        }
                    }
                }
            }
            if (beforeStep != followSet[nonTerminal].size())
                foundChange = true;
        }

    } while (foundChange);

}


void createParseTable()
{



    for (auto nonTerminal : NonTerminals)
        for (int i = 0; i < alphabet.size(); ++i)
        {
            // initialising the parse table
            parseTable[nonTerminal][string(1, alphabet[i])];
        }

    for (auto nonTerminal : NonTerminals)
        for (auto prod : productionNumbers[nonTerminal])
        {
            auto production = prod.first;
            Fz.clear();
            Fz = Fi;

            Fz["rez"] = Fz[string(1, production[0])];
            // compute first of production
            for (int i = 1; i < production.size(); ++i)
            {
                concat("rez", string(1, production[i]));
                Fz["rez"].clear();
                Fz["rez"] = result;
            }
            // add to parse table
            for (auto symbol : Fz["rez"])
                if (symbol != " ")
                    parseTable[nonTerminal][symbol].push_back({ prod.first , prod.second });
                else {
                    for (auto e : followSet[nonTerminal])
                        parseTable[nonTerminal][e].push_back({ prod.first, prod.second });
                }
        }


}


bool checkSequence(string s)
{
    if (inputStack == "")
        inputStack = " ";

    if (true)
    {
        if (s == string(1, inputStack[0]))
        {
            inputStack.erase(0, 1);
            return true;
        }
        else if (parseTable[s][string(1, inputStack[0])].size() > 0)
        {
            bool res = true;
            // cout << s <<" ->" << " " << parseTable[ s ][ string(1,inputStack[0]) ][0].first << "\n";
            for (auto symbol : parseTable[s][string(1, inputStack[0])][0].first)
            {
                res &= checkSequence(string(1, symbol));
                if (res == false)
                {
                    //cout << inputStack<< "\n";
                    return res;
                }
            }

            return res;

        }
        else if (s == " ")
            return true;

    }
    //cout << s << "\n";
    return false;
}

void printTree(string s, int father, int sibling)
{
    nodeCounter++;

    int currentCount = nodeCounter;

    if (inputStack == "")
        inputStack = " ";

    if (true)
    {
        if (s == " ")
            cout << nodeCounter << " epsilon " << father << " " << sibling << "\n";
        else  cout << nodeCounter << " " << s << " " << father << " " << sibling << "\n";

        if (s == string(1, inputStack[0]))
        {
            inputStack.erase(0, 1);

        }
        else if (parseTable[s][string(1, inputStack[0])].size() > 0)
        {

            int currentSibling = -1, before;
            for (auto symbol : parseTable[s][string(1, inputStack[0])][0].first)
            {
                before = nodeCounter + 1;

                printTree(string(1, symbol), currentCount, currentSibling);

                currentSibling = before;
            }


        }
        else if (s == " ")
        {

        }

    }


}

int main()
{
    string line;
    ifstream fin("g1.in");
    getline(fin, line);

    while (line.find(" ") != -1) {
        NonTerminals.push_back(line.substr(0, line.find(" ")));
        line.erase(0, line.find(" ") + 1);
    }
    NonTerminals.push_back(line);

    fin >> startingNonTerminal;
    getline(fin, line);
    getline(fin, alphabet);

    cout << "NonTerminals are: ";
    for (int i = 0; i < NonTerminals.size(); ++i)
        cout << NonTerminals[i] << " ";
    cout << "\n";
    cout << "Terminals are: " << alphabet << "\n";
    cout << "Starting non terminal is: " << startingNonTerminal << "\n";
    cout << "All productions are:\n";
    int o = 1;
    while (getline(fin, line))
    {
        string NonTerminal = line.substr(0, line.find(" "));
        line.erase(0, line.find(" ") + 1);
        if (line != "epsilon")
        {
            productions[NonTerminal].push_back(line);   productionNumbers[NonTerminal].push_back({ line , o });
        }
        else {
            {productions[NonTerminal].push_back(" "); productionNumbers[NonTerminal].push_back({ " " , o });  }
        }
        cout << NonTerminal << " -> " << line << "\n";

        o++;
    }

    fin.close();
    fin.open("g2.in");
    getline(fin, sequence);
    inputStack = sequence;
    fin.close();

    FIRST();

    for (int i = 0; i < NonTerminals.size(); ++i)
    {
        cout << NonTerminals[i] << " : ";
        for (int j = 0; j < Fi[NonTerminals[i]].size(); ++j)
        {
            if (Fi[NonTerminals[i]][j] == " ")
                cout << "epsilon" << " ";
            else
                cout << Fi[NonTerminals[i]][j] << " ";
        }
        cout << "\n";
    }

    cout << "-------------------------\n";
    cout << "First\n";

    for (int i = 0; i < NonTerminals.size(); ++i)
    {
        cout << NonTerminals[i] << " : ";
        for (int j = 0; j < Fi[NonTerminals[i]].size(); ++j)
        {
            if (Fi[NonTerminals[i]][j] == " ")
                cout << "epsilon" << " ";
            else
                cout << Fi[NonTerminals[i]][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";


    FOLLOW();

    cout << "-------------------------\n";
    cout << "FOLLOW\n";
    for (int i = 0; i < NonTerminals.size(); ++i)
    {
        cout << NonTerminals[i] << " : ";
        for (int j = 0; j < followSet[NonTerminals[i]].size(); ++j)
        {
            if (followSet[NonTerminals[i]][j] == " ")
                cout << "epsilon" << " ";
            else
                cout << followSet[NonTerminals[i]][j] << " ";
        }
        cout << "\n";
    }


    createParseTable();
    bool isLL1 = true;
    cout << "\n-------------------------\n";
    cout << "PARSE TABLE\n";


    for (auto nonTerminal : NonTerminals)
    {
        cout << " ............\n";
        cout << nonTerminal << " :\n";

        for (int i = 0; i < alphabet[i]; ++i)
        {
            if (i > 0)
                cout << alphabet[i];
            else cout << "epsilon";

            cout << " : ";
            if (parseTable[nonTerminal][string(1, alphabet[i])].size() > 1)
            {
                isLL1 = false;   cout << "  too many ";
            }

            for (auto pairs : parseTable[nonTerminal][string(1, alphabet[i])])
                cout << "(" << pairs.first << "," << pairs.second << ")| ";
            cout << "\n";
        }
    }

    if (isLL1)
    {
        cout << " Grammar is LL1\n";

        bool  isGood = checkSequence(startingNonTerminal);
        cout << sequence << " is" << (isGood == true ? " accepted\n" : " not accepted\n");

        inputStack = sequence;
        cout << " -- Parse tree of sequence ( father/sibling table ) --\n";
        cout << " counter / node / father / sibling\n";
        /// father / sibling table
        printTree(startingNonTerminal, -1, -1);

    }
    else cout << " Grammar not LL1";




    return 0;
}

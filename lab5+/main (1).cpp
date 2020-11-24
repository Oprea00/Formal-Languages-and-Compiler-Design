#include <bits/stdc++.h>

using namespace std;

string alphabet;
vector <string> NonTerminals;
map <string , vector <string> > productions;
string startingNonTerminal;

map <string , vector <string> > Fi , Fj , Fz;

vector <string> result;
// space is epsilon
// concatenation of length 1 function
void concat(string a , string b)
{
result.clear();
    if( Fz[a].size() == 0 || Fz[b].size() == 0 )
        return;


    result.clear();

    vector <string>::iterator it;

   // for( int i = 0 ;i < Fz[a].size() ; ++i)
     //   cout << Fz[a][i] ;

   // cout << " , ";

     //for( int i = 0 ;i < Fz[b].size() ; ++i)
        //cout << Fz[b][i] ;


    for( int i = 0 ;i < Fz[a].size() ; ++i)
        for(int j = 0 ;j < Fz[b].size();++j)
                if( Fz[a][i] != " " )
                        {
                            it = find( result.begin() , result.end() , Fz[a][i]);
                            if(it == result.end())
                                result.push_back( Fz[a][i]);
                        }
                else
                {

                     it = find( result.begin() , result.end() , Fz[b][j]);
                            if(it == result.end())
                                result.push_back( Fz[b][j]);
                }


}

// FIRST algorithm

void FIRST()
{

    for( int i = 0 ; i < alphabet.size();++i)
        {Fi[ string(1,alphabet[i]) ].push_back( string(1,alphabet[i]) );

         Fz[  string(1,alphabet[i]) ].push_back( string(1,alphabet[i]) );
        }
     vector <string>::iterator it;
    for( int i = 0  ; i< NonTerminals.size(); ++i)
        for(int j = 0; j < productions[NonTerminals[i]].size() ;++j )
        {
            /*
            if( productions[NonTerminals[i]][j][0] == ' '  )
                cout << "epsilon\n";
            else cout << productions[NonTerminals[i]][j][0] << "\n";
            */
            int pos = alphabet.find( productions[NonTerminals[i]][j][0]  )  ;


            if( pos != -1 )
                Fi[ NonTerminals[i] ].push_back(  string( 1, productions[NonTerminals[i]][j][0] ) ) ;
        }
    //cout << "\n";
     for(int i = 0 ; i  < NonTerminals.size() ; ++i)
    {
        cout << NonTerminals[i] << " " << Fi[NonTerminals[i]].size() << " : ";
        for(int j = 0  ; j < Fi[NonTerminals[i]].size() ; ++j )
        {

            if(Fi[NonTerminals[i]][j] == " ")
                cout << "epsilon" << " ";
            else
            cout <<  Fi[NonTerminals[i]][j] << " ";
        }
        cout << "\n";
    }

    while ( true )
    {
        Fj.clear();
        Fz.clear();



        for( int i = 0 ; i < alphabet.size();++i)
            {Fj[ string(1,alphabet[i]) ].push_back( string(1,alphabet[i]) );
         Fz[  string(1,alphabet[i]) ].push_back( string(1,alphabet[i]) );

         //cout << string(1,alphabet[i]) <<" "<< Fi[ string(1,alphabet[i]) ][0] <<"\n";
            }
        for( int i = 0 ; i< NonTerminals.size() ; ++i)
            for( int j = 0 ; j < productions[  NonTerminals[i] ] .size() ; ++j )
        {

            //cout << productions[  NonTerminals[i] ][j][0]<< " ";
            Fz["last"].clear();
            for(int z = 0 ; z < Fi[  string (1 , productions[  NonTerminals[i] ][j][0] ) ].size() ;++z )
                   {
                    //cout <<    Fi[  string( 1 , productions[  NonTerminals[i] ][j][0] ) ][z] << " ";
                    Fz["last"].push_back( Fi[  string( 1 , productions[  NonTerminals[i] ][j][0] ) ][z]  ) ;
                   }

            for( int z= 1 ; z < productions[  NonTerminals[i] ][j].size() ; ++z  )
            {
                Fz["current"].clear();



                for(int  x = 0 ; x <   Fi[ string(1 ,productions[  NonTerminals[i] ][j][z]) ].size() ; ++x )
                    Fz[ "current" ].push_back(   Fi[ string(1 ,productions[  NonTerminals[i] ][j][z] ) ][ x]   );

                concat( "last",  "current"   );
               // for( int l = 0 ; l < result.size() ; ++l )
               //     cout << result[l] ;
              //  cout << " | ";
                Fz[ "last" ].clear();
                Fz["last"] = result;
            }
            //scout << "\n";
            vector <string>::iterator it;

            for(int z = 0 ; z < Fi[ NonTerminals[i] ] .size() ; ++z )
            {
               it = find( Fz["last"].begin() , Fz["last"].end() , Fi[ NonTerminals[i] ][z]  );

               if( it == Fz["last"].end())
                Fz["last"].push_back( Fi[ NonTerminals[i] ][z]  );

            }

            for(int z = 0 ; z < Fz[ "last" ] .size() ; ++z )
            {
                //cout << Fz["last"][z];
               it = find( Fj[  NonTerminals[i] ].begin() , Fj[NonTerminals[i]].end() , Fz["last"][z]  );

               if( it == Fj[NonTerminals[i] ].end())
                Fj[NonTerminals[i] ].push_back( Fz["last"][z]  );

            }
            //cout << "\n";

        }

        int found = 1;

        for( int i = 0 ; i< NonTerminals.size() ; ++i)
        {
            if( Fi[  NonTerminals[i] ] .size()  !=   Fj[  NonTerminals[i] ] .size())
            {
                found = 0 ;
                break;
            }

            for(int j  =0 ; j <  Fi[NonTerminals[i] ].size() ; ++j )
            {
                vector <string>::iterator it;


                it = find( Fj[ NonTerminals[i] ].begin() , Fj[ NonTerminals[i] ].end() , Fi[NonTerminals[i] ][j]  );

                if( it == Fj[ NonTerminals[i] ].end())
                {
                    found = 0 ;
                    break;
                }
            }

            if( found == 0)
                break;

        }

        if( found == 1)
            break;


        Fi.clear();
        Fi = Fj;

        for(int i = 0 ; i  < NonTerminals.size() ; ++i)
    {
        cout << NonTerminals[i] << " " << Fi[NonTerminals[i]].size() << " : ";
        for(int j = 0  ; j < Fi[NonTerminals[i]].size() ; ++j )
        {

            if(Fi[NonTerminals[i]][j] == " ")
                cout << "epsilon" << " ";
            else
            cout <<  Fi[NonTerminals[i]][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
    }

}


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
    getline(fin,line);
    getline(fin,alphabet);

    cout << "NonTerminals are: ";
    for(int i = 0 ; i < NonTerminals.size() ; ++i)
        cout << NonTerminals[i] << " ";
    cout << "\n";

    cout << "Terminals are: " <<alphabet << "\n";


    cout << "All productions are:\n";
    while(getline(fin,line))
    {
        string NonTerminal = line.substr(0,line.find(" "));
        line.erase(0,line.find(" ")+1);
        if( line != "epsilon")
            productions [ NonTerminal ] .push_back( line );
        else {
                productions [ NonTerminal ] .push_back( " " );

            }
        cout << NonTerminal << " -> " << line << "\n";
    }

    /*
    while(true)
    {
        string Symbol;
        cout << "Enter nonTerminal : ";
        cin >> Symbol;


        for( int i = 0 ; i < productions[Symbol].size();++i)
            cout << productions[Symbol][i] << " | ";
        cout << "\n";
    }
    */
    fin.close();
    fin.open("g2.in");
    fin.close();

    FIRST();

    for(int i = 0 ; i  < NonTerminals.size() ; ++i)
    {
        cout << NonTerminals[i] << " : ";
        for(int j = 0  ; j < Fi[NonTerminals[i]].size() ; ++j )
        {

            if(Fi[NonTerminals[i]][j] == " ")
                cout << "epsilon" << " ";
            else
            cout <<  Fi[NonTerminals[i]][j] << " ";
        }
        cout << "\n";
    }

    return 0;
}

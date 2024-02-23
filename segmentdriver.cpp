
//g++ -o driver segmentdriver.cpp STTGameTree.h STT3.h
#include<string>
#include<sstream>
#include<iostream>
#include<fstream>
#include"STTGameTree.h"

using namespace std;



//file format 
//12_X.pod where 1 is the first move and 2 is the 2nd move
//only allow starting with 1 or 2, and we ignore shifts on empty rows, so segment=2 makes 2*5=10 files
/*


00_X.pod
01_X.pod
02_X.pod
07_X.pod
08_X.pod
10_X.pod
11_X.pod
12_X.pod
17_X.pod
18_X.pod



using

segmentdriver XYZ A B

XYZ is a path in the tree of starting moves and using that as the root
0102 is 1st player doing 0, 2nd using 1, etc.

A is the level to load
7 will load P7.pod if LOADPRUNE is set to true
0 will not load anything but build from scratch

B is the level to build to and save


for a new tree
./segmentdriver 11 0 7

to then go to level 12
./segmentdriver 11 7 12



#0 - add col 0
#1 - add col 1
#2 - add col 2
#3 - shift row 0 left (top row)
#4 - shift row 0 right
#5 - shift row 1 left
#6 - shift row 1 right
#7 - shift row 2 left (bottom row)
#8 - shift row 2 right

*/


//*********************************************************
//PROGRAM SETTINGS
unsigned short SEGMENT_SIZE = 2;
string STARTINGPATH = "";

//only matters if LOCAL = false
unsigned int server_innum = 12;
unsigned int server_outnum = 13;
//load the p file (only matters if LOCAL = false)
bool LOADPRUNE = true;

//prompt for more levels
bool ADDLEVELS = true;

//interactive prompt or not
bool LOCAL = true;
//output unpruned tree
bool SAVE = false;

//run wld function and prune
bool WLD = true;
//output pruned tree (only matters if WLD = true)
bool SAVEPRUNE = true;


//string CONFIG = "config.txt";
//*********************************************************



//turn int to string
string intos(long int i)
{
    stringstream si;
    si << i;
    return si.str();
}


//does wld tree recursively choosing the best moves
void WLDTree(Node* node, char p)
{
    //if leaf
    if(node->win == '0' || node->win == '1' || node->win == '2' || node->win == '3' || node->win == '4')
        return;
    
    //opposite player
    char op = (p=='2') ? '1': '2';
    
    //set all children first
    for(short i=0; i<9; i++)
    {
        if(node->children[i] != NULL)
            WLDTree(node->children[i], op);
    }
    
    
    //find optimal move for a player
    int drawmove = -1;
    int badmove = -1;
    
    for(short i=0; i<9; i++)
    {
        if(node->children[i] != NULL)
        {
            if(p=='1' && (node->children[i]->win =='1' || node->children[i]->win =='F'))
            {
                node->win = 'F';
                //break;  //
                return;
            }
            else if(p=='2' && (node->children[i]->win =='2' || node->children[i]->win =='S'))
            {
                node->win = 'S';
                //break; //
                return;
            }
            else if(node->children[i]->win =='3' || node->children[i]->win =='4' || node->children[i]->win =='D' || node->children[i]->win =='L')
            {
                drawmove = i;
            }
            else if(p=='2' && (node->children[i]->win =='1' || node->children[i]->win =='F'))
            {
                badmove = i;
            }
            else if(p=='1' && (node->children[i]->win =='2' || node->children[i]->win =='S'))
            {
                badmove = i;
            }
        }
    }
    
    if(node->win != 'F' && node->win != 'S' && drawmove != -1 )
    {
        node->win = (node->children[drawmove]->win == 'D' || node->children[drawmove]->win == '3')? 'D' : 'L';
    }
    else if (node->win != p && badmove != -1)
    {
        node->win = (node->children[badmove]->win == 'F' || node->children[badmove]->win == '1')? 'F' : 'S';
    }
    
    
}


//prunes tree of moves that can be ignored b/c there is a winning move
//p is player's turn '1' or '2'
void WLDPrune(Node* node, char p)
{
    //if leaf
    if(node->win == '0' || node->win == '1' || node->win == '2' || node->win == '3' || node->win == '4')
        return;
    
    //opposite player
    char op = (p=='2') ? '1': '2';
    
    
    //Prune children first
    for(short i=0; i<9; i++)
    {
        if(node->children[i] != NULL)
            WLDPrune(node->children[i], op);
    }
    
    
    //find optimal move for a player
    int drawmove = -1;
    int badmove = -1;
    
    for(short i=0; i<9; i++)
    {
        if(node->children[i] != NULL)
        {
            if(p=='1' && node->children[i]->win =='1')
            {
                //delete all other children
                node->win = 'F';
                for(short j=0; j<9 ;j++)
                {
                    if (j != i && node->children[j] != NULL)
                    {
                        //don't have the DeleteTree function reference to do properly
                        delete node->children[j];
                        node->children[j] = NULL;
                    }
                }
                break;
            }
            else if(p=='2' && node->children[i]->win =='2')
            {
                //delete all other children
                node->win = 'S';
                // we need to keep so it can explore all 2nd player moves
                /*for(short j=0; j<9 ;j++)
                {
                    if (j != i && node->children[j] != NULL)
                    {
                        delete node->children[j];
                        node->children[j] = NULL;
                    }
                }*/
                break;
            }
        }
    }
    
    
}


int main(int argc, char* argv[])
{
    string instring = intos(server_innum);
    string outstring = intos(server_outnum);


    string full = "";
    string fn = "";
    STTGameTree boardtree;
    unsigned int turns;
    
    
    if(argc == 4)
    {
        // should be starting path
        STARTINGPATH = argv[1];
        if (STARTINGPATH == "-")
            STARTINGPATH = "";
        //level of tree to load (0 for new)
        instring = argv[2];
        server_innum = stoi(instring);
        //number of levels to build
        outstring = argv[3];
        server_outnum = stoi(outstring);
    }
    else
        return 0;
    
    
    if(!LOCAL)
        full = "/home/twylie/CPP/";
    
    if (instring != "0")
    {
        //pre order tree .POT
        if (LOADPRUNE)
            fn = full + "P" + STARTINGPATH + "_" + instring + ".pot";
        else
            fn = full + STARTINGPATH + "_" + instring + ".pot";
        
        
        cout<<"Welcome to the script"<<endl;
        
        boardtree.LoadTree(fn);
        printf("File %s loaded\n",fn.c_str());
    }

    
    printf("Number of Levels: %u\n", boardtree.GTlevels);
    //string path = boardtree.MaxPath(boardtree.GTroot);
    
    
    if(ADDLEVELS)
    {
        boardtree.BuildLevels(server_outnum, STARTINGPATH);
        
        cout<<"Tree built."<<endl;
    }

    
    if(SAVE)
    {
        fn = full + STARTINGPATH + "_" + outstring + ".pot";
        boardtree.SaveTree(fn);
        cout<<"Tree saved."<<endl;
    }
    
    //#now traverse tree, get board, check win 
    cout<<"Checking stats"<<endl;
    boardtree.CountGames();
    boardtree.PrintStats();
    
    if(WLD)
    {
        //now we prune
        cout<<endl<<"WLD Function"<<endl;
        WLDTree(boardtree.GTroot,'1');
        cout<<"ROOT: "<<boardtree.GTroot->win<<endl;
        
        Node* n = boardtree.GTroot;
        for(short i=0; i < STARTINGPATH.length(); i++)
        {
            short choice = (short)STARTINGPATH[i] + CHARTOINT;
            n = n->children[choice];
            cout<<"Level "<< i+1 <<": "<< n->win << " Position: "<< n->position << endl;
        }
        
        if(SAVEPRUNE)
        {
            cout<<"Prune Function"<<endl;
            WLDPrune(boardtree.GTroot,'1');
            fn = full + "P" + STARTINGPATH + "_"  + outstring + ".pot";
            boardtree.SaveTree(fn);
            cout<<"Prune Tree saved."<<endl;
            cout<<"NEW STATS"<<endl;
            boardtree.CountGames();
            boardtree.PrintStats();
        }
    }
    cout<<"The End"<<endl;
    
    return 0;
}

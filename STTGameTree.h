/*
* File: STTGameTree.h
* Author: Tim Wylie
* Date: 2024
* Description: Creates the Shift Tac Toe game tree object. See documentation for usage.
* Requirements: STT3.h
*/



/*
#also going to remove starting with shifts or in column 3

#win character
# X - not set
# 0 - draw 
# 1 - p1 wins leaf
# 2 - p2 wins leaf
# 3 - draw win leaf
# 4 - loop draw leaf
# F - first player win internal
# S - second player win internal
# D - draw internal (0 or 3)
# L - loop draw internal

#path nums
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

#include<cstddef>
#include<iostream>
#include<fstream>
#include<string>
#include"STT3.h"

using namespace std;
#define CHARTOINT -48
#define INTTOCHAR 48


class OUT
{
public:
  static std::ofstream *_rout;
  static std::ifstream *_rin;
};

std::ofstream* OUT::_rout = NULL;
std::ifstream* OUT::_rin = NULL;



struct Node
{
    unsigned long int position;
    //char R0, R1, R2, SH
    char win;
    Node* children[9];
};

class STTGameTree
{
public:
    //constructor
    STTGameTree();
    //destructor
    ~STTGameTree();
    //make sure the tree has x depth
    void BuildLevels(unsigned int level);
    //full function to add levels with starting path
    void BuildLevels(unsigned int level, string startingpath);
    //make sure the tree has x depth
    void addGameLevel(Node* node, string path, unsigned int level);
    //add game with this path to tree
    void addGame(string path);
    //simulates game
    STT3 GetBoard(string path);
    //return a pointer to a node
    Node* FindNodeByPath(string path);
    //return a pointer to a node
    Node* FindNodeByPosition(Node* node, unsigned long int pos);
    //get the number of levels in the tree
    unsigned int MaxLevel(Node *node);
    //get a longest path
    string MaxPath(Node *node);
    //count stats
    void CountGames();
    //traversal for counting
    void VisitAll(Node* node);
    //prints the stats
    void PrintStats();
    
    void SaveTree(string filename);
    void SaveNode(Node* node);
    Node* LoadNode();
    void LoadTree(string filename);
    //the root of the tree
    Node* GTroot = NULL;
    //This function walks through the tree and deletes each node
    //it should be a post-order traversal LRV
    void DeleteTree(Node* vertex);
    unsigned int GTfpw = 0;
    unsigned int GTspw = 0;
    unsigned int GTdraws = 0;
    unsigned int GTbwdraws = 0;
    unsigned int GTinvalids = 0;
    unsigned int GTloops = 0;
    unsigned int GTleaves = 0;
    unsigned int GTcount = 1;    
    unsigned int GTlevels = 0;
    string GTfileio_fn;
};



//constructor
STTGameTree::STTGameTree()
{
    //set root
    GTroot = new Node();
    GTroot->position = 5000000000111;
    GTroot->win = 'X';
    for(short i=0;i<9;i++)
        GTroot->children[i] = NULL;
    
    GTfpw = 0;
    GTspw = 0;
    GTdraws = 0;
    GTbwdraws = 0;
    GTinvalids = 0;
    GTloops = 0;
    GTleaves = 0;
    GTcount = 1; 
    GTlevels = 0;
}

//destructor
STTGameTree::~STTGameTree()
{
    DeleteTree(GTroot);
}
//make sure the tree has x depth
void STTGameTree::BuildLevels(unsigned int level)
{
    BuildLevels(level,"");
}
                 //make sure the tree has x depth
void STTGameTree::BuildLevels(unsigned int level, string startingpath)
{
    //cout<<"Build Levels"<<endl;
    if (startingpath.length() == 0)
    {
        //cout<<"Build from root"<<endl;
        //initialize tree with all opening moves
        if(GTlevels == 0 && startingpath.length() == 0)
        {
            //only allowing adding col 1 or 2 for first move
            for (short i=0; i<2; i++)
            {
                string s;
                s += (char)(i + INTTOCHAR);
                //cout<<"String "<<i<<": "<< s << endl;
                addGame(s);
            }
            GTlevels = 1;
            printf("Level %u built\n", GTlevels);
            printf("Tree Size: %u \n", GTcount);
        }
        //recursively add levels
        //int numruns = level - GTlevels;
        for(short i = GTlevels + 1; i <= level; i++)
        {
            //addGameLevel(GTroot, "", level);
            addGameLevel(GTroot->children[0], "0", i);
            addGameLevel(GTroot->children[1], "1", i);
            GTlevels++;
            printf("Level %u built \n", GTlevels);
            printf("Tree Size: %u \n", GTcount);
            //cout<<"CALCULATED LEVELS: "<<MaxLevel(GTroot)<<endl;
        }
    }
    else
    {
        //cout<<"Build from path"<<endl;
        //build nodes of starting path
        Node* temproot = FindNodeByPath(startingpath);
        
        if(temproot == NULL)
        {
            //add each node in the path
            for(short i = 0; i < startingpath.length(); i++)
            {
                //cout<<startingpath.substr(0,i+1)<<endl;
                //create node, set position data
                addGame(startingpath.substr(0,i+1));
            }
            GTlevels = startingpath.length();
            printf("Path %s built\n", startingpath.c_str());
            temproot = FindNodeByPath(startingpath);
        }
        //recursively add levels
        for(short i = GTlevels + 1; i <= level; i++)
        {
            addGameLevel(temproot, startingpath, i);
            GTlevels++;
            printf("Level %u built \n", GTlevels);
            printf("Tree Size: %u \n", GTcount);
            //cout<<"CALCULATED LEVELS: "<<MaxLevel(GTroot)<<endl;
        }
    }
}



//make sure the tree has x depth
void STTGameTree::addGameLevel(Node* node, string path, unsigned int level)
{
    if(node == NULL)
        return;
    
    //cout<<"LEV:"<<level<<" Path: "<<path<<endl;
    if(path.length() <= level - 1)
    { 
        for(short i=0; i<9; i++)
        {
            string newpath = path;
            newpath += (char)(i + INTTOCHAR);
            
            //cout << path << " XXX " << newpath << endl;
            if(node->children[i] != NULL)
            {
                //cout<<"NN:"<<newpath<<" "<<level<<endl;
                addGameLevel(node->children[i], newpath, level);
            }
            else
            {
                //#some filtering- no need to shift rows without things in them
                STT3 b;
                b = GetBoard(path);
                if (b.FLAG == 'V')
                {
                    if (!((b.BOARD.substr(0,3) =="000" && (i == 3 || i==4)) 
                        || (b.BOARD.substr(3,3) =="000" && (i == 5 || i==6)) 
                        || (b.BOARD.substr(6,3) =="000" && (i == 7 || i == 8))))
                    {
                        //so we don't add back in pruned games
                        if(newpath.length()==level)
                            addGame(newpath);
                        //else
                        //    cout<<"EX:"<<newpath<<" "<<level<<endl;
                    }
                }
            }
        }
    }
}



//add game with this path to tree
void STTGameTree::addGame(string path)
{
    //cout<<"Adding "<<path<<endl;
    //#only add if position doesn't exist elsewhere in the treeand it is valid
    STT3 b;
    b = GetBoard(path);
    if(b.FLAG != 'V')
        return;
    
    //#board as the int version
    string x = "5" + b.BOARD;
    unsigned long int intpos = atol(x.c_str());
    
    //create new node
    Node* newnode = new Node();
    newnode->position = intpos;
    newnode->win = 'X';
    for(short i=0;i<9;i++)
        newnode->children[i]=NULL;
    
    
    //#set leaf/win state
    char bwin = b.FindWin();
    if(bwin == '1' || bwin == '2' || bwin == '3')
    {
        newnode->win = bwin;
    }
    
    //#whose turn it is    
    char p = (path.length()%2 == 0) ? '2' : '1';
    //#other player
    char op = (p == '2') ? '1' : '2';
    
    //cout<<"ADD GAME: " << path << " for p"<<p<<" BOARD: "<< b.BOARD << endl;
    char cur = '0';
    short curint = cur + CHARTOINT; 
    string sval = path;
    Node* pt = GTroot;
    Node* ptp = NULL;
    
    while(pt != NULL && sval.length() >= 1)
    {
        cur = sval[0];
        curint = cur + CHARTOINT;
        
        //this path already has a win
        if(pt->win =='1' || pt->win == '2' || pt->win == '3' || pt->win == '4')
        {
            delete newnode;
            return;  
        }
        
        //#check if we have loop (and we add this as a leaf)
        if(pt->position == intpos)
            newnode->win = '4';
        ptp = pt;
        pt = pt->children[curint]; //#since numbers are 0-8
        if(pt != NULL)
            sval = sval.substr(1,sval.length()-1); //sval[1:]; //#remove left digit            
    }
    ptp->children[curint] = newnode;
    
    GTcount++;
    
}

//simulates game
STT3 STTGameTree::GetBoard(string path)
{
    //#moves column 0,1,2
    //#shift rows 0 1, 2 left and right
    int move = 0;
    STT3 b;
    char p = '1';
    int r, d;
    for(short move=0; move < path.length(); move++)
    {
        char curr = path[move];
        if (curr == '0' or curr == '1' or curr == '2')
        {
            if(b.Add(curr + CHARTOINT, p) == false)
                b.FLAG = 'A';
        }
        else
        {
            if (curr == '3'){
                r = 0; d = 1;
            }
            else if(curr == '4'){
                r = 0; d = -1;
            }
            else if(curr == '5'){
                r = 1; d = 1;
            }
            else if(curr == '6'){
                r = 1; d = -1;
            }
            else if(curr == '7'){
                r = 2; d = 1;
            }
            else if (curr == '8'){ 
                r = 2; d = -1;
            }
            
            if (b.Shift(r, d) == false)
                b.FLAG = 'S';
        }
        p = (p == '1') ? '2' : '1';
    }
    return b;
}

//return a pointer to a node
Node* STTGameTree::FindNodeByPath(string path)
{
    Node* pt = GTroot;
    Node* ptp = NULL;
    char cur;
    unsigned short curint;
    
    while(pt != NULL && path.length() >= 1)
    {
        cur = path[0];
        curint = cur + CHARTOINT;
        
        ptp = pt;
        pt = pt->children[curint]; //#since numbers are 0-8
        if(pt != NULL)
            path = path.substr(1,path.length()-1);
    }
    if(pt != NULL)
        return pt;
    else
        return NULL;
}

//return a pointer to a node
Node* STTGameTree::FindNodeByPosition(Node* node, unsigned long int pos)
{
    if (node == NULL)
        return NULL;
    
    if(node->position == pos)
        return node;
    
    //find any node matching position
    for(short i=0; i<9; i++) 
    {
        Node *t = FindNodeByPosition(node->children[i], pos);
        if(t != NULL)
            return t;
    }
    return NULL;
}


//get level - children count themselves
unsigned int STTGameTree::MaxLevel(Node *node)
{
    if(node == NULL)
        return 0;

    unsigned int max=0;
    for(short i=0; i<9; i++)
    {
        unsigned int x = 0;
        if(node->children[i] != NULL)
            x = MaxLevel(node->children[i]);
        
        if(x > max)
            max = x;
    }
    if(node == GTroot)
        return max;
    
    return max + 1;
}

//get a longest path - parents add
string STTGameTree::MaxPath(Node *node)
{
    if(node == NULL)
        return "";

    string max="";
    for(short i=0; i<9; i++)
    {
        string x="";
        if(node->children[i] != NULL)
        {
            x = (char)(i + INTTOCHAR) + MaxPath(node->children[i]);
        }
        if(x.length() > max.length())
        {
            max = x;
        }
    }
    
    return max;
}

//count stats
void STTGameTree::CountGames()
{
    GTfpw = 0;
    GTspw = 0;
    GTdraws = 0;
    GTbwdraws = 0;
    GTinvalids = 0;
    GTloops = 0;
    GTleaves = 0;
    GTcount = 0;
    VisitAll(GTroot);
}

//traversal for counting
void STTGameTree::VisitAll(Node* node)
{
    if (node == NULL)
        return;
    
    GTcount++;
    //unsigned int strpos =node->position - 5000000000000;
    //cout<<node->position<<endl;
    char w = node->win;
    
    if (w =='0'|| w=='1'|| w=='2'|| w=='3'|| w=='4')
        GTleaves++;
    
    if (w == '0')
        GTdraws++;
    else if( w == '1')
        GTfpw++;
    else if (w == '2')
        GTspw++;
    else if( w == '3')
        GTbwdraws++;
    else if (w == '4')
        GTloops++;
    else
        GTinvalids++;
    
    for(short i=0; i<9; i++) 
        VisitAll(node->children[i]);
}

//prints the stats
void STTGameTree::PrintStats()
{
    printf("Number of Levels: %u\n", GTlevels);
    printf("Number of Nodes: %u\n", GTcount);
    //printf("Number of Boards: %u" + str(len(self.BOARDS)));
    printf("Draws: %u\n", GTdraws);
    printf("BW Draws: %u\n", GTbwdraws);
    printf("P1 Wins: %u\n",GTfpw);
    printf("P2 Wins: %u\n",GTspw);
    printf("Loops: %u\n",GTloops);
    printf("Leaves: %u\n",GTleaves);
    printf("Invalids: %u\n", GTinvalids);
}

//This function walks through the tree and deletes each node
//it should be a post-order traversal LRV
void STTGameTree::DeleteTree(Node* vertex)
{
    if(vertex != NULL)
    {
        for(short i=0;i<9;i++)
            DeleteTree(vertex->children[i]);
        
        delete vertex;
    }
}

//serialized using preorder traversal
//https://www.geeksforgeeks.org/serialize-deserialize-binary-tree/
void STTGameTree::SaveTree(string filename)
{
    GTfileio_fn = filename;
    
    //ofstream GTfileio;
    std::ofstream GTfileio;
    
    GTfileio.open(GTfileio_fn, ios::out | ios::binary | ios::trunc);
    OUT::_rout = &GTfileio;
    if(OUT::_rout->is_open())
    //if(GTfileio.is_open())
    {
        //GTfileio.close();
        //cout<<filename<<" write"<<endl;
        SaveNode(GTroot);
        //cout<<"finished saving"<<endl;
        OUT::_rout->close();
    }
}

void STTGameTree::SaveNode(Node* node)
{
    //ofstream GTfileio;
    //GTfileio.open(GTfileio_fn, ios::out | ios::binary | ios::app);
    
    if(node == NULL)
    {
        //cout<<"NULL"<<endl;
        char write = '#';
        OUT::_rout->write((char*)&write,sizeof(char));
        //GTfileio.write((char*)&write,sizeof(char));
        return;
    }
    else
    {
        //write node win data
        char tempc = node->win;
        OUT::_rout->write((char*)&tempc,sizeof(char));
        //GTfileio.write((char*)&tempc,sizeof(char));
        //cout << node->position << endl;
        //write node position data
        unsigned long int tempp = node->position;
        OUT::_rout->write((char*)&tempp,sizeof(unsigned long));
        //GTfileio.write((char*)&tempp,sizeof(long));
        
    }
    //GTfileio.close();
    for(short i=0;i<9;i++)
        SaveNode(node->children[i]);
       
}



Node* STTGameTree::LoadNode()
{
    //fstream GTfileio;
    //GTfileio.open(GTfileio_fn, fstream::in | fstream::binary);
    unsigned long int input;
    char peek;
    //could peek, but just going to read as win or null
    peek = OUT::_rin->get();
    if(peek == '#')
    {
        //peek = OUT::_rin->get();
        //OUT::_rin->read((char*)&peek, sizeof(char));
        return NULL;
    }
    //create node, set position data
    Node* newnode;
    newnode = new Node();
    
    //set win data
    //char c;
    //c = OUT::_rin->get();
    //OUT::_rin->read(&c,sizeof(char));
    newnode->win = peek;
    
    //read position 
    OUT::_rin->read((char*)&input, sizeof(unsigned long));
    newnode->position = input;
    
    
    //set children
    for(short i=0; i<9; i++)
        newnode->children[i] = LoadNode();
    
    return newnode;
}


void STTGameTree::LoadTree(string filename)
{
    GTfileio_fn = filename;
    
    //ofstream GTfileio;
    std::ifstream GTfileio;
    
    GTfileio.open(GTfileio_fn, ios::in | ios::binary);
    OUT::_rin = &GTfileio;
    
    if(OUT::_rin->is_open())
    //if(GTfileio.is_open())
    {
        GTroot = LoadNode();
        GTfileio.close();
    }
    
    GTlevels = MaxLevel(GTroot);
    CountGames();    
}

/*
* File: AItest.cpp
* Author: Tim Wylie
* Date: 2024
* Description: Allows the user to play against the game tree for testing purposes. With the full tree, it's an optimal AI. See documentation for usage.
* Requirements: STTGameTree.h STT3.h
*/





#include<string>
#include<iostream>
#include<sstream>
//#include"STT3.h"
#include"STTGameTree.h"
using namespace std;


#define CHARTOINT -48

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

//turn int to string
string intos(long int i)
{
    stringstream si;
    si << i;
    return si.str();
}

int main(int argc, char* argv[])
{
    STTGameTree boardtree;
    
    cout<<"Welcome to Shift-Tac-Toe"<<endl;
    
    string filename;
    if (argc > 0)
    {
        filename = argv[1];
    }
    else
    {
        cout << "File (SM_D.pot):";
        cin >> filename;
    }
    
    boardtree.LoadTree(filename);
    printf("File %s loaded\n",filename.c_str());
    
    
    printf("Number of Levels: %u\n", boardtree.GTlevels);
    
    //01
    string input;
    if(argc > 1)
    {
        input = argv[2];
    }
    else
    {
        cout<<"Starting Moves:";
        cin >> input;
    }
    
    cout << "WLD Function" << endl;
    WLDTree(boardtree.GTroot,'1');
    
    
    short player = 1;
    Node* GamePtr = boardtree.GTroot;
    //apply game moves
    for( short i=0; i < input.length();i++)
    {
        short move = input[i] + CHARTOINT;
        GamePtr = GamePtr->children[move];
        player = (player == 1) ? 2 : 1;
    }
    
    STT3 b;
    bool run = true;
    while(run)
    {
        string strcon = intos(GamePtr->position);
        b.BOARD = strcon.substr(1,b.BOARD.length());
        
        char c = b.FindWin();
        if(c == '1' || c=='2' || c=='3')
        {
            cout<<"GAME OVER"<<endl;
            run = false;
            break;
        }
        
        
        int choice = -1;
        
        //user move
        if(player == 2)
        {
            b.ToString();
            cout<<"Commands: (A)dd, (S)hift, (E)xit"<<endl;
            cout << ">";
            char cmd;
            cin>>cmd;
            switch(cmd)
            {
            case 'A':
            case 'a':
            
                unsigned int c;
                cout<<"Column: ";
                cin>>c;
                choice = c;
                break;
                
            case 'S': 
            case 's':
            
                int r;
                char d;
                cout<<"Row: ";
                cin>>r;
                cout<<"Direction (l-left, r-right): ";
                cin>>d;
                
                if(r == 0 && d == 'l')
                    choice = 3;
                else if (r == 0 && d == 'r')
                    choice = 4;
                else if (r == 1 && d == 'l')
                    choice = 5;
                else if (r == 1 && d == 'r')
                    choice = 6;
                else if (r == 2 && d == 'l')
                    choice = 7;
                else if (r == 2 && d == 'r')
                    choice = 8;
                
                break;
            case 'E':
            case 'e':
            case 'q':
                run = false;
                break;
            //shortcuts
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':;
            case '5':;
            case '6':;
            case '7':;
            case '8':;
            case '9':
                choice = cmd + CHARTOINT;
                
                break;
            default:
                run = false;
                break;
            };
            
            
        }
        else
        {
            //ai move
            int drawmove = -1;
            int badmove = -1;
            for(short i=0 ; i < 9 ; i++)
            {
                if(GamePtr->children[i] != NULL)
                {
                    if(GamePtr->children[i]->win =='1' || GamePtr->children[i]->win =='F')
                    {
                        choice = i;
                        break;
                    }
                    else if (GamePtr->children[i]->win =='2' || GamePtr->children[i]->win =='S')
                    {
                        badmove = i;
                    }
                    else if (GamePtr->children[i]->win =='3' || GamePtr->children[i]->win =='4' || GamePtr->children[i]->win =='D' || GamePtr->children[i]->win =='L')
                    {
                        drawmove = i;
                    }
                }
            }
            
            if(choice == -1 && drawmove >= 0)
                choice = drawmove;
            else if (choice == -1 && badmove >= 0)
                choice = badmove;
            
            cout<<"AI Move: "<< choice << endl;
        }
        if(run)
        {
            GamePtr = GamePtr->children[choice];
            player = (player == 1) ? 2 : 1;
        }
      
    }
    b.ToString();
    
    return 0;
}

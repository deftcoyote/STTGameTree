#include<string>
#include<iostream>
#include<stdlib.h>
using namespace std;
#define CHARTOINT -48
#define INTTOCHAR 48

class STT3
{
public:
    string BOARD;
    char FLAG;
    unsigned int ROWS;
    unsigned int COLS;
    unsigned int SHIFT;
    unsigned int POS;
    char EMPTYSPOT;
    unsigned int R0;
    unsigned int R1;
    unsigned int R2;
    
    STT3()
    {
        BOARD = "000000000111";
        FLAG = 'V';
        ROWS = 3;
        COLS = 3;
        SHIFT = 2;
        POS = 9;
        EMPTYSPOT = '0';
        R0 = 0;
        R1 = 3;
        R2 = 6;
    }
    
    
    bool Add(unsigned int col, char piece)
    {
        if(col >= ROWS || col < 0)
        {
            FLAG = 'A';
            return false;
        }
        
        if(BOARD[col] != EMPTYSPOT)
        {
            FLAG = 'A';
            return false;
        }
        else
            BOARD[col + R0] = piece;
                 
        Compact();
        return true;
    }
    //121 turned to 020 with 2,-1
    //#moves a row left or right
    //#direc is either 1 or -1, 1 is left, -1 is right
    bool Shift(unsigned int row, int direc)
    {
        //#valid direction
        if(direc != 1 && direc != -1)
        {
            FLAG = 'S';
            return false;
        }
        //#valid row
        if(row < 0 || row >= ROWS)
        {
            FLAG = 'S';
            return false;
        }
        //#valid shift    
        int ps = (int)BOARD[POS + row] + CHARTOINT + direc;
        char charps = (char)(ps + INTTOCHAR);
        if(ps  < 0 || ps > SHIFT)
        {
            FLAG = 'S';
            return false;
        }
        //#apply
        BOARD[POS + row] = charps;
        
        
        //#move elements in row
        //colrange = range(self.COLS-1) if direc == 1 else range(self.COLS-1, -1, -1);
        if(direc == 1)
        {   
            for(int i=0; i < COLS-1; i++)
                BOARD[((row)*3 + i)] = BOARD[(row)*3 + i + direc];
        }
        else
        {
            for(int i=COLS-1; i > 0; i--)
                BOARD[((row)*3 + i)] = BOARD[(row)*3 + i + direc];
        }
        //#the last cell needs to be deleted (because the thing moved)
        unsigned int blankindex = (direc == -1) ? 0 : COLS-1;
        BOARD[(row)*3 + blankindex] = EMPTYSPOT;
        //self.BOARD = self.BOARD[:((row)*3 + blankindex)] +self.EMPTYSPOT+ self.BOARD[((row)*3 + blankindex+1):]

        Compact();
        return true;
    }
    void Compact()
    {
        //#should be rewritten to go by columns
        for(int i=0; i < ROWS;i++)//# 0, 1, 2
        {
            for(int c=0; c < COLS; c++)// # 0, 1, 2
            {
                for(int r=ROWS-1; r > 0; r--)// #2, 1
                {
                    if (BOARD[(r-1)*3 + c ] != EMPTYSPOT && BOARD[(r)*3 + c ] == EMPTYSPOT)
                    {
                            BOARD[(r)*3 + c] = BOARD[(r-1)*3+c];
                            BOARD[(r-1)*3+c] = EMPTYSPOT;
                    }
                }
            }
        }
    }
    char FindWin()
    {
        string winner = "0"; //#not equal to EMPTYSPOT 
        for(int r=0; r<3; r++)
        {
            //# checks all rows
            if(BOARD[(r)*3+0] == BOARD[(r)*3+1] && BOARD[(r)*3+0] == BOARD[(r)*3+2])
                winner.push_back(BOARD[(r)*3+0]);
            //#cols
            if(BOARD[(0)*3+r] == BOARD[(1)*3+r] && BOARD[(0)*3+r] == BOARD[(2)*3+r])
                winner.push_back(BOARD[(0)*3+r]);
        }
    
        //#diagonals
        if(BOARD[(0)*3 + 0] == BOARD[(1)*3 + 1] && BOARD[(0)*3 + 0] == BOARD[(2)*3 + 2])
            winner.push_back(BOARD[(0)*3 + 0]);
            
        if(BOARD[(2)*3+0] == BOARD[(1)*3+1] && BOARD[(2)*3+0] == BOARD[(0)*3+2])
            winner.push_back(BOARD[(2)*3+0]);
    
        if(winner.find('1') !=string::npos && winner.find('2') !=string::npos)
            return '3'; //#they both have a winning run
        else if(winner.find('1') !=string::npos)
            return '1';
        else if(winner.find('2') != string::npos)
            return '2';
        else
            return '0';
    }
    void ToString()
    {
        printf("Board: %s", BOARD.c_str());
        cout<<endl;
        
        //#assume we need shift area on both sides of board
        //#col labels
        //printf("%-3s"); //#for row label
        cout<<"       0  1  2";
        //#shift spaces
        //printf("%-3s", SHIFT);
        //#board bar |
        //printf(' ',end='') 
        //[print(fmtc(str(i)),end='') for i in range(COLS)]
        //#board bar
        //print(' ',end='') 
        //#[print(' ',end='')) for i in range(self.SHIFT)]
        cout<<endl;
        
        //#print(self.BOARD)
        for(int r=0;r<ROWS;r++)
        {
            cout<<" ";
            printf("%1i",r);
            //#make space if no shift
            for(int i=0;i < SHIFT;i++)
            {
                if ((int)BOARD[9+r] + CHARTOINT + i < SHIFT)
                    cout<<" ";
                else
                    cout<<"-";
            }
            cout<<"|";
            //#print main board area
            for (int c=0; c<COLS; c++)
            {
                char o = (BOARD[(r)*3 + c] == EMPTYSPOT) ? '-' :  BOARD[(r)*3 + c];
                cout<<" "<<o<<" ";
            }
            cout<<"|";
            //#print after board
            for (int i=0; i < SHIFT; i++)
            {
                if((int)BOARD[9+r]+CHARTOINT + i + 1 <= SHIFT)
                    cout<<"-";
                else
                    cout<<" ";
            }
            cout << endl; //#for newline
        }
    }
        
};

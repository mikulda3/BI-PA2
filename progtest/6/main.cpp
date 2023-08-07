#ifndef __PROGTEST__
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

// class representing cell in table
class CCell {
    public:
        // default destructor
        virtual ~CCell (void) = default;
        // some essential getters
        int GetLen(){ return len; };
        int GetLines(){ return lines; };
        int GetForm(){ return form; };
        int GetCount(){ return count; };
        void SetCount(int c){ count += c; };
        void ResetCount(){ count = 0; };
        //return string that is to be printed
        string PrintRow(const int i, int & type);
        // return pointer to current instance
        virtual CCell * Clone (void) const = 0;
    protected:
        // constructor
        CCell(string con, int f, int al){
            form = f;
            lines = 0;
            count = 0;
            align = al;
            if(form == 1){
                len = 0;
                unsigned tmp = 0;
                // get len of string till newline
                int check = 0;
                string line;
                for(unsigned i = 0; i < con.size(); i++){
                    line = line + con.at(i);
                    if(con.at(i) == '\n'){
                        lines++;
                        line.pop_back();
                        if(tmp > len)
                            len = tmp;
                        tmp = 0;
                        check++;
                        content.push_back(line);
                        line = "";
                        if(i+1 >= con.size()){
                            content.push_back("");
                            lines++;
                        }
                    }
                    tmp++;
                    if(check){
                        check = 0;
                        tmp--;
                    }

                }
                if(line != ""){
                    lines++;
                    content.push_back(line);
                }
                if(tmp > len)
                    len = tmp;
            } else {
                len = 0;
                if(form != 3)
                    content.push_back(con);
            }
        }
        int form;
        vector<string> content;
        unsigned len;
        unsigned lines;
        int align;
        int count;
};

string CCell::PrintRow(const int i, int & type){
    if((unsigned)i >= content.size())
        return "";
    type = align;
    return content.at(i);
}

// class representing text in cell, form = 1
class CText : public CCell {
    public:
        enum Align {ALIGN_LEFT, ALIGN_RIGHT};
        CText (string str, Align al) : CCell(str, 1, 0), type(al) {
            if(type == ALIGN_LEFT)
                align = 1;
        } 
        void SetText(string str);
        virtual CCell * Clone (void) const override {
            return new CText (*this);
        }
        double a, h, o, j;
    protected:
        Align type;
};

void CText::SetText(string str){
    content.clear();
    lines = 0;
    len = 0;
    unsigned tmp = 0;
    // get len of string till newline
    int check = 0;
    string line;
    for(unsigned i = 0; i < str.size(); i++){
        line = line + str.at(i);
        if(str.at(i) == '\n'){
            line.pop_back();
            lines++;
            if(tmp > len)
                len = tmp;
            tmp = 0;
            check++;
            content.push_back(line);
            line = "";
        }
        tmp++;
        if(check){
            check = 0;
            tmp--;
        }

    }
    if(line != ""){
        lines++;
        content.push_back(line);
    }
    if(tmp > len)
        len = tmp;
}

// class representing empty cell, form = 2
class CEmpty : public CCell {
    public:
        CEmpty() : CCell("", 2, 0) {}
        virtual CCell * Clone (void) const override {
            return new CEmpty (*this);
        }
};

// class representing image in cell, form = 3
class CImage : public CCell {
    public:
        CImage() : CCell("", 3, 2) {}
        CImage AddRow (string str){
            content.push_back(str);
            if(str.size() > len)
                len = str.size();
            lines++;
            return *this;
        }
        virtual CCell * Clone (void) const override {
            return new CImage (*this);
        }
        // this surely not best solution helped me in 
        // assert(sizeof(CEmpty) < sizeof(CImage));
        double as, dgh,ui,xcg,cg;
};

// class representing table of cells
class CTable {
    public:
        // constructor
        CTable(int rows, int cols) : row(rows), col(cols) {
            for(int i = 0; i < row; i++){
                vector<CCell*> line;
                for(int j = 0; j < col; j++){
                    CEmpty * empty = new CEmpty;
                    line.push_back(empty);
                }
                table.push_back(line);
            }
        }
        // destructor
        ~CTable(void){
            for(int i = 0; i < row; i++){
                for(int j = 0; j < col; j++){
                    delete table[i][j];
                }
            }
            table.clear();
        }
        // copying constructor
        CTable(const CTable & a);
        // assign operator
        CTable & operator = (const CTable & a);

        CCell & GetCell (const int & row, const int & col);
        void SetCell (const int & row, const int & col, const CCell & newContent);
        friend ostream & operator << (ostream & out, const CTable & a);
    private:
        int row, col;
        vector<vector<CCell*>> table;
};

CTable & CTable::operator = (const CTable & a){
    if(&a.table != &table){
        for(int i = 0; i < row; i++)
            for(int j = 0; j < col; j++)
                delete table[i][j];
        row = a.row;
        col = a.col;
        table = a.table;
        for(int i = 0; i < row; i++){
            for(int j = 0; j < col; j++){
                table[i][j] = a.table[i][j]->Clone();
            }
        }
    }
    return (*this);
}

CTable::CTable(const CTable & a){
    row = a.row;
    col = a.col;
    table = a.table;
    for(int i = 0; i < row; i++)
        for(int j = 0; j < col; j++)
            table[i][j] = a.table[i][j]->Clone();
}

CCell & CTable::GetCell(const int & rows, const int & cols){
    if(row < rows || col < cols)
        throw invalid_argument ("Cannot fit into table.");

    return *table[rows][cols];
}

void CTable::SetCell (const int & row, const int & col, const CCell & newContent){
    if(this->row < row || this->col < col)
        throw invalid_argument ("Cannot fit into table.");
    if(&newContent != table[row][col]){
        delete table[row][col];
        table[row][col] = newContent.Clone();
    }
}

ostream & operator << (ostream & out, const CTable & a){
    // initialize vectors for width and height of cells
    vector<int> width(a.col, 0);
    vector<int> height(a.row, 0);

    // get values to vectors
    for(int i = 0; i < a.row; i++){
        for(int j = 0; j < a.col; j++){
            CCell * cell = a.table[i][j];
            if(cell->GetLen() > width[j])
                width[j] = cell->GetLen();
            if(cell->GetLines() > height[i])
                height[i] = cell->GetLines();
        }
    }

    // seperation line 
    string separator = "";
    for(unsigned i = 0; i < width.size(); i++){
        separator += "+";
        for(int j = 0; j < width[i]; j++)   
            separator += "-";
    }
    separator += "+\n";
    out << separator;

    for(int i = 0; i < a.row; i++){
        for(int j = 0; j < height[i]; j++){
            out << "|";
            for(int k = 0; k < a.col; k++){
                CCell * cell = a.table[i][k];
                int check = 1;
                // only for images because they need to be centered
                if(cell->GetForm() == 3){
                    if( height[i] >= cell->GetLines() && cell->GetLines() != 0 ){
                        int h = height[i] - cell->GetLines();
                        if(h/2 > j){
                            out << setw(width[k]) << setfill(' ') << ' ';
                            check = 0;
                            cell->SetCount(1);
                        }
                    }
                }
                if(check){
                    int align = 0;
                    string tmp;
                    if(cell->GetForm() == 3)
                        tmp = cell->PrintRow(j-cell->GetCount(), align);
                    else 
                        tmp = cell->PrintRow(j, align);
                    int total_len = width[k];
                    if(align == 1) // align left
                        out << setw(total_len) << left << tmp;
                    else if(align == 0) // align right
                        out << setw(total_len) << right << tmp;
                    else if(align == 2){ // image - center
                        // align width
                        int len = cell->GetLen();
                        len = (total_len - len);
                        if(len%2 == 1){
                            len /= 2;
                            out << setw(len) << setfill(' ') << "" << tmp
                                << setw(len+1) << setfill(' ') << "";
                        } else {
                            len /= 2;
                            out << setw(len) << setfill(' ') << "" << tmp;
                            out << setw(len) << setfill(' ') << "";
                        }
                    }
                }
                out << "|";
            }
            out << "\n";
        }
        // write line after each row
        out << separator;
    }

    for(int i = 0; i < a.row; i++){
        for(int j = 0; j < a.col; j++){
            CCell * cell = a.table[i][j];
            cell->ResetCount();
        }
    }

    return out;
}

#ifndef __PROGTEST__
int main ( void ){  

CTable tt ( 5, 5 );    
tt.SetCell ( 0, 0, CText ( "Hello,\n\n\n", CText::ALIGN_LEFT ) );
tt . SetCell ( 1, 1, CImage ()
    . AddRow ( "1abc" )
    . AddRow ( "2abc" )
    . AddRow ( "3abc" )
    . AddRow ( "4abc" )
    . AddRow ( "5abc" ) );
tt . SetCell ( 1, 0, CImage ()
    . AddRow ( "111111" )
    . AddRow ( "222222" )
    . AddRow ( "333333" )
    . AddRow ( "444444" )
    . AddRow ( "555555" ) );
tt . SetCell ( 1, 3, CImage ()
    . AddRow ( "1" )
    . AddRow ( "2" )
    . AddRow ( "3" ) );
tt . SetCell ( 0, 3, CImage ()
    . AddRow ( "22222222222222" ) );
tt . SetCell ( 0, 1, CText ( "progtes         t\n", CText::ALIGN_LEFT ) );
tt . SetCell ( 1, 2, CText ( "Hello Kitty\nHello Kitty\nHello Kitty\nHello Kitty\nHello Kitty\nHello Kitty\nHello Kitty\nHello Kitty\nHello Kitty\nHello Kitty\nHello Kitty\n", CText::ALIGN_LEFT ) );
tt . SetCell ( 0, 0, CText ( "Hello Kitty\nHello Kitty\nHello Kitty\nHello Kitty\nHello Kitty\nHello Kitty\nHello Kitty\nHello Kitty\nHello Kitty\nHello Kitty\nHello Kitty\n", CText::ALIGN_LEFT ) );
cout << tt;
    ostringstream oss;
    CTable t0 ( 3, 2 );    
    t0 . SetCell ( 0, 0, CText ( "Hello,\n"
            "Hello Kitty", CText::ALIGN_LEFT ) );
    t0 . SetCell ( 1, 0, CText ( "Lorem ipsum dolor sit amet", CText::ALIGN_LEFT ) );
    t0 . SetCell ( 2, 0, CText ( "Bye,\n"
            "Hello Kitty", CText::ALIGN_RIGHT ) );
    t0 . SetCell ( 1, 1, CImage ()
            . AddRow ( "###                   " )
            . AddRow ( "#  #                  " )
            . AddRow ( "#  # # ##   ###    ###" )
            . AddRow ( "###  ##    #   #  #  #" )
            . AddRow ( "#    #     #   #  #  #" )
            . AddRow ( "#    #     #   #  #  #" )
            . AddRow ( "#    #      ###    ###" )
            . AddRow ( "                     #" )
            . AddRow ( "                   ## " )
            . AddRow ( "                      " )
            . AddRow ( " #    ###   ###   #   " )
            . AddRow ( "###  #   # #     ###  " )
            . AddRow ( " #   #####  ###   #   " )
            . AddRow ( " #   #         #  #   " )
            . AddRow ( "  ##  ###   ###    ## " ) );
    t0 . SetCell ( 2, 1, CEmpty () );
    oss . str ("");
    oss . clear ();
    t0 = t0;
    oss << t0;
    assert ( oss . str () ==
            "+--------------------------+----------------------+\n"
            "|Hello,                    |                      |\n"
            "|Hello Kitty               |                      |\n"
            "+--------------------------+----------------------+\n"
            "|Lorem ipsum dolor sit amet|###                   |\n"
            "|                          |#  #                  |\n"
            "|                          |#  # # ##   ###    ###|\n"
            "|                          |###  ##    #   #  #  #|\n"
            "|                          |#    #     #   #  #  #|\n"
            "|                          |#    #     #   #  #  #|\n"
            "|                          |#    #      ###    ###|\n"
            "|                          |                     #|\n"
            "|                          |                   ## |\n"
            "|                          |                      |\n"
            "|                          | #    ###   ###   #   |\n"
            "|                          |###  #   # #     ###  |\n"
            "|                          | #   #####  ###   #   |\n"
            "|                          | #   #         #  #   |\n"
            "|                          |  ##  ###   ###    ## |\n"
            "+--------------------------+----------------------+\n"
            "|                      Bye,|                      |\n"
            "|               Hello Kitty|                      |\n"
            "+--------------------------+----------------------+\n" );

    t0 . SetCell ( 0, 1, t0 . GetCell ( 1, 1 ) );
    t0 . SetCell ( 2, 1, CImage ()
            . AddRow ( "*****   *      *  *      ******* ******  *" )
            . AddRow ( "*    *  *      *  *      *            *  *" )
            . AddRow ( "*    *  *      *  *      *           *   *" )
            . AddRow ( "*    *  *      *  *      *****      *    *" )
            . AddRow ( "****    *      *  *      *         *     *" )
            . AddRow ( "*  *    *      *  *      *        *       " )
            . AddRow ( "*   *   *      *  *      *       *       *" )
            . AddRow ( "*    *    *****   ****** ******* ******  *" ) );

    dynamic_cast<CText &> ( t0 . GetCell ( 1, 0 ) ) . SetText ( "Lorem ipsum dolor sit amet,\n"
            "consectetur adipiscing\n"
            "elit. Curabitur scelerisque\n"
            "lorem vitae lectus cursus,\n"
            "vitae porta ante placerat. Class aptent taciti\n"
            "sociosqu ad litora\n"
            "torquent per\n"
            "conubia nostra,\n"
            "per inceptos himenaeos.\n"
            "\n"
            "Donec tincidunt augue\n"
            "sit amet metus\n"
            "pretium volutpat.\n"
            "Donec faucibus,\n"
            "ante sit amet\n"
            "luctus posuere,\n"
            "mauris tellus" );

    oss . str ("");
    oss . clear ();
    oss << t0;
    assert ( oss . str () ==
            "+----------------------------------------------+------------------------------------------+\n"
            "|Hello,                                        |          ###                             |\n"
            "|Hello Kitty                                   |          #  #                            |\n"
            "|                                              |          #  # # ##   ###    ###          |\n"
            "|                                              |          ###  ##    #   #  #  #          |\n"
            "|                                              |          #    #     #   #  #  #          |\n"
            "|                                              |          #    #     #   #  #  #          |\n"
            "|                                              |          #    #      ###    ###          |\n"
            "|                                              |                               #          |\n"
            "|                                              |                             ##           |\n"
            "|                                              |                                          |\n"
            "|                                              |           #    ###   ###   #             |\n"
            "|                                              |          ###  #   # #     ###            |\n"
            "|                                              |           #   #####  ###   #             |\n"
            "|                                              |           #   #         #  #             |\n"
            "|                                              |            ##  ###   ###    ##           |\n"
            "+----------------------------------------------+------------------------------------------+\n"
            "|Lorem ipsum dolor sit amet,                   |                                          |\n"
            "|consectetur adipiscing                        |          ###                             |\n"
            "|elit. Curabitur scelerisque                   |          #  #                            |\n"
            "|lorem vitae lectus cursus,                    |          #  # # ##   ###    ###          |\n"
            "|vitae porta ante placerat. Class aptent taciti|          ###  ##    #   #  #  #          |\n"
            "|sociosqu ad litora                            |          #    #     #   #  #  #          |\n"
            "|torquent per                                  |          #    #     #   #  #  #          |\n"
            "|conubia nostra,                               |          #    #      ###    ###          |\n"
            "|per inceptos himenaeos.                       |                               #          |\n"
            "|                                              |                             ##           |\n"
            "|Donec tincidunt augue                         |                                          |\n"
            "|sit amet metus                                |           #    ###   ###   #             |\n"
            "|pretium volutpat.                             |          ###  #   # #     ###            |\n"
            "|Donec faucibus,                               |           #   #####  ###   #             |\n"
            "|ante sit amet                                 |           #   #         #  #             |\n"
            "|luctus posuere,                               |            ##  ###   ###    ##           |\n"
            "|mauris tellus                                 |                                          |\n"
            "+----------------------------------------------+------------------------------------------+\n"
            "|                                          Bye,|*****   *      *  *      ******* ******  *|\n"
            "|                                   Hello Kitty|*    *  *      *  *      *            *  *|\n"
            "|                                              |*    *  *      *  *      *           *   *|\n"
            "|                                              |*    *  *      *  *      *****      *    *|\n"
            "|                                              |****    *      *  *      *         *     *|\n"
            "|                                              |*  *    *      *  *      *        *       |\n"
            "|                                              |*   *   *      *  *      *       *       *|\n"
            "|                                              |*    *    *****   ****** ******* ******  *|\n"
            "+----------------------------------------------+------------------------------------------+\n" );

    CTable t1 ( t0 );
    t1 . SetCell ( 1, 0, CEmpty () );
    t1 . SetCell ( 1, 1, CEmpty () );
    oss . str ("");
    oss . clear ();
    oss << t0;
    assert ( oss . str () ==
            "+----------------------------------------------+------------------------------------------+\n"
            "|Hello,                                        |          ###                             |\n"
            "|Hello Kitty                                   |          #  #                            |\n"
            "|                                              |          #  # # ##   ###    ###          |\n"
            "|                                              |          ###  ##    #   #  #  #          |\n"
            "|                                              |          #    #     #   #  #  #          |\n"
            "|                                              |          #    #     #   #  #  #          |\n"
            "|                                              |          #    #      ###    ###          |\n"
            "|                                              |                               #          |\n"
            "|                                              |                             ##           |\n"
            "|                                              |                                          |\n"
            "|                                              |           #    ###   ###   #             |\n"
            "|                                              |          ###  #   # #     ###            |\n"
            "|                                              |           #   #####  ###   #             |\n"
            "|                                              |           #   #         #  #             |\n"
            "|                                              |            ##  ###   ###    ##           |\n"
            "+----------------------------------------------+------------------------------------------+\n"
            "|Lorem ipsum dolor sit amet,                   |                                          |\n"
            "|consectetur adipiscing                        |          ###                             |\n"
            "|elit. Curabitur scelerisque                   |          #  #                            |\n"
            "|lorem vitae lectus cursus,                    |          #  # # ##   ###    ###          |\n"
            "|vitae porta ante placerat. Class aptent taciti|          ###  ##    #   #  #  #          |\n"
            "|sociosqu ad litora                            |          #    #     #   #  #  #          |\n"
            "|torquent per                                  |          #    #     #   #  #  #          |\n"
            "|conubia nostra,                               |          #    #      ###    ###          |\n"
            "|per inceptos himenaeos.                       |                               #          |\n"
            "|                                              |                             ##           |\n"
            "|Donec tincidunt augue                         |                                          |\n"
            "|sit amet metus                                |           #    ###   ###   #             |\n"
            "|pretium volutpat.                             |          ###  #   # #     ###            |\n"
            "|Donec faucibus,                               |           #   #####  ###   #             |\n"
            "|ante sit amet                                 |           #   #         #  #             |\n"
            "|luctus posuere,                               |            ##  ###   ###    ##           |\n"
            "|mauris tellus                                 |                                          |\n"
            "+----------------------------------------------+------------------------------------------+\n"
            "|                                          Bye,|*****   *      *  *      ******* ******  *|\n"
            "|                                   Hello Kitty|*    *  *      *  *      *            *  *|\n"
            "|                                              |*    *  *      *  *      *           *   *|\n"
            "|                                              |*    *  *      *  *      *****      *    *|\n"
            "|                                              |****    *      *  *      *         *     *|\n"
            "|                                              |*  *    *      *  *      *        *       |\n"
            "|                                              |*   *   *      *  *      *       *       *|\n"
            "|                                              |*    *    *****   ****** ******* ******  *|\n"
            "+----------------------------------------------+------------------------------------------+\n" );
    oss . str ("");
    oss . clear ();
    oss << t1;
    assert ( oss . str () ==
            "+-----------+------------------------------------------+\n"
            "|Hello,     |          ###                             |\n"
            "|Hello Kitty|          #  #                            |\n"
            "|           |          #  # # ##   ###    ###          |\n"
            "|           |          ###  ##    #   #  #  #          |\n"
            "|           |          #    #     #   #  #  #          |\n"
            "|           |          #    #     #   #  #  #          |\n"
            "|           |          #    #      ###    ###          |\n"
            "|           |                               #          |\n"
            "|           |                             ##           |\n"
            "|           |                                          |\n"
            "|           |           #    ###   ###   #             |\n"
            "|           |          ###  #   # #     ###            |\n"
            "|           |           #   #####  ###   #             |\n"
            "|           |           #   #         #  #             |\n"
            "|           |            ##  ###   ###    ##           |\n"
            "+-----------+------------------------------------------+\n"
            "+-----------+------------------------------------------+\n"
            "|       Bye,|*****   *      *  *      ******* ******  *|\n"
            "|Hello Kitty|*    *  *      *  *      *            *  *|\n"
            "|           |*    *  *      *  *      *           *   *|\n"
            "|           |*    *  *      *  *      *****      *    *|\n"
            "|           |****    *      *  *      *         *     *|\n"
            "|           |*  *    *      *  *      *        *       |\n"
            "|           |*   *   *      *  *      *       *       *|\n"
            "|           |*    *    *****   ****** ******* ******  *|\n"
            "+-----------+------------------------------------------+\n" );
    t1 = t0;
    t1 . SetCell ( 0, 0, CEmpty () );
    t1 . SetCell ( 1, 1, CImage ()
            . AddRow ( "  ********                    " )
            . AddRow ( " **********                   " )
            . AddRow ( "**        **                  " )
            . AddRow ( "**             **        **   " )
            . AddRow ( "**             **        **   " )
            . AddRow ( "***         ********  ********" )
            . AddRow ( "****        ********  ********" )
            . AddRow ( "****           **        **   " )
            . AddRow ( "****           **        **   " )
            . AddRow ( "****      **                  " )
            . AddRow ( " **********                   " )
            . AddRow ( "  ********                    " ) );
    oss . str ("");
    oss . clear ();
    oss << t0;
    assert ( oss . str () ==
            "+----------------------------------------------+------------------------------------------+\n"
            "|Hello,                                        |          ###                             |\n"
            "|Hello Kitty                                   |          #  #                            |\n"
            "|                                              |          #  # # ##   ###    ###          |\n"
            "|                                              |          ###  ##    #   #  #  #          |\n"
            "|                                              |          #    #     #   #  #  #          |\n"
            "|                                              |          #    #     #   #  #  #          |\n"
            "|                                              |          #    #      ###    ###          |\n"
            "|                                              |                               #          |\n"
            "|                                              |                             ##           |\n"
            "|                                              |                                          |\n"
            "|                                              |           #    ###   ###   #             |\n"
            "|                                              |          ###  #   # #     ###            |\n"
            "|                                              |           #   #####  ###   #             |\n"
            "|                                              |           #   #         #  #             |\n"
            "|                                              |            ##  ###   ###    ##           |\n"
            "+----------------------------------------------+------------------------------------------+\n"
            "|Lorem ipsum dolor sit amet,                   |                                          |\n"
            "|consectetur adipiscing                        |          ###                             |\n"
            "|elit. Curabitur scelerisque                   |          #  #                            |\n"
            "|lorem vitae lectus cursus,                    |          #  # # ##   ###    ###          |\n"
            "|vitae porta ante placerat. Class aptent taciti|          ###  ##    #   #  #  #          |\n"
            "|sociosqu ad litora                            |          #    #     #   #  #  #          |\n"
            "|torquent per                                  |          #    #     #   #  #  #          |\n"
            "|conubia nostra,                               |          #    #      ###    ###          |\n"
            "|per inceptos himenaeos.                       |                               #          |\n"
            "|                                              |                             ##           |\n"
            "|Donec tincidunt augue                         |                                          |\n"
            "|sit amet metus                                |           #    ###   ###   #             |\n"
            "|pretium volutpat.                             |          ###  #   # #     ###            |\n"
            "|Donec faucibus,                               |           #   #####  ###   #             |\n"
            "|ante sit amet                                 |           #   #         #  #             |\n"
            "|luctus posuere,                               |            ##  ###   ###    ##           |\n"
            "|mauris tellus                                 |                                          |\n"
            "+----------------------------------------------+------------------------------------------+\n"
            "|                                          Bye,|*****   *      *  *      ******* ******  *|\n"
            "|                                   Hello Kitty|*    *  *      *  *      *            *  *|\n"
            "|                                              |*    *  *      *  *      *           *   *|\n"
            "|                                              |*    *  *      *  *      *****      *    *|\n"
            "|                                              |****    *      *  *      *         *     *|\n"
            "|                                              |*  *    *      *  *      *        *       |\n"
            "|                                              |*   *   *      *  *      *       *       *|\n"
            "|                                              |*    *    *****   ****** ******* ******  *|\n"
            "+----------------------------------------------+------------------------------------------+\n" );
    oss . str ("");
    oss . clear ();
    oss << t1;
    assert ( oss . str () ==
            "+----------------------------------------------+------------------------------------------+\n"
            "|                                              |          ###                             |\n"
            "|                                              |          #  #                            |\n"
            "|                                              |          #  # # ##   ###    ###          |\n"
            "|                                              |          ###  ##    #   #  #  #          |\n"
            "|                                              |          #    #     #   #  #  #          |\n"
            "|                                              |          #    #     #   #  #  #          |\n"
            "|                                              |          #    #      ###    ###          |\n"
            "|                                              |                               #          |\n"
            "|                                              |                             ##           |\n"
            "|                                              |                                          |\n"
            "|                                              |           #    ###   ###   #             |\n"
            "|                                              |          ###  #   # #     ###            |\n"
            "|                                              |           #   #####  ###   #             |\n"
            "|                                              |           #   #         #  #             |\n"
            "|                                              |            ##  ###   ###    ##           |\n"
            "+----------------------------------------------+------------------------------------------+\n"
            "|Lorem ipsum dolor sit amet,                   |                                          |\n"
            "|consectetur adipiscing                        |                                          |\n"
            "|elit. Curabitur scelerisque                   |        ********                          |\n"
            "|lorem vitae lectus cursus,                    |       **********                         |\n"
            "|vitae porta ante placerat. Class aptent taciti|      **        **                        |\n"
            "|sociosqu ad litora                            |      **             **        **         |\n"
            "|torquent per                                  |      **             **        **         |\n"
            "|conubia nostra,                               |      ***         ********  ********      |\n"
            "|per inceptos himenaeos.                       |      ****        ********  ********      |\n"
            "|                                              |      ****           **        **         |\n"
            "|Donec tincidunt augue                         |      ****           **        **         |\n"
            "|sit amet metus                                |      ****      **                        |\n"
            "|pretium volutpat.                             |       **********                         |\n"
            "|Donec faucibus,                               |        ********                          |\n"
            "|ante sit amet                                 |                                          |\n"
            "|luctus posuere,                               |                                          |\n"
            "|mauris tellus                                 |                                          |\n"
            "+----------------------------------------------+------------------------------------------+\n"
            "|                                          Bye,|*****   *      *  *      ******* ******  *|\n"
            "|                                   Hello Kitty|*    *  *      *  *      *            *  *|\n"
            "|                                              |*    *  *      *  *      *           *   *|\n"
            "|                                              |*    *  *      *  *      *****      *    *|\n"
            "|                                              |****    *      *  *      *         *     *|\n"
            "|                                              |*  *    *      *  *      *        *       |\n"
            "|                                              |*   *   *      *  *      *       *       *|\n"
            "|                                              |*    *    *****   ****** ******* ******  *|\n"
            "+----------------------------------------------+------------------------------------------+\n" );
    return 0;
}
#endif /* __PROGTEST__ */

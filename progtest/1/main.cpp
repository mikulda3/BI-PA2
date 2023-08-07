#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>

using namespace std;

#endif

// tree struct =============================================================
struct Node {
    int state;
    unsigned char symbol;
    unsigned char symbol_2;
    unsigned char symbol_3;
    unsigned char symbol_4;
    struct Node * left;
    struct Node * right;    
};

// load data from input file ==============================================
bool load_data(int & size, ifstream & i_file, vector<int> & v){
    char byte;
    int check = 1;
    while(i_file.get(byte)){
        if(i_file.fail())
            return false;
        for(int i = 7; i >= 0; i--){
            v.push_back((byte >> i) & 1);
            if(v.at(size))
                check = 0;
            size++;
        }
    }
    if(!i_file.eof() || check)
        return false;
    v.push_back('\n'); // rusi test 7 ?
    return true;
}

// create new node ========================================================
Node * new_node(char symbol){ 
    Node* node = new Node;
    node->symbol = symbol; 
    node->state = 0;
    node->left = node->right = NULL; 
    return node; 
} 

//print tree =============================================================
void print_tree(Node* root) { 
    if (root != NULL) { 
        print_tree(root->left);  
        print_tree(root->right);
        if(root->symbol != 0){
            if(root->state == 1)
                cout << (int)root->symbol << " "; 
            else if(root->state == 2){
                cout << (int)root->symbol;
                cout << " ";
                cout << (int)root->symbol_2;
                cout << " ";
            } else if(root->state == 3){
                cout << (int)root->symbol;
                cout << " ";
                cout << (int)root->symbol_2;
                cout << " ";
                cout << (int)root->symbol_3;
                cout << " ";
            } else if(root->state == 4){
                cout << (int)root->symbol;
                cout << " ";
                cout << (int)root->symbol_2;
                cout << " ";
                cout << (int)root->symbol_3;
                cout << " ";
                cout << (int)root->symbol_4;
                cout << " ";
            }
        }
    } 
} 

// build tree =============================================================
bool build_tree(vector<int> text, int & i, int size, Node * & root, 
                int & zero, int & one){
    if(i < size){
        int bit = text.at(i);
        Node * temp = new_node(bit);
        root = temp;
        if(bit){
            if(i+8 >= size)
                return false;
            if(text.at(i+1) == 0){ // 1 byte - ASCII
               // cout << "0\n";
                if(i+8 >= size)
                    return false;
                int pow = 128, ascii = 0;
                for(int j = 0; j < 8; j++){
                    ascii += pow * text.at(++i);
                    pow /= 2;
                }
                root->symbol = ascii;
                root->state = 1;
                one++;
                return true;
        //110 =====================================================
            } else if(text.at(i+1) && text.at(i+2) && !text.at(i+3)){ 
               // cout << "110\n";
                if(i+16 >= size)
                    return false;
                // 8 bitu
                int pow = 128, ascii = 0;
                for(int j = 0; j < 8; j++){
                    ascii += pow * text.at(++i);
                    pow /= 2;
                }
                root->symbol = ascii;
                // 10 check 
                if(text.at(i+1) != 1 || text.at(i+2) != 0)
                    return false;
                // 8 bitu
                pow = 128, ascii = 0;
                for(int j = 0; j < 8; j++){
                    ascii += pow * text.at(++i);
                    pow /= 2;
                }
                root->symbol_2 = ascii;
                root->state = 2;
                one++;
                return true;
        //1110 ====================================================
            } else if(text.at(i+1) && text.at(i+2) && text.at(i+3)
                      && !text.at(i+4)){ 
                //cout << "1110\n";
                if(i+24 >= size)
                    return false;
                // 4 bity
                int pow = 128, ascii = 0;
                for(int j = 0; j < 8; j++){
                    ascii += pow * text.at(++i);
                    pow /= 2;
                }
                root->symbol = ascii;
                // 10 check 
                if(text.at(i+1) != 1 || text.at(i+2) != 0)
                    return false;
                // 6 bitu
                pow = 128;
                int ascii2 = 0;
                for(int j = 0; j < 8; j++){
                    ascii2 += pow * text.at(++i);
                    pow /= 2;
                }
                root->symbol_2 = ascii2;
                if( ( ascii == 224 && (ascii2 < 160 || ascii2 > 191)) ||
                    ( ascii ==  237 && (ascii2 < 128 || ascii2 > 159)) )
                    return false;
                // check 10
                if(text.at(i+1) != 1 || text.at(i+2) != 0)
                    return false;
                // 6 bitu
                pow = 128, ascii = 0;
                for(int j = 0; j < 8; j++){
                    ascii += pow * text.at(++i);
                    pow /= 2;
                }
                root->symbol_3 = ascii;
                root->state = 3;
                one++;
                return true;
        // 11110 ===================================================
            } else if(text.at(i+1) && text.at(i+2) && text.at(i+3)
                      && text.at(i+4) && !text.at(i+5)){ 
                //cout << "11110\n";
                if(i+32 >= size)
                    return false;
                // 3 bity
                long long int check_pow = 2147483648;
                long long int check = 0;
                int pow = 128, ascii = 0;
                for(int j = 0; j < 8; j++){
                    ascii += pow * text.at(++i);
                    pow /= 2;
                    check += check_pow * text.at(i);
                    check_pow /= 2;
                }
                root->symbol = ascii;
                // 10 check 
                if(text.at(i+1) != 1 || text.at(i+2) != 0)
                    return false;
                // 6 bitu
                pow = 128;
                int ascii2 = 0;
                for(int j = 0; j < 8; j++){
                    ascii2 += pow * text.at(++i);
                    pow /= 2;
                    check += check_pow * text.at(i);
                    check_pow /= 2;
                }
                root->symbol_2 = ascii2;
                if( ( ascii == 240 && (ascii2 < 144 || ascii2 > 191)) ||
                    ( ascii ==  244 && (ascii2 < 128 || ascii2 > 143)) )
                    return false;
                // check 10
                if(text.at(i+1) != 1 || text.at(i+2) != 0)
                    return false;
                // 8 bitu
                pow = 128, ascii = 0;
                for(int j = 0; j < 8; j++){
                    ascii += pow * text.at(++i);
                    pow /= 2;
                    check += check_pow * text.at(i);
                    check_pow /= 2;
                }
                root->symbol_3 = ascii;
                // check 10
                if(text.at(i+1) != 1 || text.at(i+2) != 0)
                    return false;
                // 6 bitu
                pow = 128, ascii = 0;
                for(int j = 0; j < 8; j++){
                    ascii += pow * text.at(++i);
                    pow /= 2;
                    check += check_pow * text.at(i);
                    check_pow /= 2;
                }
                root->symbol_4 = ascii;
                //cout << check << " == check\n";
                if(check > 4103061439)
                    return false;
                root->state = 4;
                one++;
                return true;
            } else 
                return false;
        } else {
            i++;
            if(!build_tree(text, i, size, root->left, zero, one))
                return false;
        }
        temp = new_node(bit);
        root->right = temp;
        i++;
        zero++;
        if(!build_tree(text, i, size, root->right, zero, one))
            return false;
    }
    return true;
}

// get size of the chunk ==================================================
bool get_chunk(vector<int> text, int & i, int size, int & chunk, int & check){
    chunk = 0;
    if(text.at(++i)){
        chunk = 4096;
    } else {
        if(i+12 > size)
           return false;
        check = 1;
        int pow = 2048;
        for(int j = 0; j < 12; j++){
            chunk += text.at(++i) * pow;
            pow /= 2;
        }
    }
    if(i+chunk >= size)
        return false;
    return true;
}

// print chunk ============================================================
bool print_chunk(vector<int> text, int & i, int size, int chunk, Node * root,
                 ofstream & o_file){
    for(int j = 0; j < chunk; j++){
        Node * temp = root;
        while(root){
            if(i >= size || !o_file.good() || o_file.fail())
                return false;
            if(root->state == 1){
                o_file << root->symbol;
                break;
            } else if(root->state == 2){
                o_file << root->symbol << root->symbol_2;                
                break;
            } else if(root->state == 3){
                o_file << root->symbol << root->symbol_2 << root->symbol_3;
                break;
            } else if(root->state == 4){
                o_file << root->symbol << root->symbol_2 << root->symbol_3 << root->symbol_4;
                break;
            }
            if(text.at(++i))
                root = root->right;
            else
                root = root->left;
        }
        root = temp;
    }
    return true;
}

// delete binary tree =====================================================
void delete_children(Node* root){  
   if(root == NULL)
        return;  
    delete_children(root->left);  
    delete_children(root->right);    
}  

void delete_tree(Node ** root){  
    delete_children(*root);  
    *root = NULL;  
}  

// main function to decompress file =======================================
bool decompressFile( const char * inFileName, const char * outFileName){
    vector<int> text;
    int size = 0;
    ifstream i_file(inFileName, ios::binary | ios::in);
    
    // cant open file or empty file
    if(!i_file.is_open() || i_file.peek() == ifstream::traits_type::eof())
        return false;
    
    // load data from input file
    if(!load_data(size, i_file, text))
        return false;

    // int count = 0;
    // for(int n : text){
    //     cout << n;
    //     count++;
    //     if(count%8 == 0)
    //         cout << " ";
    //     // if(count%80 == 0)
    //     //     break;
    // }
    // cout <<endl;
    // build tree from bits
    int i = 0;
    Node * root = NULL; 
    int zero = 0, one = 0;
    if(!build_tree(text, i, size, root, zero, one)){
        delete_tree(&root);
        return false;
    }
    if(zero >= one)
        return false;
    
    // cout << endl;
    // print_tree(root);
    // cout << endl;
    //cout << "===================\n";
    // write to new file 
    ofstream o_file(outFileName, ios::binary | ios::out);
    if(!o_file.is_open() || o_file.fail()){
        delete_tree(&root);
        return false;
    }

    int check = 0, chunk = 1;
    while(chunk){
        if(!get_chunk(text, i, size, chunk, check)){
            delete_tree(&root);
            return false;
        }
        if(!chunk){
            delete_tree(&root);
            return true;
        } else if(check && chunk){
            if(!print_chunk(text, i, size, chunk, root, o_file)){
                delete_tree(&root);
                return false;
            }
            delete_tree(&root);
            return true;
        } else if(chunk){
            if(!print_chunk(text, i, size, chunk, root, o_file)){
                delete_tree(&root);
                return false;
            }
        }
    }
    delete_tree(&root);
    return true;
}

bool compressFile ( const char * inFileName, const char * outFileName ){
    return false;
}


#ifndef __PROGTEST__
bool identicalFiles( const char * fileName1, const char * fileName2){
    ifstream file1(fileName1, ios::in | ios::binary), 
             file2(fileName2, ios::in | ios::binary);

    if(!file1.is_open() || !file2.is_open())
        return false;

    char * buffer_1 = new char[1](),
         * buffer_2 = new char[1]();

    while(file1.good() || file2.good()){
        file1.read(buffer_1, 1);
        file2.read(buffer_2, 1);
        if (memcmp(buffer_1, buffer_2, 1) != 0){
            delete[] buffer_1;
            delete[] buffer_2;
            return false;
        }
    } 
    delete[] buffer_1;
    delete[] buffer_2;
    return true;
}

int main(void){
    // assert ( decompressFile ( "wrongutf.bin", "tempfile" ) );
    // assert ( decompressFile ( "wrong.bin", "tempfile" ) );
    // assert ( !decompressFile ( "wrongwiki.bin", "tempfile" ) );
    // assert ( !decompressFile ( "progtester5.huf", "tempfile" ) );
    // assert(!decompressFile("progtester0.huf", "tempfile"));
    //====================================================

    // assert ( !decompressFile ( "wr.bin", "tempfile" ) );

    assert ( decompressFile ( "tests/test0.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/test0.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/test1.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/test1.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/test2.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/test2.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/test3.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/test3.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/test4.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/test4.orig", "tempfile" ) );

    assert ( ! decompressFile ( "tests/test5.huf", "tempfile" ) );
// // ===================================================
//     assert( !decompressFile ( "tests/empty.huf", "tempfile" ));
//     assert( !decompressFile ( "tests/test7.huf", "tempfile" ));
//     assert ( decompressFile ("wrong.bin", "tempfile"));
//     assert ( decompressFile ( "progtester4.huf", "tempfile" ) );
//     assert ( identicalFiles ( "progtester4.orig", "tempfile" ) );
//     assert ( ! decompressFile ( "progtester2.huf", "tempfile" ) );

// ===================================================

    assert ( decompressFile ( "tests/extra0.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra0.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/extra1.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra1.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/extra2.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra2.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/extra3.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra3.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/extra4.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra4.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/extra5.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra5.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/extra6.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra6.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/extra7.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra7.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/extra8.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra8.orig", "tempfile" ) );

    assert ( decompressFile ( "tests/extra9.huf", "tempfile" ) );
    assert ( identicalFiles ( "tests/extra9.orig", "tempfile" ) );

    return 0;
}
#endif 

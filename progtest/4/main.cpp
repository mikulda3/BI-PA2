#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
#endif /* __PROGTEST__ */

class CTransaction {
    public:
        char sign;
        int amount;
        char * who;
        char * signature;
};

class CAccount {
    public:
        char * accID;
        int initBalance;
        int currentBalance;

        CTransaction * transaction;
        int capacity;
        int current;
        int Balance();
};

int CAccount::Balance(){
    return currentBalance;
}

class CBank {
    public:
        // default constructor
        CBank() { this->data = new CAccount[1]; this->capacity = 1;
                  this->current = 0; } 
        // copying constructor
        CBank(const CBank & a);
        // destructor
        ~CBank();
        // operator =
        CBank & operator = (const CBank & a);


        bool NewAccount( const char * accID, int initialBalance );
        bool Transaction( const char * debAccID, const char * credAccID,
                        unsigned int amount, const char * signature );
        bool TrimAccount( const char * accID );
        CAccount Account(const char * accID);
        friend ostream & operator << (ostream & out, const CBank & account);
        void Print() const;
    private:
        CAccount * data; 
        int capacity; 
        int current; 
};

CBank::~CBank(){                        
    for(int i = 0; i < current; i++){
        for(int j = 0; j < data[i].current; j++){
            delete[] data[i].transaction[j].signature;
            delete[] data[i].transaction[j].who;
        }
        delete[] data[i].transaction;
        delete[] data[i].accID;
    }
    delete[] data;
}

CBank & CBank::operator = (const CBank & a){
    for(int i = 0; i < current; i++){
        for(int j = 0; j < data[i].current; j++){
            delete[] data[i].transaction[j].signature;
            delete[] data[i].transaction[j].who;
        }
        delete[] data[i].transaction;
        delete[] data[i].accID;
    }
    delete[] data;
    data = new CAccount[a.capacity];
    for(int i = 0; i < a.current; i++){
        int len = strlen(a.data[i].accID);
        data[i].accID = new char[len+1];
        strcpy(data[i].accID, a.data[i].accID);
        data[i].currentBalance = a.data[i].currentBalance;
        data[i].initBalance = a.data[i].initBalance;
        data[i].capacity = a.data[i].capacity;
        data[i].current = a.data[i].current;
        len = a.data[i].capacity;
        data[i].transaction = new CTransaction [len];
        for(int j = 0; j < a.data[i].current; j++){
            data[i].transaction[j].amount = a.data[i].transaction[j].amount;
            data[i].transaction[j].sign = a.data[i].transaction[j].sign;
            len = strlen(a.data[i].transaction[j].signature);
            data[i].transaction[j].signature = new char[len+1];
            strcpy(data[i].transaction[j].signature, a.data[i].transaction[j].signature);
            len = strlen(a.data[i].transaction[j].who);
            data[i].transaction[j].who = new char[len+1];
            strcpy(data[i].transaction[j].who, a.data[i].transaction[j].who);
        }
    }
    capacity = a.capacity;
    current = a.current;
    return *this;
}

ostream & operator << (ostream & out, const CAccount & account){
    out << account.accID << ":\n   " << account.initBalance << "\n";
    for(int i = 0; i < account.current; i++){
        int amount = account.transaction[i].amount;
        if(account.transaction[i].amount < 0)
            amount *= -1;
        out << " " << account.transaction[i].sign << " "
        << account.transaction[i].amount;
        if(account.transaction[i].sign == '-')
            out << ", to: ";
        else 
            out << ", from: ";
        out << account.transaction[i].who << ", sign: "
        << account.transaction[i].signature << "\n";
    }
    out << " = " << account.currentBalance << endl;
    return out;
}

void CBank::Print() const{
    for(int i = 0; i < current; i++)
        cout << "acc: " << data[i].accID << " | " << data[i].initBalance 
        << " | cur: " << data[i].currentBalance << endl;
    cout << endl;
}

CAccount CBank::Account(const char * accID){
    // check if accID exists
    for(int i = 0; i < current; i++)
        if(!strcmp(accID, data[i].accID))
            return data[i];
    throw invalid_argument ("not found\n");
}

CBank::CBank(const CBank & a){
    data = new CAccount[a.capacity];
    for(int i = 0; i < a.current; i++){
        int len = strlen(a.data[i].accID);
        data[i].accID = new char[len+1];
        strcpy(data[i].accID, a.data[i].accID);
        data[i].currentBalance = a.data[i].currentBalance;
        data[i].initBalance = a.data[i].initBalance;
        data[i].capacity = a.data[i].capacity;
        data[i].current = a.data[i].current;
        len = a.data[i].capacity;
        data[i].transaction = new CTransaction [len];
        for(int j = 0; j < a.data[i].current; j++){
            data[i].transaction[j].amount = a.data[i].transaction[j].amount;
            data[i].transaction[j].sign = a.data[i].transaction[j].sign;
            len = strlen(a.data[i].transaction[j].signature);
            data[i].transaction[j].signature = new char[len+1];
            strcpy(data[i].transaction[j].signature, a.data[i].transaction[j].signature);
            len = strlen(a.data[i].transaction[j].who);
            data[i].transaction[j].who = new char[len+1];
            strcpy(data[i].transaction[j].who, a.data[i].transaction[j].who);
        }
    }
    capacity = a.capacity;
    current = a.current;
}

bool CBank::NewAccount(const char * accID, int initialBalance){
    // check, if accID is unique
    for(int i = 0; i < current; i++)
        if(!strcmp(accID, data[i].accID))
            return false;

    // realloc if needed
    if(current == capacity){ 
        CAccount * temp = new CAccount[2 * capacity]; 
        for(int i = 0; i < current; i++){
            int len = strlen(data[i].accID);
            temp[i].accID = new char[len+1];
            strcpy(temp[i].accID, data[i].accID);
            delete[] data[i].accID;

            temp[i].initBalance = data[i].initBalance;
            temp[i].currentBalance = data[i].currentBalance; 
            temp[i].capacity = data[i].capacity;
            temp[i].current = data[i].current;
            temp[i].transaction = data[i].transaction;


        }
        delete[] data; 
        capacity *= 2; 
        data = temp; 
    }     
    // add new account
    int len = strlen(accID);
    data[current].accID = new char[len + 1];
    strcpy(data[current].accID, accID);
    data[current].initBalance = initialBalance;  
    data[current].currentBalance = initialBalance; 
    data[current].transaction = new CTransaction[1];
    data[current].capacity = 1;
    data[current].current = 0;
    current++;
    return true;
}

bool CBank::Transaction( const char * debAccID, const char * credAccID,
                    unsigned int amount, const char * signature ){
    // check if it is not the same account
    if(!strcmp(debAccID, credAccID))
        return false;
    
    // find indexes
    int deb_i = 0, cred_i = 0;
    int deb_found = 0, cred_found = 0;
    for(int i = 0; i < current; i++){
        if(deb_found && cred_found)
            break;
        
        if(!strcmp(debAccID, data[i].accID) && !deb_found){
            deb_found = 1;
            deb_i = i;
        }
        if(!strcmp(credAccID, data[i].accID) && !cred_found){
            cred_found = 1;
            cred_i = i;
        }
    }
    if(!deb_found || !cred_found)
        return false;
    
    // deb transaction realloc
    if(data[deb_i].current == data[deb_i].capacity){ 
        CTransaction * temp = new CTransaction[2 * data[deb_i].capacity]; 
        for(int i = 0; i < data[deb_i].capacity; i++){
            temp[i].sign = data[deb_i].transaction[i].sign;
            temp[i].amount = data[deb_i].transaction[i].amount;

            int len = strlen(data[deb_i].transaction[i].who);
            temp[i].who = new char[len+1];
            strcpy(temp[i].who, data[deb_i].transaction[i].who);
            delete[] data[deb_i].transaction[i].who;

            len = strlen(data[deb_i].transaction[i].signature);
            temp[i].signature = new char[len+1];
            strcpy(temp[i].signature, data[deb_i].transaction[i].signature);
            delete[] data[deb_i].transaction[i].signature;
        }
        delete[] data[deb_i].transaction; 
        data[deb_i].capacity *= 2; 
        data[deb_i].transaction = temp; 
    }
    // copy paste realloc pro cred 
    if(data[cred_i].current == data[cred_i].capacity){ 
        CTransaction * temp = new CTransaction[2 * data[cred_i].capacity]; 
        for(int i = 0; i < data[cred_i].capacity; i++){
            temp[i].sign = data[cred_i].transaction[i].sign;
            temp[i].amount = data[cred_i].transaction[i].amount;

            int len = strlen(data[cred_i].transaction[i].who);
            temp[i].who = new char[len+1];
            strcpy(temp[i].who, data[cred_i].transaction[i].who);
            delete[] data[cred_i].transaction[i].who;

            len = strlen(data[cred_i].transaction[i].signature);
            temp[i].signature = new char[len+1]; 
            strcpy(temp[i].signature, data[cred_i].transaction[i].signature);
            delete[] data[cred_i].transaction[i].signature;
        }
        delete[] data[cred_i].transaction; 
        data[cred_i].capacity *= 2; 
        data[cred_i].transaction = temp; 
    }

    // get len of signature and malloc memory needed
    int len = strlen(signature);
    data[deb_i].transaction[data[deb_i].current].signature = new char[len+1];
    data[cred_i].transaction[data[cred_i].current].signature = new char[len+1];


    // add debit transaction
    data[deb_i].currentBalance -= amount;
    data[deb_i].transaction[data[deb_i].current].amount = amount; // index transakce
    data[deb_i].transaction[data[deb_i].current].sign = '-';
    strcpy(data[deb_i].transaction[data[deb_i].current].signature, signature);
    len = strlen(credAccID);
    data[deb_i].transaction[data[deb_i].current].who = new char[len+1];
    strcpy(data[deb_i].transaction[data[deb_i].current].who, credAccID);
    data[deb_i].current++;

    // add credit transaction
    data[cred_i].currentBalance += amount;
    data[cred_i].transaction[data[cred_i].current].amount = amount; // index transakce
    data[cred_i].transaction[data[cred_i].current].sign = '+';
    strcpy(data[cred_i].transaction[data[cred_i].current].signature, signature);
    len = strlen(debAccID);
    data[cred_i].transaction[data[cred_i].current].who = new char[len+1];
    strcpy(data[cred_i].transaction[data[cred_i].current].who, debAccID);
    data[cred_i].current++;
    return true;
}

bool CBank::TrimAccount( const char * accID ){
    int i = 0;
    int found = 0;
    for(; i < current; i++){
        if(!strcmp(accID, data[i].accID)){
            found = 1;
            break;
        }
    }
    if(!found)
        return false;
    
    data[i].initBalance = data[i].currentBalance;
    for(int j = 0; j < data[i].current; j++){
        delete[] data[i].transaction[j].signature;
        delete[] data[i].transaction[j].who;
    }
    delete[] data[i].transaction;
    data[i].current = 0;
    data[i].capacity = 1;
    data[i].transaction = new CTransaction[1];
    return true;
}

#ifndef __PROGTEST__
int main ( void ){
    CBank x666, x667;
    assert ( x666 . NewAccount ( "B", 1000 ) );
    assert ( x666 . NewAccount ( "C", 1000 ) );
    assert ( x666 . Transaction ( "B", "C", 123, "asdf78wrnASDT3W" ) );
    x666 = x666;
    x667 = x666;

    CBank x21, x22, x23, x24, x25, x26, x27, x28;

    assert ( x21 . NewAccount ( "B", 1000 ) );
    assert ( x21 . NewAccount ( "C", 1000 ) );
    assert ( x21 . Transaction ( "B", "C", 123, "asdf78wrnASDT3W" ) );
    x22 = x23 = x24 = x25 = x26 = x27 = x21;

    x21 = x28;
    assert ( x27 . NewAccount ( "A", 1000 ) );
    assert ( x27 . NewAccount ( "D", 1000 ) );
    assert ( x27 . Transaction ( "B", "A", 123, "asdf78wrnASDT3W" ) );

    CBank x31, x32, x33, x34;

    assert ( x31 . NewAccount ( "B", 1000 ) );
    assert ( x31 . NewAccount ( "C", 1000 ) );
    assert ( x31 . Transaction ( "B", "C", 123, "asdf78wrnASDT3W" ) );

    x34 = x32 = x31;
    assert ( x32 . NewAccount ( "A", 1000 ) );
    assert ( x32 . NewAccount ( "D", 1000 ) );
    assert ( x32 . Transaction ( "A", "B", 123, "sssssssssssss" ) );
    assert ( x32 . Transaction ( "C", "D", 123, "sssssssssssss" ) );
    x31 = x33;

    CBank x1;
    assert ( x1 . NewAccount ( "A", 1000 ) );
    assert ( x1 . NewAccount ( "B", 1000 ) );
    assert ( x1 . Transaction ( "A", "B", 123, "sssssssssssss" ) );

    assert ( x1 . TrimAccount ( "A" ) );
    assert ( x1 . TrimAccount ( "B" ) );


    CBank x41;
    assert ( x41 . NewAccount ( "A", 1000 ) );
    assert ( x41 . NewAccount ( "B", 1000 ) );
    assert ( x41 . NewAccount ( "C", 1000 ) );
    assert ( x41 . NewAccount ( "D", 1000 ) );
    assert ( x41 . NewAccount ( "E", 1000 ) );
    assert ( x41 . NewAccount ( "F", 1000 ) );
    assert ( x41 . NewAccount ( "G", 1000 ) );
    assert ( x41 . NewAccount ( "H", 1000 ) );
    assert ( x41 . NewAccount ( "I", 1000 ) );
    assert ( x41 . NewAccount ( "J", 1000 ) );
    assert ( x41 . Transaction ( "C", "D", 123, "sssssssssssss" ) );
    assert ( x41 . Transaction ( "B", "A", 123, "sssssssssssss" ) );
    assert ( x41 . Transaction ( "C", "D", 123, "sssssssssssss" ) );
    assert ( x41 . Transaction ( "C", "E", 123, "sssssssssssss" ) );
    assert ( x41 . Transaction ( "C", "I", 123, "sssssssssssss" ) );
    assert ( x41 . Transaction ( "J", "D", 123, "sssssssssssss" ) );
    assert ( x41 . Transaction ( "C", "D", 123, "sssssssssssss" ) );
    assert ( x41 . Transaction ( "H", "D", 123, "sssssssssssss" ) );
    assert ( x41 . Transaction ( "C", "A", 123, "sssssssssssss" ) );
    assert ( x41 . Transaction ( "G", "D", 123, "sssssssssssss" ) );
    assert ( x41 . Transaction ( "C", "D", 123, "sssssssssssss" ) );
    assert ( x41 . Transaction ( "E", "C", 123, "sssssssssssss" ) );
    assert ( x41 . Transaction ( "C", "D", 123, "sssssssssssss" ) );
    assert ( x41 . Transaction ( "C", "B", 123, "sssssssssssss" ) );
    assert ( x41 . Transaction ( "C", "F", 123, "sssssssssssss" ) );
    assert ( x41 . Transaction ( "C", "F", 123, "sssssssssssss" ) );

    CBank x42(x41);
    CBank x43(x41);
    CBank x45;
    assert ( x45 . NewAccount ( "A", 1000 ) );
    assert ( x45 . NewAccount ( "B", 1000 ) );
    assert ( x45 . NewAccount ( "C", 1000 ) );
    assert ( x45 . NewAccount ( "D", 1000 ) );
    assert ( x45 . NewAccount ( "E", 1000 ) );
    assert ( x45 . Transaction ( "B", "A", 123, "sssssssssssss" ) );
    assert ( x45 . Transaction ( "C", "D", 123, "sssssssssssss" ) );
    assert ( !x45 . Transaction ( "J", "D", 123, "sssssssssssss" ) );
    assert ( x45 . Transaction ( "C", "D", 123, "sssssssssssss" ) );
    assert ( !x45 . Transaction ( "H", "D", 123, "sssssssssssss" ) );
    x43 = x45;
    CBank x44(x41);
    assert ( x45 . TrimAccount ( "A" ) );
    assert ( x45 . TrimAccount ( "B" ) );
    assert ( x45 . TrimAccount ( "C" ) );
    assert (! x41 . NewAccount ( "J", 1000 ) );
    assert ( x41 . NewAccount ( "K", 1000 ) );
    assert ( x41 . NewAccount ( "L", 1000 ) );
    assert ( x41 . NewAccount ( "M", 1000 ) );
    assert ( x41 . Transaction ( "C", "K", 123, "sssssssssssss" ) );
    assert ( x41 . Transaction ( "J", "M", 123, "sssssssssssss" ) );
    assert ( x41 . Transaction ( "I", "D", 123, "sssssssssssss" ) );

    assert ( x41 . TrimAccount ( "A" ) );
    assert ( x41 . TrimAccount ( "B" ) );
    assert ( x41 . TrimAccount ( "C" ) );
    assert ( x41 . TrimAccount ( "J" ) );
    assert ( x41 . TrimAccount ( "K" ) );

    x41 = x45;
    x44 = x41;
    return 0;
}
#endif /* __PROGTEST__ */

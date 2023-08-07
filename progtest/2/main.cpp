/*
Program works as a database that holds information about people and their
incomes and expenses. Each person is identified by a name, an address and an 
account number. Each account number must be unique. There can be only one 
unique combination of a name and an adress, however, a name and an address
can be used multiple times (with different addresses/names).
*/

#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <stdexcept>
#include <algorithm>

using namespace std;

#endif

// class People to store essential information about each person ===========
class People {
    public:
        string name, address, account;
        int income, expense;
};

// class Iterator to iterate trough database and get required information ==
class CIterator {
    public:
        bool AtEnd( void ) const; 
        void Next( void ); 
        string Name( void ) const;
        string Addr( void ) const;
        string Account( void ) const;
        CIterator( int index = 0 ){ this->index = index; }; // constructor
        void Copy( vector<People> data, int index );
  private:
        unsigned int index;  // current index of data
        unsigned int size;   // integer to store the size of our vector
        vector<People> data; // vector to store copied database
};

// method to copy our vector from CTaxRegister to Citerator so we can iterate
void CIterator::Copy(vector<People> datas, int index){
    this->size = index;
    this->data = datas;
}

// method that checks if we are in or behind the end of database ===========
bool CIterator::AtEnd( void ) const {
    if(index >= size)
        return true;
    return false;
}

// method that increases index =============================================
void CIterator::Next( void ) {
    index++;
}

// method that returns name of a person at current index ===================
string CIterator::Name( void ) const {
    if(AtEnd())
        return NULL;
    return data.at(index).name;
}

// method that returns address of a person at current index ================
string CIterator::Addr( void ) const {   
    if(AtEnd())
        return NULL;
    return data.at(index).address;
}

// method that returns account number of a person at current index =========
string CIterator::Account( void ) const {
    if(AtEnd())
        return NULL;
    return data.at(index).account;
}

// class Tax Register to edit our database =================================
class CTaxRegister {
    public:
        bool Birth( const string & name,
                    const string & addr,
                    const string & account );
        bool Death( const string & name,
                    const string & addr );
        bool Income( const string & account,
                     int amount );
        bool Income( const string & name,
                     const string & addr, 
                     int amount );
        bool Expense( const string & account, 
                      int amount );
        bool Expense( const string & name, 
                      const string & addr, 
                      int amount );
        bool Audit( const string & name,
                    const string & addr,
                    string & account,
                    int & sumIncome, 
                    int & sumExpense) const;
        CIterator ListByName( void ) const;
        CTaxRegister( int index = 0 ){ this->index = index; }; // constructor
        void Print( void ) const;
  private:
        int index; // integer to save size of vector
        vector<People> data; // vector that stores our database
};

// function to print database (if needed) ==================================
void CTaxRegister::Print( void ) const {
    for(unsigned i = 0; i < data.size(); i++)
        cout << data.at(i).name << " | " << data.at(i).address 
             << " | " << data.at(i).account << " | I: "
             << data.at(i).income << " | E:  " << data.at(i).expense << endl;
    cout << "=================\n";
}

/* 
In order to efficiently search trough our database we need it to be sorted.
This function takes two persons and finds out in which order should they be 
placed in our database. It firstly checks their names, if they are the same, 
it check their addresses (they cannot be the same, we check that in Birth).
It returns information about which person should be higher and which lower 
in our database. 
*/
bool compare(const People &a, const People &b){
    if(a.name == b.name)
        return (a.address < b.address);
    return (a.name < b.name);
}

// method to add new person to database ====================================
bool CTaxRegister::Birth( const string & name, const string & addr,
                          const string & account ){
    // at first we create new class member according to given information
    People person;
    person.name = name;
    person.address = addr;
    person.account = account;
    person.income = 0;
    person.expense = 0;
    // if it is the first record, simply add the person to database
    if(!index){
        data.push_back(person);
        index++;
        return true;
    }
    // if not, check if it is possible to have such record in our database
    for(unsigned i = 0; i < data.size(); i++){
        if(data.at(i).account == account)
            return false;
        if(data.at(i).name == name && data.at(i).address == addr)
            return false;
    }
    // insert new person to our database at index that is determined by
    // lower_bound function
    vector<People>::const_iterator it;
    it = lower_bound(data.begin(), data.end(), person, compare);
    data.insert(it, person);
    index++;
    return true;
}

// remove person from database =============================================
bool CTaxRegister::Death( const string & name, const string & addr ){
    for(unsigned i = 0; i < data.size(); i++){
        if(data.at(i).name == name && data.at(i).address == addr){
            data.erase(data.begin() + i);
            index--;
            return true;
        }
    }
    return false;
}

// raise income for a person according to its account number ===============
bool CTaxRegister::Income( const string & account, int amount ){
    for(unsigned i = 0; i < data.size(); i++){
        if(data.at(i).account == account){
            data.at(i).income += amount;
            return true;
        }
    }
    return false;
}

// raise income for a person according to its name and address =============
bool CTaxRegister::Income( const string & name, const string & addr,
                           int amount ){
    People person;
    person.name = name;
    person.address = addr;
    // if we find such person, get its index and raise income
    if(binary_search(data.begin(), data.end(), person, compare)){
        vector<People>::const_iterator it;
        it = lower_bound(data.begin(), data.end(), person, compare);
        data.at( it - data.begin() ).income += amount;
        return true;
    }
    return false;                                   
}

// register expense for a person according to its account ==================
bool CTaxRegister::Expense( const string & account, int amount ){
    for(unsigned i = 0; i < data.size(); i++){
        if(data.at(i).account == account){
            data.at(i).expense += amount;
            return true;
        }
    }
    return false;
}

// register expense for a person according to its name and address =========
bool CTaxRegister::Expense( const string & name, const string & addr,
                            int amount ){
    People person;
    person.name = name;
    person.address = addr;
    // if we find such person, get its index and register expense
    if(binary_search(data.begin(), data.end(), person, compare)){
        vector<People>::const_iterator it;
        it = lower_bound(data.begin(), data.end(), person, compare);
        data.at( it - data.begin() ).expense += amount;
        return true;
    }
    return false; 
}

// do audit for a person, return sums of its incomes and expenses ==========
bool CTaxRegister::Audit( const string & name, const string & addr, 
                          string & account, int & sumIncome, 
                          int & sumExpense) const{
    People person;
    person.name = name;
    person.address = addr;
    // if person is found, save info into parameters
    if(binary_search(data.begin(), data.end(), person, compare)){
        vector<People>::const_iterator it;
        it = lower_bound(data.begin(), data.end(), person, compare);
        sumExpense = data.at( it - data.begin() ).expense;
        sumIncome = data.at( it - data.begin() ).income;
        account = data.at( it - data.begin() ).account;
        return true;
    }
    return false; 
}

// create instance of CIterator class by copying the vector and passing size
CIterator CTaxRegister::ListByName( void ) const {
    CIterator instance;
    instance.Copy(data, index);
    return instance;
}

#ifndef __PROGTEST__
int main(void){

    string acct;
    int sumIncome, sumExpense;
    CTaxRegister b1;
    
    assert ( b1 . Birth ( "John Smith", "Oak Road 23", "123/456/789" ) );
    assert ( b1 . Birth ( "Jane Hacker", "Main Street 17", "Xuj5#94" ) );
    assert ( b1 . Birth ( "Peter Hacker", "Main Street 17", "634oddT" ) );
    assert ( b1 . Birth ( "John Smith", "Main Street 17", "Z343rwZ" ) );    

    assert ( b1 . Income ( "Xuj5#94", 1000 ) );
    assert ( b1 . Income ( "634oddT", 2000 ) );
    assert ( b1 . Income ( "123/456/789", 3000 ) );
    assert ( b1 . Income ( "634oddT", 4000 ) );
    assert ( b1 . Income ( "Peter Hacker", "Main Street 17", 2000 ) );

    assert ( b1 . Expense ( "Jane Hacker", "Main Street 17", 2000 ) );
    assert ( b1 . Expense ( "John Smith", "Main Street 17", 500 ) );
    assert ( b1 . Expense ( "Jane Hacker", "Main Street 17", 1000 ) );
    assert ( b1 . Expense ( "Xuj5#94", 1300 ) );

    assert ( b1 . Audit ( "John Smith", "Oak Road 23", acct, sumIncome, sumExpense ) );
    assert ( acct == "123/456/789" );
    assert ( sumIncome == 3000 );
    assert ( sumExpense == 0 );

    assert ( b1 . Audit ( "Jane Hacker", "Main Street 17", acct, sumIncome, sumExpense ) );
    assert ( acct == "Xuj5#94" );
    assert ( sumIncome == 1000 );
    assert ( sumExpense == 4300 );
    assert ( b1 . Audit ( "Peter Hacker", "Main Street 17", acct, sumIncome, sumExpense ) );
    assert ( acct == "634oddT" );
    assert ( sumIncome == 8000 );
    assert ( sumExpense == 0 );
    assert ( b1 . Audit ( "John Smith", "Main Street 17", acct, sumIncome, sumExpense ) );
    assert ( acct == "Z343rwZ" );
    assert ( sumIncome == 0 );
    assert ( sumExpense == 500 );

    CIterator it = b1 . ListByName ();
    assert ( ! it . AtEnd ()
            && it . Name () == "Jane Hacker"
            && it . Addr () == "Main Street 17"
            && it . Account () == "Xuj5#94" );
    it . Next ();

    assert ( ! it . AtEnd ()
            && it . Name () == "John Smith"
            && it . Addr () == "Main Street 17"
            && it . Account () == "Z343rwZ" );
    it . Next ();

    assert ( ! it . AtEnd ()
            && it . Name () == "John Smith"
            && it . Addr () == "Oak Road 23"
            && it . Account () == "123/456/789" );
    it . Next ();
    
    assert ( ! it . AtEnd ()
            && it . Name () == "Peter Hacker"
            && it . Addr () == "Main Street 17"
            && it . Account () == "634oddT" );
    it . Next ();

    assert ( it . AtEnd () );
    assert ( b1 . Death ( "John Smith", "Main Street 17" ) );

    CTaxRegister b2;
    assert ( b2 . Birth ( "John Smith", "Oak Road 23", "123/456/789" ) );
    assert ( b2 . Birth ( "Jane Hacker", "Main Street 17", "Xuj5#94" ) );
    assert ( !b2 . Income ( "634oddT", 4000 ) );
    assert ( !b2 . Expense ( "John Smith", "Main Street 18", 500 ) );
    assert ( !b2 . Audit ( "John Nowak", "Second Street 23", acct, sumIncome, sumExpense ) );
    assert ( !b2 . Death ( "Peter Nowak", "5-th Avenue" ) );
    assert ( !b2 . Birth ( "Jane Hacker", "Main Street 17", "4et689A" ) );
    assert ( !b2 . Birth ( "Joe Hacker", "Elm Street 23", "Xuj5#94" ) );
    assert ( b2 . Death ( "Jane Hacker", "Main Street 17" ) );
    assert ( b2 . Birth ( "Joe Hacker", "Elm Street 23", "Xuj5#94" ) );
    assert ( b2 . Audit ( "Joe Hacker", "Elm Street 23", acct, sumIncome, sumExpense ) );
    assert ( acct == "Xuj5#94" );
    assert ( sumIncome == 0 );
    assert ( sumExpense == 0 );
    assert ( !b2 . Birth ( "Joe Hacker", "Elm Street 23", "AAj5#94" ) );

    return 0;
}
#endif 

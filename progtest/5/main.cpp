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
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>
#endif /* __PROGTEST */
using namespace std;

#ifndef __PROGTEST__
// cas a datum doruceni mailu
class CTimeStamp {
    public:
        // konstruktor
        CTimeStamp (int year, int month, int day, int hour, int minute, 
                    int sec );
        // porovna instanci a parametr x, return viz progtest
        int Compare ( const CTimeStamp & x ) const;
        // zobrazi reprezentovany casovy udaj v podobe YYYY-MM-DD HH24:MI:SS
        friend ostream & operator << ( ostream & os, const CTimeStamp & x );
    private:
        int year, month, day, hour, minute, sec;
};

ostream & operator << ( ostream & os, const CTimeStamp & x ){
    os << x.year << "-";
    if(x.month / 10 == 0)
        os << "0";
    os << x.month << "-";
    if(x.day / 10 == 0)
        os << "0";
    os << x.day << " ";
    if(x.hour / 10 == 0)
        os << "0";
    os << x.hour << ":";
    if(x.minute / 10 == 0)
        os << "0";
    os << x.minute << ":";
    if(x.sec / 10 == 0)
        os << "0";
    os << x.sec << " ";
    return os;
}

int CTimeStamp::Compare( const CTimeStamp & x ) const {
    if(year < x.year)
        return -1;
    else if(year > x.year)
        return 1;
    else
        if(month < x.month)
            return -1;
        else if(month > x.month)
            return 1;
        else
            if(day < x.day)
                return -1;
            else if(day > x.day)
                return 1;
            else
                if(hour < x.hour)
                    return -1;
                else if(hour > x.hour)
                    return 1;
                else
                    if(minute < x.minute){
                        return -1;
                    }
                    else if(minute > x.minute)
                        return 1;
                    else
                        if(sec < x.sec)
                            return -1;
                        else if(sec > x.sec)
                            return 1;
    return 0;
}

CTimeStamp::CTimeStamp (int year, int month, int day, int hour, int minute, 
                        int sec ){
    this->year = year;
    this->month = month;
    this->day = day;
    this->hour = hour;
    this->minute = minute;
    this->sec = sec;
}

// zabaluje mail
class CMailBody {
    public:
        // konstruktor
        CMailBody ( int size, const char * data );
        // copy cons/op=/destructor is correctly implemented in the testing environment
        // << jen pro ladeni
        friend ostream & operator << ( ostream & os, const CMailBody & x ){
            return os << "mail body: " << x . m_Size << " B";
        }
    private:
        int m_Size;
        char * m_Data;
};

CMailBody::CMailBody(int size, const char * data){
    this->m_Size = size;
    this->m_Data = new char[size+1];
    strcpy(this->m_Data, data);
}

// priloha mailu
class CAttach {
    public:
        // konstruktor
        CAttach ( int x ) : m_X (x), m_RefCnt ( 1 ){}
        // zvedne pocet odkazu, vola vzdy ten, kdo ziska odkaz na objekt
        void AddRef ( void ) const { 
            m_RefCnt ++; 
        }
        // snizi pocet odkazu, vola ten, kdo vola treba destruktor 
        void Release ( void ) const { 
            if ( !--m_RefCnt ) 
                delete this; 
        }
    private:
        int m_X;
        // tento int lze modifikovat i v const metodach
        mutable int m_RefCnt;
        // kopirujici konstruktor
        CAttach ( const CAttach & x );
        // operator prirazeni
        CAttach & operator = ( const CAttach   & x );
        // destrutkor
        ~CAttach ( void ) = default;
        // << pouze pro testovani
        friend ostream & operator << ( ostream & os, const CAttach   & x ){
            return os << "attachment: " << x . m_X << " B";
        }
};
#endif /* __PROGTEST__, DO NOT remove */

class CMail {
    public: 
        // konstruktor
        CMail ( const CTimeStamp & timeStamp,const string & from,
                    const CMailBody  & body, const CAttach * attach );
        // destruktor, kopirujici konstruktor a operator = if needed
        // gettery
        const string & From ( void ) const;
        const CMailBody & Body ( void ) const;
        const CTimeStamp & TimeStamp ( void ) const;
        const CAttach * Attachment ( void ) const;
        // datum, odesilatel, telo mailu, priloha (pokud vse existuje)
        friend ostream & operator << ( ostream & os, const CMail & x );

        void SetPointer( int p ){
            pointer = p;
        }
        const int & Pointer( void ) const;
    private:
        int pointer;
        string from;
        CTimeStamp time;
        CMailBody body;
        const CAttach * attach;
};

ostream & operator << ( ostream & os, const CMail & x ){
    os << x.TimeStamp() << x.from << " " << x.Body();
    if(x.attach != nullptr)
        os << " + " << *(x.Attachment());
    return os;
}

const int & CMail::Pointer(void) const {
    return this->pointer;
}

const string & CMail::From ( void ) const {
    return from;
}

const CTimeStamp & CMail::TimeStamp ( void ) const {
    return time;
}
const CMailBody & CMail::Body ( void ) const {
    return body;
}
const CAttach * CMail::Attachment ( void ) const {
    return attach;
}

CMail::CMail ( const CTimeStamp & timeStamp, const string & from,
               const CMailBody  & bod, const CAttach * att )
    : time(timeStamp), body(bod), attach(att) {
    this->from = from;
    this->pointer = 0;
    if(att != nullptr)
        attach->AddRef();
}

class CFolder {
    public:
        bool operator < (const CFolder & a) const {
            return name < a.name;
        }
        int pointer;
        // kazda slozka ma sve pole pointeru na maily
        vector<CMail> mails;
        string name;    
        int count;
};

// jedna emailova schranka
// může obsahovat došlou poštu (instance třídy CMail)
// pošta může být rozdělena do složek.
class CMailBox {
    public:
        // konstruktor jedne slozky s nazvem inbox
        CMailBox ( void );
        // destruktor, kopirujici konstruktor a operator = if needed
        // prida mail do inboxu, nemelo by selhat
        bool Delivery ( const CMail & mail );
        // nova prazdna slozka
        bool NewFolder ( const string & folderName );
        // presun emailu ze slozek, puvodni slozka bude pak prazdna
        bool MoveMail ( const string & fromFolder, const string & toFolder );
        // seznam mailu ze zadane slozky v zadanem rozmezi casu <uzavreny>
        list<CMail> ListMail ( const string & folderName, 
                               const CTimeStamp & from,
                               const CTimeStamp & to ) const;
        // mnozina adres z daneho casoveho intervalu <uzavreny interval>
        // hleda ve vsech slozkach
        set<string> ListAddr ( const CTimeStamp & from, 
                               const CTimeStamp & to ) const;

        void Print( void ) const;
        void PrintFolder( void ) const;
    private:
        vector<CFolder> folders;
        // info o poctu slozek, zaroven priradi pointer (index) slozce
        int folders_index; 
};


void CMailBox::PrintFolder( void ) const{
    cout << "================\n";
    for(unsigned i = 0; i < folders.size(); i++)
        cout << folders.at(i).name << endl;
    cout << "================\n";
}
void CMailBox::Print( void ) const {
    cout << "==================================================================\n";
    cout << "Print:\n";
    for(unsigned i = 0; i < folders.at(0).mails.size(); i++)
        cout << folders.at(0).mails.at(i) << endl;
    cout << "==================================================================\n"; 
}

// default constructor, create inbox folder with pointer 0
CMailBox::CMailBox (void) {
    folders_index = 0;
    CFolder tmp;
    tmp.name = "inbox";
    tmp.count = 0;
    tmp.pointer = 0;
    folders.push_back(tmp);
}

// compare files and mails =================================================
bool compare_f(const CFolder &a, const CFolder &b){
    // cout << a.name << " "<< b.name << endl;
    return (a.name < b.name);
}
bool compare_m(const CMail &a, const CMail &b){
    int c = a.TimeStamp().Compare(b.TimeStamp());
    if(c <= 0)
        return true;
    return false;
}

// Move Mail ===============================================================
bool CMailBox::MoveMail (const string & fromFolder, const string & toFolder){
    CFolder from;
    from.name = fromFolder;
    auto it = lower_bound(folders.begin(), folders.end(), from, compare_f);
    unsigned i_from = distance( folders.begin(), it);

    CFolder to;
    to.name = toFolder;
    it = lower_bound(folders.begin(), folders.end(), to, compare_f);
    unsigned i_to = distance( folders.begin(), it);

    if(i_from >= folders.size() || i_to >= folders.size())
        return false;
    if(folders.at(i_from).name != fromFolder || folders.at(i_to).name != toFolder)
        return false;
    if(fromFolder == toFolder)
        return false;
    for(int i = 0; i < folders.at(i_from).count; i++){
        auto it = lower_bound(folders.at(i_to).mails.begin(),
                              folders.at(i_to).mails.end(), 
                              folders.at(i_from).mails.at(i),
                              compare_m);
        folders.at(i_to).mails.insert(it, folders.at(i_from).mails.at(i));
        folders.at(i_to).count++;
    }
    folders.at(i_from).mails.clear();
    folders.at(i_from).count = 0;
    return true; 
}

// Delivery ================================================================
bool CMailBox::Delivery ( const CMail & mail ){
    auto it = lower_bound(folders.at(0).mails.begin(), folders.at(0).mails.end(), mail, compare_m);
    folders.at(0).mails.insert(it, mail);
    folders.at(0).count++;
    //if(mail.Attachment != nullptr) ->> ref++ ?
    return true;
}

// New Folder ==============================================================
bool CMailBox::NewFolder ( const string & folderName ){
    CFolder tmp;
    tmp.name = folderName;

    // check if it is unique folder
    if(binary_search(folders.begin(), folders.end(), tmp, compare_f) || 
       folders.at(folders.size()-1).name == folderName ) 
        return false;

    folders_index++;
    tmp.pointer = folders_index;
    folders.push_back(tmp);
    return true;
}

// compare mail time and time ==============================================
bool compare_time(const CMail &a, const CTimeStamp &b){
    int c = a.TimeStamp().Compare(b);
    if(c < 0)
        return true;
    return false;
}

// List Mail ===============================================================
list<CMail> CMailBox::ListMail ( const string & folderName, 
        const CTimeStamp & from, const CTimeStamp & to ) const{
    list<CMail> res;
    CFolder folder;
    folder.name = folderName;

    // if not found, return empty
    auto ch = lower_bound(folders.begin(), folders.end(), folder, compare_f);
    if(ch->name.compare(folderName))
        return res;

    auto it = lower_bound ( ch->mails.begin(), ch->mails.end(), from,
                            compare_time);
    unsigned i = distance( ch->mails.begin(), it);
    for( ; i < ch->mails.size(); i++){
        // dalsi cas je vetsi, break
        if(it->TimeStamp().Compare(to) > 0)
            break;
        res.push_back(ch->mails.at(i));
        it++;
    }
    return res;
}

// List Addr ===============================================================
set<string> CMailBox::ListAddr ( const CTimeStamp & from,
                              const CTimeStamp & to ) const {
    set<string> res;
    for(unsigned i = 0; i < folders.size(); i++){
        // najdi prvni nejblizsi cas ze slozky
        auto it = lower_bound ( folders.at(i).mails.begin(),
                                folders.at(i).mails.end(),
                                from,
                                compare_time);
        while( it != folders.at(i).mails.end() ){
            // dalsi cas je vetsi, break
            if(it->TimeStamp().Compare(to) > 0)
                break;
            res.insert(it->From());
            it++;
        }
    }
    return res;
}

//=================================================================================================
#ifndef __PROGTEST__

static string showMail ( const list<CMail> & l ){
    ostringstream oss;
    for ( const auto & x : l ){
        //cout << x << endl;
        oss << x << endl;
    }
    return oss . str ();
}

static string showUsers ( const set<string> & s ){
    ostringstream oss;
    for ( const auto & x : s )
        oss << x << endl;
    return oss . str ();
}

int main ( void ){
    list<CMail> mailList;
    set<string> users;
    CAttach   * att;

    CMailBox m0;
    assert ( m0 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 15, 24, 13 ), "user1@fit.cvut.cz", CMailBody ( 14, "mail content 1" ), nullptr ) ) );
    assert ( m0 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 15, 26, 23 ), "user2@fit.cvut.cz", CMailBody ( 22, "some different content" ), nullptr ) ) );

    att = new CAttach ( 200 );
    assert ( m0 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 11, 23, 43 ), "boss1@fit.cvut.cz", CMailBody ( 14, "urgent message" ), att ) ) );
    assert ( m0 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 18, 52, 27 ), "user1@fit.cvut.cz", CMailBody ( 14, "mail content 2" ), att ) ) );
    att -> Release ();
    att = new CAttach ( 97 );
    assert ( m0 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 16, 12, 48 ), "boss1@fit.cvut.cz", CMailBody ( 24, "even more urgent message" ), att ) ) );
    att -> Release ();
    
    //m0.Print();
  
    assert ( showMail ( m0 . ListMail ( "inbox",
                        CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                        CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == 
                            "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B + attachment: 200 B\n"
                            "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
                            "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B\n"
                            "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B + attachment: 97 B\n"
                            "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 200 B\n" );
    assert ( showMail ( m0 . ListMail ( "inbox",
                        CTimeStamp ( 2014, 3, 31, 15, 26, 23 ),
                        CTimeStamp ( 2014, 3, 31, 16, 12, 48 ) ) ) == "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B\n"
                            "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B + attachment: 97 B\n" );

    assert ( showUsers ( m0 . ListAddr ( CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                        CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "boss1@fit.cvut.cz\n"
                            "user1@fit.cvut.cz\n"
                            "user2@fit.cvut.cz\n" );
    assert ( showUsers ( m0 . ListAddr ( CTimeStamp ( 2014, 3, 31, 15, 26, 23 ),
                        CTimeStamp ( 2014, 3, 31, 16, 12, 48 ) ) ) == "boss1@fit.cvut.cz\n"
                            "user2@fit.cvut.cz\n" );

    CMailBox m1;
    assert ( m1 . NewFolder ( "work" ) );
    assert ( m1 . NewFolder ( "spam" ) );
    assert ( !m1 . NewFolder ( "spam" ) );

    assert ( m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 15, 24, 13 ), "user1@fit.cvut.cz", CMailBody ( 14, "mail content 1" ), nullptr ) ) );
    att = new CAttach ( 500 );
    assert ( m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 15, 26, 23 ), "user2@fit.cvut.cz", CMailBody ( 22, "some different content" ), att ) ) );
    att -> Release ();
    assert ( m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 11, 23, 43 ), "boss1@fit.cvut.cz", CMailBody ( 14, "urgent message" ), nullptr ) ) );
    att = new CAttach ( 468 );
    assert ( m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 18, 52, 27 ), "user1@fit.cvut.cz", CMailBody ( 14, "mail content 2" ), att ) ) );
    att -> Release ();
    assert ( m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 16, 12, 48 ), "boss1@fit.cvut.cz", CMailBody ( 24, "even more urgent message" ), nullptr ) ) );
    assert ( showMail ( m1 . ListMail ( "inbox",
                        CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                        CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
                            "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
                            "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
                            "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
                            "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n" );

    assert ( showMail ( m1 . ListMail ( "work",
                        CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                        CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "" );
                    
    assert ( m1 . MoveMail ( "inbox", "work" ) );

    assert ( showMail ( m1 . ListMail ( "inbox",
                        CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                        CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "" );

    assert ( showMail ( m1 . ListMail ( "work",
                        CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                        CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
                            "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
                            "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
                            "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
                            "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n" );

    assert ( m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 19, 24, 13 ), "user2@fit.cvut.cz", CMailBody ( 14, "mail content 4" ), nullptr ) ) );
    att = new CAttach ( 234 );
    assert ( m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 13, 26, 23 ), "user3@fit.cvut.cz", CMailBody ( 9, "complains" ), att ) ) );
    att -> Release ();
    assert ( showMail ( m1 . ListMail ( "inbox",
                        CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                        CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "2014-03-31 13:26:23 user3@fit.cvut.cz mail body: 9 B + attachment: 234 B\n"
                            "2014-03-31 19:24:13 user2@fit.cvut.cz mail body: 14 B\n" );
    assert ( showMail ( m1 . ListMail ( "work",
                        CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                        CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
                            "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
                            "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
                            "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
                            "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n" );
    assert ( m1 . MoveMail ( "inbox", "work" ) );
    assert ( showMail ( m1 . ListMail ( "inbox",
                        CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                        CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "" );
    assert ( showMail ( m1 . ListMail ( "work",
                        CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
                        CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) ) == "2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B\n"
                            "2014-03-31 13:26:23 user3@fit.cvut.cz mail body: 9 B + attachment: 234 B\n"
                            "2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B\n"
                            "2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B\n"
                            "2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B\n"
                            "2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B\n"
                            "2014-03-31 19:24:13 user2@fit.cvut.cz mail body: 14 B\n" );

    return 0;
}
#endif /* __PROGTEST__ */

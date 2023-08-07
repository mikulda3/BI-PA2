#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>

using namespace std;

#endif /* __PROGTEST__ */

class CBigInt {
    public:
        CBigInt(){ this->number.push_back('0'); this->sign = '+';}; 
        // copying/assignment/destruction
        CBigInt(const int number); 
        CBigInt(const char * number);

        // operator +, any combination {CBigInt/int/string} + {CBigInt/int/string}
        friend CBigInt operator + (const CBigInt & a, const CBigInt & b);
        friend CBigInt operator + (const CBigInt & a, const int b);
        friend CBigInt operator + (const CBigInt & a, const char * b);
        friend CBigInt operator + (const int a, const CBigInt & b);
        friend CBigInt operator + (const char * a, const CBigInt & b);
    
        // operator *, any combination {CBigInt/int/string} * {CBigInt/int/string}
        friend CBigInt operator * (const CBigInt & a, const CBigInt & b);
        friend CBigInt operator * (const CBigInt & a, const int b);
        friend CBigInt operator * (const CBigInt & a, const char * b);
        friend CBigInt operator * (const int a, const CBigInt & b);
        friend CBigInt operator * (const char * a, const CBigInt & b);

        // operator +=, any of {CBigInt/int/astring}
        friend CBigInt operator += (CBigInt & a, const CBigInt & b);
        friend CBigInt operator += (CBigInt & a, const int b);
        friend CBigInt operator += (CBigInt & a, const char * b);

        // operator *=, any of {CBigInt/int/string}
        friend CBigInt operator *= (CBigInt & a, const CBigInt & b);
        friend CBigInt operator *= (CBigInt & a, const int b);
        friend CBigInt operator *= (CBigInt & a, const char * b);

        // comparison operators, any combination {CBigInt/int/string} {<,<=,>,>=,==,!=} {CBigInt/int/string}
        friend bool operator > (const CBigInt & a, const CBigInt & b);
        friend bool operator > (const CBigInt & a, const int b);
        friend bool operator > (const CBigInt & a, const char * b);
        friend bool operator > (const int a, const CBigInt & b);
        friend bool operator > (const char * a, const CBigInt & b);

        friend bool operator < (const CBigInt & a, const CBigInt & b);
        friend bool operator < (const CBigInt & a, const int b);
        friend bool operator < (const CBigInt & a, const char * b);
        friend bool operator < (const int a, const CBigInt & b);
        friend bool operator < (const char * a, const CBigInt & b);
        
        friend bool operator >= (const CBigInt & a, const CBigInt & b);
        friend bool operator >= (const CBigInt & a, const int b);
        friend bool operator >= (const CBigInt & a, const char * b);
        friend bool operator >= (const int a, const CBigInt & b);
        friend bool operator >= (const char * a, const CBigInt & b);

        friend bool operator <= (const CBigInt & a, const CBigInt & b);
        friend bool operator <= (const CBigInt & a, const int b);
        friend bool operator <= (const CBigInt & a, const char * b);
        friend bool operator <= (const int a, const CBigInt & b);
        friend bool operator <= (const char * a, const CBigInt & b);

        friend bool operator == (const CBigInt & a, const CBigInt & b);
        friend bool operator == (const CBigInt & a, const int b);
        friend bool operator == (const CBigInt & a, const char * b);
        friend bool operator == (const int a, const CBigInt & b);
        friend bool operator == (const char * a, const CBigInt & b);

        friend bool operator != (const CBigInt & a, const CBigInt & b);
        friend bool operator != (const CBigInt & a, const int b);
        friend bool operator != (const CBigInt & a, const char * b);
        friend bool operator != (const int a, const CBigInt & b);
        friend bool operator != (const char * a, const CBigInt & b);
        
        // output operator <<
        friend ostream & operator << (ostream & out, const CBigInt & a);
        // input operator >>
        friend bool operator >> (istream & in, CBigInt & a);

        void print( void ) const;
    private:
        char sign;
        string number;
};

bool check_string(string tmp, string & fin, char & sign, int type){
    if(tmp[0] == '-' || tmp[0] =='+'){
        if((2 >= tmp.size() && !isdigit(tmp[1])) || tmp.size() < 2)
            return false;
        sign = tmp[0];
        tmp.erase(0,1);
    } else {
        sign = '+';
    }
    int check = 0, null = 0, space = 0, done = 0;
    for(unsigned i = 0; i < tmp.size(); i++){
        if(tmp[i] == '\\' && i+1 <= tmp.size() && tmp[i+1] == 't'){
            i++;
            continue;
        }
        if(!isdigit(tmp[i]) && done) {
            check = 1;
            if(type)
                return false;
            break;
        }
        if(isdigit(tmp[i]))
            done = 1;
        if(space && (tmp[i] == '-' || tmp[i] == '+')){
            sign = tmp[i];
            space = 0;
            check = 1;
            continue;   
        } else if(tmp[i] == ' '){
            space = 1;
            continue;
        } else if(tmp[i] == '0' && !check){
            null = 1;
            continue;
        } else if(!isdigit(tmp[i])){
            return false;
        }
        fin.push_back(tmp[i]);
        check = 1;
    }
    if(null && fin == ""){
        fin.push_back('0');
        sign = '+';
    }
    if(fin == "")
        return false;
    return true;
}

// add ===================================================================
string add_big_ints(int size_1, int size_2, string & result, string str_1,
                    string str_2){
    int i_1 = 0, i_2 = 0, remainder = 0;
    while(i_2 < size_2){
        int count = -remainder;
        count += str_2[size_2 - (++i_2)] - 48;
        count -= (i_1 < size_1) ? str_1[size_1 - (++i_1)] - 48: 0;
        if(count < 0){
            remainder = 1;
            count += 10;
        } else 
            remainder = 0;
        result.insert(0, to_string(count%10));
    }
    return result;
}
void add(string & result, string str_1, string str_2, char & sign_1,
         char sign_2, int size_1, int size_2, int type){
    if((sign_1 == '+' && sign_2 != '-') || (sign_1 == '-' && sign_2 == '-')){
        int i_1 = 0, i_2 = 0, remainder = 0;
        while(i_1 < size_1 || i_2 < size_2 || remainder > 0){
            int count = remainder;
            count += (i_1 < size_1) ? str_1[size_1 - (++i_1)] - 48: 0;
            count += (i_2 < size_2) ? str_2[size_2 - (++i_2)] - 48: 0;
            result.insert(0, to_string(count%10));
            remainder = count/10;
        }
    } else {
        if(size_1 < size_2 || (str_1[0] < str_2[0] && size_1 == size_2)){
            add_big_ints(size_1, size_2, result, str_1, str_2);
            sign_1 = sign_2;
        } else if(size_1 > size_2 || (str_1[0] > str_2[0] && size_1 == size_2)){
            add_big_ints(size_2, size_1, result, str_2, str_1);
        } else {
            int check = 1;  
            for(int i = 0; i < size_1; i++){
                if(str_1[i] < str_2[i])
                    break;
                if(str_1[i] > str_2[i]){
                    add_big_ints(size_2, size_1, result, str_2, str_1);
                    check = 0;
                    sign_1 = sign_2;
                    break;
                }
            }
            if(check){
                add_big_ints(size_1, size_2, result, str_1, str_2);
                sign_1 = sign_2;
            }
        }
    }
    // get rid of new excesive zeros
    if(type){
        for(unsigned i = 0; i < result.size(); i++){
            if(result[i] != '0' || result.size() == 1)
                break;
            if(result[i] == '0'){
                result.erase(i,i+1);
                i--;
            }
        }
    }
    if(result[0] == '0')
        sign_1 = '+';
}

// operator * ============================================================
string multiply(string str_1, string str_2) { 
    int size_1 = str_1.size(); 
    int size_2 = str_2.size(); 
    if(size_1 == 0 || size_2 == 0) 
        return "0"; 
    
    vector<int> tmp(size_1 + size_2); 
    int tmp_i = 0, tmp_j, remainder;    
    for(int i = size_1 - 1; i >= 0; i--){ 
        remainder = 0; 
        tmp_j = 0;  
        for(int j = size_2 - 1; j >= 0; j--){ 
            int sum = (str_1[i] - 48) * (str_2[j] - 48);
            sum += tmp[tmp_i + tmp_j] + remainder; 
            remainder = sum/10; 
            tmp[tmp_i + tmp_j] = sum % 10; 
            tmp_j++; 
        }  
        if(remainder > 0) 
            tmp[tmp_i + tmp_j] += remainder; 
        tmp_i++; 
    } 
  
    int i = tmp.size() - 1; 
    while(1){ 
        if(i < 0 || tmp[i] != 0)
            break;
        i--; 
    }
    if(i == -1) 
        return "0"; 
    string result = "";   
    while(i >= 0){
        result += tmp[i] + 48; 
        i--;
    }
    return result; 
}  

CBigInt operator * (const CBigInt & a, const CBigInt & b){
    string result = "";
    string str_1 = a.number;
    string str_2 = b.number;
    CBigInt ret;
    ret.sign = '+';
    char sign_1 = a.sign;
    char sign_2 = b.sign;
    if((sign_1 == '-' && sign_2 == '+') || (sign_1 != '+' && sign_2 != '-' )) 
        ret.sign = '-'; 
  
    ret.number = multiply(str_1, str_2);
    return ret;
}

CBigInt operator * (const CBigInt & a, const int b){
    string result = "";
    string str_1 = a.number;
    string str_2 = to_string(b);
    char sign_1 = a.sign;
    char sign_2;
    CBigInt ret;
    ret.sign = '+';
    if(str_2[0] == '-'){
        sign_2 = '-';
        str_2.erase(0,1);
    } else 
        sign_2 = '+';
    if((sign_1 == '-' && sign_2 == '+') || (sign_1 != '+' && sign_2 != '-' )) 
        ret.sign = '-'; 
  
    ret.number = multiply(str_1, str_2);
    return ret;
}
CBigInt operator * (const CBigInt & a, const char * b){
    string tmp = b;
    string str_2;
    char sign_2;
     if(!check_string(tmp, str_2, sign_2, 1))
        throw invalid_argument ("NaN");
    string result = "";
    string str_1 = a.number;
    char sign_1 = a.sign;
    CBigInt ret;
    ret.sign = '+';
    if((sign_1 == '-' && sign_2 == '+') || (sign_1 != '+' && sign_2 != '-' )) 
        ret.sign = '-'; 
  
    ret.number = multiply(str_1, str_2);
    return ret;
}
CBigInt operator * (const int a, const CBigInt & b){
    string result = "";
    string str_1 = to_string(a);
    string str_2 = b.number;
    char sign_1;
    char sign_2 = b.sign;
    if(str_1[0] == '-'){
        sign_1 = '-';
        str_1.erase(0,1);
    } else 
        sign_1 = '+';
    CBigInt ret;
    ret.sign = '+';
    if((sign_1 == '-' && sign_2 == '+') || (sign_1 != '+' && sign_2 != '-' )) 
        ret.sign = '-'; 
  
    ret.number = multiply(str_1, str_2);
    return ret;
}

CBigInt operator * (const char * a, const CBigInt & b){
    string tmp = a;
    string str_1;
    char sign_1;
     if(!check_string(tmp, str_1, sign_1, 1))
        throw invalid_argument ("NaN");
    string result = "";
    string str_2 = b.number;
    char sign_2 = b.sign;
    
    CBigInt ret;
    ret.sign = '+';
    if((sign_1 == '-' && sign_2 == '+') || (sign_1 != '+' && sign_2 != '-' )) 
        ret.sign = '-'; 
  
    ret.number = multiply(str_1, str_2);
    return ret;
}

// operator *= ============================================================
CBigInt operator *= (CBigInt & a, const CBigInt & b){
    a = a * b;
    return a;
}
CBigInt operator *= (CBigInt & a, const int b){
    a = a * b;
    return a;
}
CBigInt operator *= (CBigInt & a, const char * b){
    string tmp = b;
    string str_2;
    char sign_2;
     if(!check_string(tmp, str_2, sign_2, 1))
        throw invalid_argument ("NaN");
    CBigInt plus;
    plus.number = str_2;
    plus.sign = sign_2;
    a = a * plus;
    return a;
}

// operator << ===========================================================
ostream & operator << (ostream & out, const CBigInt & a){
    if(a.sign == '-' && a.number.size() > 1)
        out << a.sign;
    out << a.number;
    return out;
}

// operator >> ============================================================
bool operator >> (istream & in, CBigInt & a){
    string tmp;
    in >> tmp;
    char sign = '+';
    string fin;
    if(!check_string(tmp, fin, sign, 0))
        return false;
    a.number = fin;
    a.sign = sign;
    return true;
}
// operator == ============================================================
int find_equal(string str_1, string str_2, char sign_1, char sign_2){
    if(str_1.size() != str_2.size() || sign_1 != sign_2)
        return 0;
    for(unsigned i = 0; i < str_2.size(); i++)
        if(str_1[i] != str_2[i])
            return 0;
    return 1;
}
bool operator != (const CBigInt & a, const CBigInt & b){
    string str_1 = a.number;
    string str_2 = b.number;
    char sign_1 = a.sign;
    char sign_2 = b.sign;
    
    if(!find_equal(str_1, str_2, sign_1, sign_2))
        return true;
    return false;
}
bool operator != (const CBigInt & a, const int b){
    string str_1 = a.number;
    string str_2 = to_string(b);
    char sign_1 = a.sign;
    char sign_2 = '+';
    if(str_2[0] == '-'){
        sign_2 = '-';
        str_2.erase(0,1);
    }
    
    if(!find_equal(str_1, str_2, sign_1, sign_2))
        return true;
    return false;
}
bool operator != (const CBigInt & a, const char * b){
    string tmp = b;
    string str_2;
    char sign_2;
     if(!check_string(tmp, str_2, sign_2, 1))
        throw invalid_argument ("NaN");
    string str_1 = a.number;
    char sign_1 = a.sign;

    if(!find_equal(str_1, str_2, sign_1, sign_2))
        return true;
    return false;
}
bool operator != (const int a, const CBigInt & b){
    string str_1 = to_string(a);
    string str_2 = b.number;
    char sign_1 = '+';
    char sign_2 = b.sign;
    if(str_1[0] == '-'){
        sign_1 = '-';
        str_1.erase(0,1);
    }
    
    if(!find_equal(str_1, str_2, sign_1, sign_2))
        return true;
    return false;
}
bool operator != (const char * a, const CBigInt & b){
    string tmp = a;
    string str_1;
    char sign_1;
     if(!check_string(tmp, str_1, sign_1, 1))
        throw invalid_argument ("NaN");
    string str_2 = b.number;
    char sign_2 = b.sign;
    
    if(!find_equal(str_1, str_2, sign_1, sign_2))
        return true;
    return false;
}

// operator == ===========================================================
bool operator == (const CBigInt & a, const CBigInt & b){
    string str_1 = a.number;
    string str_2 = b.number;
    char sign_1 = a.sign;
    char sign_2 = b.sign;
    
    if(find_equal(str_1, str_2, sign_1, sign_2))
        return true;
    return false;
}
bool operator == (const CBigInt & a, const int b){
    string str_1 = a.number;
    string str_2 = to_string(b);
    char sign_1 = a.sign;
    char sign_2 = '+';
    if(str_2[0] == '-'){
        sign_2 = '-';
        str_2.erase(0,1);
    }
    
    if(find_equal(str_1, str_2, sign_1, sign_2))
        return true;
    return false;
}
bool operator == (const CBigInt & a, const char * b){
    string tmp = b;
    string str_2;
    char sign_2;
     if(!check_string(tmp, str_2, sign_2, 1))
        throw invalid_argument ("NaN");
    string str_1 = a.number;
    char sign_1 = a.sign;
    
    if(find_equal(str_1, str_2, sign_1, sign_2))
        return true;
    return false;
}
bool operator == (const int a, const CBigInt & b){
    string str_1 = to_string(a);
    string str_2 = b.number;
    char sign_1 = '+';
    char sign_2 = b.sign;
    if(str_1[0] == '-'){
        sign_1 = '-';
        str_1.erase(0,1);
    }
    
    if(find_equal(str_1, str_2, sign_1, sign_2))
        return true;
    return false;
}
bool operator == (const char * a, const CBigInt & b){
    string tmp = a;
    string str_1;
    char sign_1;
     if(!check_string(tmp, str_1, sign_1, 1))
        throw invalid_argument ("NaN");
    string str_2 = b.number;
    char sign_2 = b.sign;

    if(find_equal(str_1, str_2, sign_1, sign_2))
        return true;
    return false;
}

// operator <= ============================================================
int find_higher_equal(string str_1, string str_2, char sign_1, char sign_2){
    if(sign_1 == '+' && sign_2 == '-')
        return 1;
    else if(sign_1 == '-' && sign_2 == '+')
        return 0;
    // + and +
    int check = 1;
    if(sign_1 == '+'){
        if(str_1.size() > str_2.size())
            return 1;
        else if(str_1.size() < str_2.size())
            return 0;
        for(unsigned i = 0; i < str_1.size(); i++){
            if(str_1[i] < str_2[i])
                return 0;
            else if(str_1[i] > str_2[i])
                return 1;
        }
    } else { // - and -
        if(str_1.size() < str_2.size())
            return 1;
        else if(str_1.size() > str_2.size())
            return 0;
        for(unsigned i = 0; i < str_1.size(); i++){
            if(str_1[i] == str_2[i] && check)
                check = 1;
            else 
                check = 0;
            if(str_1[i] < str_2[i])
                return 1;
            else if(str_1[i] > str_2[i])
                return 0;
        }
        if(check)
            return 3;
        return 0;
    }
    return 2;
}
bool operator <= (const CBigInt & a, const CBigInt & b){
    string str_1 = a.number;
    string str_2 = b.number;
    char sign_1 = a.sign;
    char sign_2 = b.sign;
    int res = find_higher_equal(str_1, str_2, sign_1, sign_2);
    if(!res || res == 2 || res == 3)
        return true;
    return false;
}
bool operator <= (const CBigInt & a, const int b){
    string str_1 = a.number;
    string str_2 = to_string(b);
    char sign_1 = a.sign;
    char sign_2 = '+';
    if(str_2[0] == '-'){
        sign_2 = '-';
        str_2.erase(0,1);
    }
    int res = find_higher_equal(str_1, str_2, sign_1, sign_2);
    if(!res || res == 2 || res == 3)
        return true;
    return false;
}
bool operator <= (const CBigInt & a, const char * b){
    string tmp = b;
    string str_2;
    char sign_2;
     if(!check_string(tmp, str_2, sign_2, 1))
        throw invalid_argument ("NaN");
    string str_1 = a.number;
    char sign_1 = a.sign;
    int res = find_higher_equal(str_1, str_2, sign_1, sign_2);
    if(!res || res == 2 || res == 3)
        return true;
    return false;
}
bool operator <= (const int a, const CBigInt & b){
    string str_1 = to_string(a);
    string str_2 = b.number;
    char sign_1 = '+';
    char sign_2 = b.sign;
    if(str_1[0] == '-'){
        sign_1 = '-';
        str_1.erase(0,1);
    }
    int res = find_higher_equal(str_1, str_2, sign_1, sign_2);
    if(!res || res == 2 || res == 3)
        return true;
    return false;
}
bool operator <= (const char * a, const CBigInt & b){
    string tmp = a;
    string str_1;
    char sign_1;
     if(!check_string(tmp, str_1, sign_1, 1))
        throw invalid_argument ("NaN");
    string str_2 = b.number;
    char sign_2 = b.sign;
    int res = find_higher_equal(str_1, str_2, sign_1, sign_2);
    if(!res || res == 2 || res == 3)
        return true;
    return false;
}

// operator >= =============================================================
bool operator >= (const CBigInt & a, const CBigInt & b){
    string str_1 = a.number;
    string str_2 = b.number;
    char sign_1 = a.sign;
    char sign_2 = b.sign;
    int res = find_higher_equal(str_1, str_2, sign_1, sign_2);
    if(res)
        return true;
    return false;
}
bool operator >= (const CBigInt & a, const int b){
    string str_1 = a.number;
    string str_2 = to_string(b);
    char sign_1 = a.sign;
    char sign_2 = '+';
    if(str_2[0] == '-'){
        sign_2 = '-';
        str_2.erase(0,1);
    }
    int res = find_higher_equal(str_1, str_2, sign_1, sign_2);
    if(res)
        return true;
    return false;
}
bool operator >= (const CBigInt & a, const char * b){
    string tmp = b;
    string str_2;
    char sign_2;
     if(!check_string(tmp, str_2, sign_2, 1))
        throw invalid_argument ("NaN");
    string str_1 = a.number;
    char sign_1 = a.sign;
    int res = find_higher_equal(str_1, str_2, sign_1, sign_2);
    if(res)
        return true;
    return false;
}
bool operator >= (const int a, const CBigInt & b){
    string str_1 = to_string(a);
    string str_2 = b.number;
    char sign_1 = '+';
    char sign_2 = b.sign;
    if(str_1[0] == '-'){
        sign_1 = '-';
        str_1.erase(0,1);
    }
    int res = find_higher_equal(str_1, str_2, sign_1, sign_2);
    if(res)
        return true;
    return false;
}
bool operator >= (const char * a, const CBigInt & b){
    string tmp = a;
    string str_1;
    char sign_1;
     if(!check_string(tmp, str_1, sign_1, 1))
        throw invalid_argument ("NaN");
    string str_2 = b.number;
    char sign_2 = b.sign;

    int res = find_higher_equal(str_1, str_2, sign_1, sign_2);
    if(res)
        return true;
    return false;
}

// operator < =============================================================
int find_higher(string str_1, string str_2, char sign_1, char sign_2){
    if(sign_1 == '+' && sign_2 == '-')
        return 1;
    else if(sign_1 == '-' && sign_2 == '+')
        return 0;
    // + and +
    if(sign_1 == '+'){
        if(str_1.size() > str_2.size())
            return 1;
        else if(str_1.size() < str_2.size())
            return 0;
        for(unsigned i = 0; i < str_1.size(); i++){
            if(str_1[i] < str_2[i])
                return 0;
            else if(str_1[i] > str_2[i])
                return 1;
        }
    } else { // - and -
        if(str_1.size() < str_2.size())
            return 1;
        else if(str_1.size() > str_2.size())
            return 0;
        for(unsigned i = 0; i < str_1.size(); i++){
            if(str_1[i] < str_2[i])
                return 1;
            else if(str_1[i] > str_2[i])
                return 0;
        }
        return 0;
    }
    if(str_1[str_1.size()-1] == str_2[str_2.size()-1])
        return 2;
    return 1;

}
bool operator < (const CBigInt & a, const CBigInt & b){
    string str_1 = a.number;
    string str_2 = b.number;
    char sign_1 = a.sign;
    char sign_2 = b.sign;
    
    if(!find_higher(str_1, str_2, sign_1, sign_2))
        return true;
    return false;
}
bool operator < (const CBigInt & a, const int b){
    string str_1 = a.number;
    string str_2 = to_string(b);
    char sign_1 = a.sign;
    char sign_2 = '+';
    if(str_2[0] == '-'){
        sign_2 = '-';
        str_2.erase(0,1);
    }
    
    if(!find_higher(str_1, str_2, sign_1, sign_2))
        return true;
    return false;
}
bool operator < (const CBigInt & a, const char * b){
    string tmp = b;
    string str_2;
    char sign_2;
     if(!check_string(tmp, str_2, sign_2, 1))
        throw invalid_argument ("NaN");
    string str_1 = a.number;
    char sign_1 = a.sign;
    
    if(!find_higher(str_1, str_2, sign_1, sign_2))
        return true;
    return false;
}
bool operator < (const int a, const CBigInt & b){
    string str_1 = to_string(a);
    string str_2 = b.number;
    char sign_1 = '+';
    char sign_2 = b.sign;
    if(str_1[0] == '-'){
        sign_1 = '-';
        str_1.erase(0,1);
    }
    
    if(!find_higher(str_1, str_2, sign_1, sign_2))
        return true;
    return false;
}
bool operator < (const char * a, const CBigInt & b){
    string tmp = a;
    string str_1;
    char sign_1;
     if(!check_string(tmp, str_1, sign_1, 1))
        throw invalid_argument ("NaN");
    string str_2 = b.number;
    char sign_2 = b.sign;
    
    if(!find_higher(str_1, str_2, sign_1, sign_2))
        return true;
    return false;
}

// operator > =============================================================
bool operator > (const CBigInt & a, const CBigInt & b){
    string str_1 = a.number;
    string str_2 = b.number;
    char sign_1 = a.sign;
    char sign_2 = b.sign;
    int res = find_higher(str_1, str_2, sign_1, sign_2);
    if(res == 2)
        return false;
    if(res)
        return true;
    return false;
}
bool operator > (const CBigInt & a, const int b){
    string str_1 = a.number;
    string str_2 = to_string(b);
    char sign_1 = a.sign;
    char sign_2 = '+';
    if(str_2[0] == '-'){
        sign_2 = '-';
        str_2.erase(0,1);
    }
    int res = find_higher(str_1, str_2, sign_1, sign_2);
    if(res == 2)
        return false;
    if(res)
        return true;
    return false;
}
bool operator > (const CBigInt & a, const char * b){
    string tmp = b;
    string str_2;
    char sign_2;
     if(!check_string(tmp, str_2, sign_2, 1))
        throw invalid_argument ("NaN");

    string str_1 = a.number;
    char sign_1 = a.sign;

    int res = find_higher(str_1, str_2, sign_1, sign_2);
    if(res == 2)
        return false;
    if(res)
        return true;
    return false;
}
bool operator > (const int a, const CBigInt & b){
    string str_1 = to_string(a);
    string str_2 = b.number;
    char sign_1 = '+';
    char sign_2 = b.sign;
    if(str_1[0] == '-'){
        sign_1 = '-';
        str_1.erase(0,1);
    }
    int res = find_higher(str_1, str_2, sign_1, sign_2);
    if(res == 2)
        return false;
    if(res)
        return true;
    return false;
}
bool operator > (const char * a, const CBigInt & b){
    string tmp = a;
    string str_1;
    char sign_1;
     if(!check_string(tmp, str_1, sign_1, 1))
        throw invalid_argument ("NaN");

    string str_2 = b.number;
    char sign_2 = b.sign;

    int res = find_higher(str_1, str_2, sign_1, sign_2);
    if(res == 2)
        return false;
    if(res)
        return true;
    return false;
}

// operator += ============================================================
CBigInt operator += (CBigInt & a, const CBigInt & b){
    a = a + b;
    return a;
}
CBigInt operator += (CBigInt & a, const int b){
    a = a + b;
    return a;
}
CBigInt operator += (CBigInt & a, const char * b){
    string tmp = b;
    string str_2;
    char sign_2;
     if(!check_string(tmp, str_2, sign_2, 1))
        throw invalid_argument ("NaN");
    CBigInt plus;
    plus.number = str_2;
    plus.sign = sign_2;
    a = a + plus;
    return a;
}

// operator + =============================================================
CBigInt operator + (const CBigInt & a, const CBigInt & b){
    string result = "";
    string str_1 = a.number;
    string str_2 = b.number;
    char sign_1 = a.sign;
    char sign_2 = b.sign;
    int size_1 = str_1.size();
    int size_2 = str_2.size();
    add(result, str_1, str_2, sign_1, sign_2, size_1, size_2, 1);
    // store data
    CBigInt ret;
    ret.sign = sign_1;
    ret.number = result;
    return ret;
}
CBigInt operator + (const CBigInt & a, const int b){
    string result = "";
    string str_1 = a.number;
    string str_2 = to_string(b);
    char sign_1 = a.sign;
    char sign_2;
    if(str_2[0] == '-'){
        sign_2 = '-';
        str_2.erase(0,1);
    } else 
        sign_2 = '+';
    int size_1 = str_1.size();
    int size_2 = str_2.size();
    add(result, str_1, str_2, sign_1, sign_2, size_1, size_2, 1);
    // store data
    CBigInt ret;
    ret.sign = sign_1;
    ret.number = result;
    return ret;
}
CBigInt operator + (const CBigInt & a, const char * b){
    string tmp = b;
    string str_2;
    char sign_2;
     if(!check_string(tmp, str_2, sign_2, 1))
        throw invalid_argument ("NaN");

    string result = "";
    string str_1 = a.number;
    char sign_1 = a.sign;

    int size_1 = str_1.size();
    int size_2 = str_2.size();
    add(result, str_1, str_2, sign_1, sign_2, size_1, size_2, 1);
    // store data
    CBigInt ret;
    ret.sign = sign_1;
    ret.number = result;
    return ret;
}
CBigInt operator + (const int a, const CBigInt & b){
    string result = "";
    string str_1 = to_string(a);
    string str_2 = b.number;
    char sign_1;
    char sign_2 = b.sign;
    if(str_1[0] == '-'){
        sign_1 = '-';
        str_1.erase(0,1);
    } else 
        sign_1 = '+';
    int size_1 = str_1.size();
    int size_2 = str_2.size();
    add(result, str_1, str_2, sign_1, sign_2, size_1, size_2, 1);
    // store data
    CBigInt ret;
    ret.sign = sign_1;
    ret.number = result;
    return ret;
}

CBigInt operator + (const char * a, const CBigInt & b){
    string tmp = a;
    string str_1;
    char sign_1;
     if(!check_string(tmp, str_1, sign_1, 1))
        throw invalid_argument ("NaN");

    string result = "";
    string str_2 = b.number;
    char sign_2 = b.sign;
    
    int size_1 = str_1.size();
    int size_2 = str_2.size();
    add(result, str_1, str_2, sign_1, sign_2, size_1, size_2, 1);
    // store data
    CBigInt ret;
    ret.sign = sign_1;
    ret.number = result;
    return ret;
}

// constructor with integer ===============================================
CBigInt::CBigInt(const int number){
    string tmp = to_string(number);
    unsigned i = 0;
    if(tmp[0] == '-'){
        this->sign = '-';
        i++;
    } else {
        this->sign = '+';
    }
    for(; i < tmp.size(); i++)
        this->number.push_back(tmp[i]);
}

// constructor with string, if it is not a number, throw exception ========
CBigInt::CBigInt(const char * number){
    string tmp = number;
    string fin = "";
    char sign = '+';
    if(!check_string(tmp, fin, sign, 1))
        throw invalid_argument ("NaN");
    this->number = fin;
    this->sign = sign;
}

// print number ===========================================================
void CBigInt::print(void) const {
    cout << this->sign;
    for(char i : this->number)
        cout << i;
    cout << endl;
}

#ifndef __PROGTEST__

static bool equal ( const CBigInt & x, const char * val ){
    ostringstream oss;
    oss << x;
    return oss . str () == val;
}

int main ( void ){
    CBigInt a, b;
    istringstream is;
    a = 10;
    a += 20;
    assert ( equal ( a, "30" ) );
    a *= 5;
    assert ( equal ( a, "150" ) );
    b = a + 3;
    assert ( equal ( b, "153" ) );
    b = a * 7;
    assert ( equal ( b, "1050" ) );
    assert ( equal ( a, "150" ) );

    a = 10;
    a += -20;
    assert ( equal ( a, "-10" ) );
    a *= 5;
    assert ( equal ( a, "-50" ) );
    b = a + 73;
    assert ( equal ( b, "23" ) );
    b = a * -7;
    assert ( equal ( b, "350" ) );
    assert ( equal ( a, "-50" ) );

    a = "12345678901234567890";
    a += "-99999999999999999999";
    assert ( equal ( a, "-87654321098765432109" ) );
    a *= "54321987654321987654";
    assert ( equal ( a, "-4761556948575111126880627366067073182286" ) );
    
    
    a *= 0;
    assert ( equal ( a, "0" ) );
    a = 10;
    b = a + "400";
    assert ( equal ( b, "410" ) );
    b = a * "15";
    assert ( equal ( b, "150" ) );
    assert ( equal ( a, "10" ) );

    is . clear ();
    is . str ( " 1234" );
    assert ( is >> b );
    assert ( equal ( b, "1234" ) );
    is . clear ();
    is . str ( " 12 34" );
    assert ( is >> b );
    assert ( equal ( b, "12" ) );

    is . clear ();
    is . str ( "999z" );
    assert ( is >> b );
    assert ( equal ( b, "999" ) );
    is . clear ();
    is . str ( "abcd" );
    assert ( ! ( is >> b ) );
    is . clear ();
    is . str ( "- 758" );
    assert ( ! ( is >> b ) );
    a = 42;
    try {
        a = "-xyz";
        assert ( "missing an exception" == NULL );
    } catch ( const invalid_argument & e ){
        assert ( equal ( a, "42" ) );
    }

    a = "73786976294838206464";
    assert ( a < "1361129467683753853853498429727072845824" );
    assert ( a <= "1361129467683753853853498429727072845824" );
    assert ( ! ( a > "1361129467683753853853498429727072845824" ) );
    assert ( ! ( a >= "1361129467683753853853498429727072845824" ) );
    assert ( ! ( a == "1361129467683753853853498429727072845824" ) );
    assert ( a != "1361129467683753853853498429727072845824" );
    assert ( ! ( a < "73786976294838206464" ) );
    assert ( a <= "73786976294838206464" );
    assert ( ! ( a > "73786976294838206464" ) );
    assert ( a >= "73786976294838206464" );
    assert ( a == "73786976294838206464" );
    assert ( ! ( a != "73786976294838206464" ) );
    assert ( a < "73786976294838206465" );
    assert ( a <= "73786976294838206465" );
    assert ( ! ( a > "73786976294838206465" ) );
    assert ( ! ( a >= "73786976294838206465" ) );
    assert ( ! ( a == "73786976294838206465" ) );
    assert ( a != "73786976294838206465" );
    a = "2147483648";
    assert ( ! ( a < -2147483648 ) );
    assert ( ! ( a <= -2147483648 ) );
    assert ( a > -2147483648 );
    assert ( a >= -2147483648 );
    assert ( ! ( a == -2147483648 ) );
    assert ( a != -2147483648 );

    return 0;
}
#endif /* __PROGTEST__ */

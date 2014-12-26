/*
 *  File: stringCommon.h
 *  Author: jiazhiwei
 *
 *  Created on 2014-8-8
 */

#ifndef _STRING_COMMON_H
#define _STRING_COMMON_H

#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <functional>
#include <sstream>

using namespace std;

//trim space from left of str
inline string &ltrim(string &str) {
    str.erase(str.begin(), 
            find_if(str.begin(), str.end(), 
                not1(ptr_fun<int, int>(isspace))));
    return str;
}

//trim space from right of str
inline string &rtrim(string &str) {
    str.erase(find_if(str.rbegin(), str.rend(), 
                not1(ptr_fun<int, int>(isspace))).base(), 
                    str.end());
    return str;
}

//trim space from left and right of str
inline string &trim(string &str) {
    rtrim(ltrim(str));
    return str;
}

//split str by delim, return new vector<string>
inline vector<string> &split(string &str, vector<string> &elems, char delim) {
    stringstream ss(str);

    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }

    return elems;
}

#endif

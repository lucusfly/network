/*
 *  File:   ini.h
 *  Author: jiazhiwei
 * 
 *  Created on 2014年8月8日
 *	Modify to add template on 2014年8月29 
 * */


#ifndef _INI_H
#define _INI_H

#include <string>
#include <map>
#include <vector>
#include <set>
#include "stringCommon.h"

#define ANNOTATION '#'
#define VECTORSPLIT ','
#define SECTIONSTART '['
#define SECTIONEND ']'

using namespace std;

template<typename Key>
inline Key transform(const string &str) {
    return (Key)atoll(str.c_str());
}

template<>
inline double transform<double>(const string &str) {
    return atof(str.c_str());
}

template<>
inline float transform<float>(const string &str) {
    return (float)atof(str.c_str());
}

template<>
inline string transform<string>(const string &str) {
    return str;
}

class Ini{
public:
    Ini();
    //加载ini file
    bool loadFile(const string &filename);
    //重新加载配置文件
    bool reloadFile();
    //加载ini stream
    bool loadStream(istream &in);
    //获得配置信息
    string getString(const string &section, const string &key, string ignore="");
    //以vector结构解析配置信息,默认使用VECTORSPLIT(,)分割vector
    template<typename Key>
    vector<Key> &getContainer(const string &section, const string &key, vector<Key> &elems);
    //以hash_set的结构解析配置信息,默认使用VECTORSPLIT(,)分割vector
    template<typename Key>
    set<Key> &getContainer(const string &section, const string &key, set<Key> &elems);
    //以数值类型解析配置信息,包括bool,int,long,long long,double,float
    template<typename Key>
    Key getValue(const string &section, const string &key, Key ignore = 0);
    //打印所有配置信息
    friend ostream &operator<<(ostream &os, const Ini &ini);
private:
    map<string, map<string, string> > m_tree;
    string filename;
    bool error;//取得数据时是否出错
};


template<typename Key>
vector<Key> &Ini::getContainer(const string &section, const string &key, vector<Key> &elems) {
    string value = getString(section, key);
    
    vector<string> splitRes;
    if (!value.empty() && !error)
        split(value, splitRes, VECTORSPLIT);
    
    elems.clear();
    for (size_t i = 0; i < splitRes.size(); ++i) {
        elems.push_back(transform<Key>(splitRes[i]));
    }
    return elems;
}

template<typename Key>
set<Key> &Ini::getContainer(const string &section, const string &key, set<Key> &elems) {
    string value = getString(section, key);
    
    vector<string> splitRes;
    if (!value.empty() && !error)
        split(value, splitRes, VECTORSPLIT);
    
    elems.clear();
    for (size_t i = 0; i < splitRes.size(); ++i) {
        elems.insert(transform<Key>(splitRes[i]));
    }
    return elems;
}

template<typename Key>
Key Ini::getValue(const string &section, const string &key, Key ignore) {
    string value = getString(section, key);

    if (error || value.empty()) 
        return ignore;
    else
        return transform<Key>(value);
}

#endif 

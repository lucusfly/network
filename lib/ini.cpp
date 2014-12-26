#include "ini.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

Ini::Ini():error(false) {
}

bool Ini::loadFile(const string &filename) {
   ifstream ifs(filename.c_str()); 
   if (!ifs) {
       //log
       return false;
   }

   this->filename = filename;

   return loadStream(ifs);
}

bool Ini::reloadFile() {
    return loadFile(this->filename);
}

bool Ini::loadStream(istream &in) {
    string section;
    string line;
    while (getline(in, line, '\n')) {
        //删除注释
        size_t pos = line.find_first_of(ANNOTATION);
        if (pos != string::npos) {
            line = line.substr(0, pos);
        }

        if (trim(line).empty()) 
            continue;

        if (line[0] == SECTIONSTART) {
            section = line.substr(1, line.find_last_of(SECTIONEND) - 1);//if there is no ], will whole line
            trim(section);
            continue;
        } else {
            //如果没有section,不处理
            if (section.empty()) {
                continue;
            }

            vector<string> keyvalue;
            split(line, keyvalue, '=');

            //key value有问题，扔掉继续
            if (keyvalue.size() != 2) {
                continue;
            }

            string key = trim(keyvalue[0]);
            string value = trim(keyvalue[1]);

            m_tree[section][key] = value;
        }
    }

    return true;
}

string Ini::getString(const string &section, const string &key, string ignore) {
    error = false;
    map<string, map<string, string> >::iterator result = m_tree.find(section);

    if (result == m_tree.end()) {
        error = true;
        return ignore;
    }

    map<string, string> keyvalue = result->second;
    map<string, string>::iterator value = keyvalue.find(key);
    if (value == keyvalue.end()) {
        error = true;
        return ignore;
    }

    //返回值为空，标为error
    if (trim(value->second).empty()) 
        error = true;

    return value->second;
    //当尝试取得不存在的key时，会有些许内存冗余，可用上面代码代替
    //return m_tree[section][key];
}

ostream &operator<<(ostream &os, const Ini &ini) {
    for (map<string, map<string, string> >::const_iterator it = ini.m_tree.begin(); it != ini.m_tree.end(); ++it) {
        map<string, string> temp = it->second;

        os << it->first << endl;

        for (map<string, string>::iterator i = temp.begin(); i != temp.end(); ++i) {
            os << "\t" << i->first << "  " << i->second << endl;
        }
    }

    return os;
}

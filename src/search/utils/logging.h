/* -*- mode : c++ -*- */
#ifndef UTILS_LOGGING_H
#define UTILS_LOGGING_H

#include "system.h"
#include "timer.h"

#include <ostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <list>
#include <forward_list>
#include <utility>


namespace Utils {
/*
  Simple logger that prepends time and peak memory info to messages.
  Logs are written to stdout.

  Usage:
        g_log << "States: " << num_states << endl;
*/
struct Log {
    template<typename T>
    std::ostream &operator<<(const T &elem) {
        return std::cout << "[t=" << g_timer << ", "
                         << get_peak_memory_in_kb() << " KB] " << elem;
    }
};

class TraceBlock {
    std::string block_name;
public:
    explicit TraceBlock(const std::string &block_name);
    ~TraceBlock();
};

extern void trace(const std::string &msg = "");
}

namespace std {
    template<class T, class C >
    ostream &myprint(ostream &stream, const C &c) {
        stream << "(";
        bool flag=false;
        for (const T &e : c){
            if (!flag){
                flag=true;
            }else{
                stream << " ";
            }
            stream << e;
        }
        stream << ")";
        return stream;
    }
    template<class T>
    ostream &operator<<(ostream &stream, const vector<T> &c) {
        return myprint <T, const vector<T>> (stream, c);
    }
    template<class T>
    ostream &operator<<(ostream &stream, const list<T> &c) {
        return myprint <T, const list<T>> (stream, c);
    }
    template<class T>
    ostream &operator<<(ostream &stream, const forward_list<T> &c) {
        return myprint <T, const forward_list<T>> (stream, c);
    }
    template<class T>
    ostream &operator<<(ostream &stream, const set<T> &c) {
        return myprint <T, const set<T>> (stream, c);
    }
    template<class T>
    ostream &operator<<(ostream &stream, const unordered_set<T> &c) {
        return myprint <T, const unordered_set<T>> (stream, c);
    }
    template<class T1,class T2>
    ostream &operator<<(ostream &stream, const map<T1,T2> &c) {
        stream << "(";
        bool flag=false;
        for (const auto &e : c){
            if (!flag){
                flag=true;
            }else{
                stream << " ";
            }
            // stream << e ;
            stream << "(" << e.first << " " << e.second << ")";
        }
        stream << ")";
        return stream;
        // return myprint <typename map<T1,T2>::iterator, const map<T1,T2>> (stream, c);
    }
    template<class T1,class T2>
    ostream &operator<<(ostream &stream, const unordered_map<T1,T2> &c) {
        stream << "(";
        bool flag=false;
        for (const auto &e : c){
            if (!flag){
                flag=true;
            }else{
                stream << " ";
            }
            // stream << e ;
            stream << "(" << e.first << " " << e.second << ")";
        }
        stream << ")";
        return stream;
        // return myprint <typename unordered_map<T1,T2>::iterator, const unordered_map<T1,T2>> (stream, c);
    }
    
    template<class T1, class T2>
    ostream &operator<<(ostream &stream, const pair<T1,T2> &vec) {
        stream << "(" << vec.first << " " << vec.second << ")";
        return stream;
    }
}

#endif

#pragma once
#include <iostream>
#include <cassert>

template<typename V>
std::ostream &operator<<(std::ostream &o, const std::vector<V> &v){
    o << "#(";
    bool first = true;
    for (const V &elem : v){
        if (first){
            first = false;
        }else{
            o << " ";
        }
        o << elem ;
    }
    o << ")";
    return o;
}

template<typename T1,typename T2>
std::ostream &operator<<(std::ostream &o, const std::pair<T1,T2> &v){
    o << "(" << v.first << " . " << v.second << ")";
    return o;
}


#define print(something) std::cout << something << std::endl;

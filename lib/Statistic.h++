#pragma once

#include <iostream>

using namespace std;

struct Statistic{
    long long win = 0;
    long long lose = 0;
    long long draw = 0;

    void print(){
        double total = win + lose + draw;
        cout << "total = " << total << endl;
        cout << win / total * 100 << "%" << endl; 
        cout << lose / total * 100 << "%" << endl; 
        cout << draw / total * 100 << "%" << endl; 
    }
};
#pragma once

#include "../stdafx.h++"

using namespace std;

struct Statistic{
    long long win = 0;
    long long lose = 0;
    long long draw = 0;

    friend std::istream& operator>>(std::istream& is, Statistic& p) {
        is >> p.win >> p.lose >> p.draw;
        return is;
    }

    friend std::ostream& operator<<(std::ostream& is, Statistic& p) {
        is << p.win << " " << p.lose << " " << p.draw << endl;
        return is;
    }

        Statistic& operator+=(const Statistic& other) {
        win += other.win;
        lose += other.lose;
        draw += other.draw;
        return *this;
    }

    void print(){
        double total = win + lose + draw;
        cout << "total = " << total << endl;
        cout << win / total * 100 << "%" << endl; 
        cout << lose / total * 100 << "%" << endl; 
        cout << draw / total * 100 << "%" << endl; 
    }
};
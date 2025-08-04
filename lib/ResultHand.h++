#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "Hand.h++"
#include "Combs.h++"

struct Resulthand{
    std::vector<Hand> high;
    Hand kiker;
    Combs comb;
    int count;

    void print(){
        std::vector<std::string> combs{
                "старшая карта",
                "пара",
                "две пары",
                "сет",
                "стрит",
                "флеш",
                "фуллхаус",
                "каре",
                "стрит флеш"
        };
        std::cout << "Комбинация: " << combs[comb] << std::endl; 
        if(high.size() > 0){
            for(int i = 0; i < high.size(); i++){
                std::cout << "Карта[" << i << "] ";
                high[i].print();
            }
        }
        if(comb <= 3 || comb == 7){
            std::cout << "Кикер:";
            kiker.print();
            std::cout << std::endl;
        }
        std::cout << "Количество:" << count << std::endl;
    }
};
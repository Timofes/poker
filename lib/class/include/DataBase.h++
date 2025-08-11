#pragma once

#include "../../stdafx.h++"

class DataBase{
    private:
        vector<Statistic> data;
    
        Error save(string& file);
        Error load(string& file);
    public:
        DataBase(string& file);
        void createReserve(string& file);
        void setCart(Statistic& stat, Hand left, Hand right);
        void addCart(Statistic& stat, Hand left, Hand right);
        Statistic getCart(Hand left, Hand right);
        vector<Statistic> getData();

        void print(int max);
};
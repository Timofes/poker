#pragma once

#include "../../stdafx.h++"

Error DataBase::load(string& file){
    std::ifstream loadfile(file);
    
    if(!loadfile.is_open()){
       return ERROR_OPEN_FILE;
    }

    /*cout << data.size() * 3 << " " << std::filesystem::file_size(file) << endl;
    if(std::filesystem::file_size(file) != data.capacity() * 3){
        return ERROR_SIZE_FILE;
    }*/


    for(size_t i = 0; !loadfile.eof(); i++){
        loadfile >> data[i];
    }
    data[0].print();

    loadfile.close();
    return SUCCES;
}

Error DataBase::save(string& file){
    ofstream savefile(file);
    
    if(!savefile.is_open()){
       return ERROR_OPEN_FILE;
    }

    for(size_t i = 0; i < data.size(); i++){
        savefile << data[i];
    }
    

    savefile.close();

    return SUCCES;
}

vector<Statistic> DataBase::getData(){
    return data;
}

DataBase::DataBase(string& file): data((COUNT_ALL_CART) * (COUNT_ALL_CART), Statistic()){
    auto code = load(file); 
    if(code != SUCCES){
        cout << "Error " << code << endl;
    }
}

void DataBase::createReserve(string& file){
    auto code = save(file); 
    if(code != SUCCES){
        cout << "Error " << code << endl;
    }
}

void DataBase::setCart(Statistic& stat, Hand left, Hand right){
    /*auto maxC = max(left.cart, right.cart) - 2;
    auto minC = min(left.cart, right.cart) - 2;

    auto maxS = max(left.suit, right.suit) - 1;
    auto minS = min(left.suit, right.suit) - 1;*/

    data[COUNT_SUIT * (left.cart - 2) + COUNT_ALL_CART * COUNT_SUIT *
        (right.cart - 2) + (left.suit - 1) * COUNT_ALL_CART + (right.suit - 1)] = stat;
}

void DataBase::addCart(Statistic& stat, Hand left, Hand right){
    data[COUNT_SUIT * (left.cart - 2) + COUNT_ALL_CART * COUNT_SUIT *
        (right.cart - 2) + (left.suit - 1) * COUNT_ALL_CART + (right.suit - 1)] += stat;
}

Statistic DataBase::getCart(Hand left, Hand right){
    return data[COUNT_SUIT * (left.cart - 2) + COUNT_ALL_CART * COUNT_SUIT *
        (right.cart - 2) + (left.suit - 1) * COUNT_ALL_CART + (right.suit - 1)];
}

std::string shortenNumber(long num) {
    if (num == 0) return "  0"; // Ноль
    
    const string suffixes = " KMBT"; // Тысячи (K), Миллионы (M), Миллиарды (B), Триллионы (T)
    int suffixIndex = 0;
    
    // Находим подходящий суффикс (K, M, B, T)
    while (abs(num) >= 1000 && suffixIndex < 4) {
        num /= 1000;
        suffixIndex++;
    }
    
    // Если число стало двузначным (например, 12K), добавляем пробел
    if (abs(num) < 100) {
        return to_string(num) + suffixes[suffixIndex] + " ";
    }
    
    // Если трёхзначное (например, 123K), просто добавляем суффикс
    return to_string(num) + suffixes[suffixIndex];
}

void DataBase::print(int max){
    int count = max * max * COUNT_SUIT * COUNT_SUIT;

    for(int i = 0; i < data.size(); i++){
        if(i % COUNT_ALL_CART < max * COUNT_SUIT && i / COUNT_ALL_CART < max * COUNT_SUIT){
            if(i % COUNT_ALL_CART == 0){
                cout << endl;
            }
            cout << fixed << setw(4) << shortenNumber(data[i].win) << " " 
                << setw(4)  << shortenNumber(data[i].draw) << " " 
                << setw(4) << shortenNumber(data[i].lose) << "  ";
        }
    }

}
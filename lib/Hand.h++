#pragma once

#include "Cart.h++"
#include "Suit.h++"

struct Hand{
    Cart cart;
    Suit suit;

    bool operator<(const Hand& other) const {
        return (cart != other.cart) ? (cart < other.cart) : (suit < other.suit);
    }

    bool operator!=(const Hand& other) const {
        return (cart != other.cart && suit != other.suit);
    }

void print(){
    switch (cart)
    {
    case Cart::two:     std::cout << "2"; break;
    case Cart::three:   std::cout << "3"; break;
    case Cart::four:    std::cout << "4"; break;
    case Cart::five:    std::cout << "5"; break;
    case Cart::six:     std::cout << "6"; break;
    case Cart::seven:   std::cout << "7"; break;
    case Cart::eight:   std::cout << "8"; break;
    case Cart::nine:    std::cout << "9"; break;
    case Cart::ten:     std::cout << "T"; break;
    case Cart::J:       std::cout << "J"; break;
    case Cart::Q:       std::cout << "Q"; break;
    case Cart::K:       std::cout << "K"; break;
    case Cart::A:       std::cout << "A"; break;
    }

    switch (suit)
    {
    case Suit::clubs:   std::cout << "♣"; break;
    case Suit::diamonds:std::cout << "♦"; break;
    case Suit::hearts:  std::cout << "♥"; break;
    case Suit::spades:  std::cout << "♠"; break;

    }
    std::cout << std::endl;
}
};
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

#include "lib/Hand.h++"
#include "lib/Combs.h++"
#include "lib/ResultHand.h++"
#include "lib/Statistic.h++"

using namespace std;

Suit moreSuit(vector<Hand>& cart){
    vector<int> count(4, 0);
    for(size_t i = 0; i < cart.size(); i++){
        switch (cart[i].suit)
        {
        case clubs :    count[clubs - 1]++; break;
        case diamonds : count[diamonds - 1]++; break;
        case hearts :   count[hearts - 1]++; break;
        case spades :   count[spades - 1]++; break;
        }
    }

    int pt = 0;
    int maxCt = 0;
    for(size_t i = 0; i < count.size(); i++){
        if(count[maxCt] < count[i]){
            maxCt = count[i];
            pt = i;
        }
    }
    return Suit{pt + 1};
}

bool sortCart(Hand& A, Hand& B){
    return A.cart > B.cart;
}

bool sortSuit(Hand& A, Hand& B){
    return A.suit == B.suit ? A.cart > B.cart : A.suit > B.suit;
}

Resulthand checkStraight(vector<Hand> cart){
    auto iter = cart.begin();
    while(iter + 1 != cart.end()){
        Hand first = *iter;
        Hand next = *(iter + 1);
        if(first.cart == next.cart){
            cart.erase(iter);
        }else{
            iter++;
        }
        
    }

    if(cart.size() < 5){
        return Resulthand{vector<Hand>{}, Hand{}, Combs{0}, 0};
    }

    for(size_t i = 0; i <= cart.size() - 5; i++){
        int j = i + 1;
        for(j; j < i + 5; j++){
            if(cart[j].cart != cart[j - 1].cart - 1){
                break;
            }
        }   
        if(j == i + 5){
            return Resulthand{vector<Hand>{cart[i]}, Hand{}, Combs::straight, 1};
        }
    }

    for(size_t i = 0; i <= cart.size() - 5; i++){
        if (cart[i].cart == 14) {  
            if(
                (cart[i + 4].cart == 5) &&
                (cart[i + 3].cart == 4) &&
                (cart[i + 2].cart == 3) &&
                (cart[i + 1].cart == 2)
            ){
                return Resulthand{vector<Hand>{cart[i + 4]}, Hand{}, Combs::straight, 1};
            }
        }
    }

    return Resulthand{vector<Hand>{}, Hand{}, Combs{0}, 0};
}

Resulthand checkFlash(vector<Hand> cart){
    auto suit = moreSuit(cart);

    auto iter = cart.begin();
    while(iter != cart.end()){
        if((*iter).suit != suit){
            cart.erase(iter);
        }
        else{
            iter++;
        }
    }

    if(cart.size() < 5){
        return Resulthand{vector<Hand>{}, Hand{}, Combs{0}, 0};
    }


    return Resulthand{
        vector<Hand>{
            cart[0],
            cart[1],
            cart[2],
            cart[3],
            cart[4]
        }, 
        Hand{}, 
        Combs::flash, 
        1
    };
}

Resulthand countLoop(vector<Hand> cart, int sizeLoop){
    int nowLoop = 0;
    Resulthand ans;

    if(sizeLoop == 1){
        ans.comb = Combs::pair;
    } else if(sizeLoop == 2){
        ans.comb = Combs::set;
    } else if(sizeLoop == 3){
        ans.comb = Combs::kare;
    } else{
        return Resulthand{};
    }

    for(int i = 0; i < cart.size() - 1; i++){
        if(cart[i].cart == cart[i + 1].cart){
            nowLoop++;
        }
        else{
            if(nowLoop == sizeLoop){
                ans.count++;
                ans.high.push_back(cart[i - 1]);
            }
            nowLoop = 0;
        }
    }

    if(nowLoop == sizeLoop){
        ans.count++;
        ans.high.push_back(cart.back());
    }

    ans.kiker = Hand{Cart{2}, Suit{1}};

    for(int i = 0; i < cart.size(); i++){
        bool isRight = true;
        for(int j = 0; j < ans.high.size(); j++){
            if(cart[i].cart == ans.high[j].cart){
                isRight = false;
            }
        }
        if(isRight){
            if(ans.kiker.cart < cart[i].cart){
                ans.kiker = cart[i];
            }
        }
    }

    return ans; 
}

Resulthand checkStraightFlash(vector<Hand> cart){
    auto suit = moreSuit(cart);

    auto iter = cart.begin();
    while(iter != cart.end()){
        if((*iter).suit != suit){
            cart.erase(iter);
        }
        else{
            iter++;
        }
    }

    if(cart.size() < 5){
        return Resulthand{vector<Hand>{}, Hand{}, Combs{0}, 0};
    }

    for(size_t i = 0; i <= cart.size() - 5; i++){
        int j = i + 1;
        for(j; j < i + 5; j++){
            if(cart[j].cart != cart[j - 1].cart - 1){
                break;
            }
        }   
        if(j == i + 5){
            return Resulthand{vector<Hand>{cart[i]}, Hand{}, Combs::straight_flash, 1};
        }
    }

    for(size_t i = 0; i <= cart.size() - 5; i++){
        if (cart[i].cart == 14) {  
            if(
                (cart[i + 4].cart == 5) &&
                (cart[i + 3].cart == 4) &&
                (cart[i + 2].cart == 3) &&
                (cart[i + 1].cart == 2) &&
                (cart[i + 4].suit == cart[i + 3].suit) &&
                (cart[i + 3].suit == cart[i + 2].suit) &&
                (cart[i + 2].suit == cart[i + 1].suit) &&
                (cart[i + 1].suit == cart[i].suit)
            ){
                return Resulthand{vector<Hand>{cart[i + 4]}, Hand{}, Combs::straight_flash, 1};
            }
        }
    }

        return Resulthand{vector<Hand>{}, Hand{}, Combs{0}, 0};
}

Resulthand getCombs(Hand hand, Hand hand1, vector<Hand> table){
    vector<Hand> allCart = table;
    allCart.push_back(hand);
    allCart.push_back(hand1);

    Resulthand ans;

    sort(allCart.begin(), allCart.end(), sortCart);
    
    ans = checkStraightFlash(allCart);
    if(ans.count == 1){ return ans; }

    ans = countLoop(allCart, 3);
    if(ans.count >= 1){ return ans; }
    
    Resulthand isSet = countLoop(allCart, 2);
    Resulthand isPair = countLoop(allCart, 1); 

    if(isSet.count == 2 || (isSet.count == 1 && isPair.count >= 1)){
        ans.comb = Combs::fullhouse;
        ans.count = 1;
        ans.high = isSet.high;
        if(isSet.count == 1){
            ans.high.push_back(isPair.high.front());
        }
        return ans;
    }

    ans = checkFlash(allCart);
    if(ans.count == 1){ return ans; }

    ans = checkStraight(allCart);
    if(ans.count == 1){ return ans; }

    if(isSet.count >= 1){ return isSet; }
    if(isPair.count >= 1)
    {
        isPair.comb = Combs::pair_pair; 
        return isPair; 
    }
    if(isPair.count == 1){return isPair; }

    return Resulthand{
        vector<Hand>{allCart[0]},
        allCart[1],
        Combs::older,
        1
    };
}

int winHand(Resulthand one, Resulthand two){   
    if(one.comb > two.comb){ return 1; }
    if(one.comb < two.comb){ return 2; }

    vector<Hand> oneCart = one.high;
    vector<Hand> twoCart = two.high;

    if(oneCart.front().cart > twoCart.front().cart){
        return 1;
    }
    if(oneCart.front().cart < twoCart.front().cart){
        return 2;
    }

    switch (one.comb)
    {
    case straight_flash:{
        return 0;
    } 
    case kare:{
        if(one.kiker.cart > two.kiker.cart){
            return 1;
        }
        if(one.kiker.cart < two.kiker.cart){
            return 2;
        }
        return 0;
    }
    case fullhouse:{
        if(oneCart[1].cart > twoCart[1].cart){
            return 1;
        }
        if(oneCart[1].cart < twoCart[1].cart){
            return 2;
        }
        return 0;
    }
    case flash:{
        for(size_t i = 1; i < oneCart.size(); i++){
            if(oneCart[i].cart > twoCart[i].cart){
                return 1;
            }
            if(oneCart[i].cart < twoCart[i].cart){
                return 2;
            }
        }
        return 0;
    }
    case straight:{
        return 0;
    }  
    case Combs::set:{
        if(one.kiker.cart > two.kiker.cart){
            return 1;
        }
        if(one.kiker.cart < two.kiker.cart){
            return 2;
        }
        return 0;
    }  
    case pair_pair:{
        if(oneCart[1].cart > twoCart[1].cart){
            return 1;
        }
        if(oneCart[1].cart < twoCart[1].cart){
            return 2;
        }
        if(one.kiker.cart > two.kiker.cart){
            return 1;
        }
        if(one.kiker.cart < two.kiker.cart){
            return 2;
        }
        return 0;
    }
    case Combs::pair:{
        if(one.kiker.cart > two.kiker.cart){
            return 1;
        }
        if(one.kiker.cart < two.kiker.cart){
            return 2;
        }
        return 0;
    }
    case older:{
        if(one.kiker.cart > two.kiker.cart){
            return 1;
        }
        if(one.kiker.cart < two.kiker.cart){
            return 2;
        }
        return 0;
    }
}

    return 0;
}

void procentWin(Resulthand combs, vector<Hand> table, std::set<Hand> handCart, Statistic& stat){
    
    for(auto x : table){
        handCart.insert(x);
    }

    vector<Hand> freeCart;
    for(int i = Cart::two; i <= Cart::A; i++){
        for(int j = Suit::spades; j <= Suit::clubs; j++){
            if(handCart.count({Cart{i}, Suit{j}}) == 0){
                freeCart.push_back({Cart{i}, Suit{j}});
            }
        }
    }

    auto w = 0; auto l = 0; auto n = 0;

    for(int i = 0; i < freeCart.size() - 1; i++){
        for(int j = i + 1; j < freeCart.size(); j++){
            auto win = winHand(combs, getCombs(freeCart[i], freeCart[j], table));
            if(win == 1){
                w++;
            } else if(win == 0){
                n++;
            }
            else{
                l++;
            }
        }
    }

    stat.win += w;
    stat.draw += n;
    stat.lose += l;
}

int main(){
    Hand hand1 = {Cart::A, Suit::clubs};
    Hand hand2 = {Cart::A, Suit::diamonds};
    std::set<Hand> handCart;
    handCart.insert(hand1);
    handCart.insert(hand2);

    vector<Hand> freeCart;
    for(int i = Cart::two; i <= Cart::A; i++){
        for(int j = Suit::spades; j <= Suit::clubs; j++){
            if(handCart.count({Cart{i}, Suit{j}}) == 0){
                freeCart.push_back({Cart{i}, Suit{j}});
            }
        }
    }

    Statistic stat;

    for(int i1 = 0; i1 < freeCart.size() - 4; i1++){
        for(int i2 = i1 + 1; i2 < freeCart.size() - 3; i2++){
            for(int i3 = i2 + 1; i3 < freeCart.size() - 2; i3++){
                for(int i4 = i3 + 1; i4 < freeCart.size() - 1; i4++){
                    for(int i5 = i4 + 1; i5 < freeCart.size(); i5++){
                        vector<Hand> table{
                            freeCart[i1],
                            freeCart[i2],
                            freeCart[i3],
                            freeCart[i4],
                            freeCart[i5]
                        };
                        auto combs = getCombs(hand1, hand2, table);
                        procentWin(combs, table, handCart, stat);
                    }
                }
                stat.print();
            }
        }
    }

    stat.print();

    return 0;
}
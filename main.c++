#include "lib/stdafx.h++"

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

ResultHand checkStraight(vector<Hand> cart){
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
        return ResultHand{vector<Hand>{}, Hand{}, Combs{0}, 0};
    }

    for(size_t i = 0; i <= cart.size() - 5; i++){
        int j = i + 1;
        for(j; j < i + 5; j++){
            if(cart[j].cart != cart[j - 1].cart - 1){
                break;
            }
        }   
        if(j == i + 5){
            return ResultHand{vector<Hand>{cart[i]}, Hand{}, Combs::straight, 1};
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
                return ResultHand{vector<Hand>{cart[i + 4]}, Hand{}, Combs::straight, 1};
            }
        }
    }

    return ResultHand{vector<Hand>{}, Hand{}, Combs{0}, 0};
}

ResultHand checkFlash(vector<Hand> cart){
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
        return ResultHand{vector<Hand>{}, Hand{}, Combs{0}, 0};
    }


    return ResultHand{
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

ResultHand countLoop(vector<Hand> cart, int sizeLoop){
    int nowLoop = 0;
    ResultHand ans;

    if(sizeLoop == 1){
        ans.comb = Combs::pair;
    } else if(sizeLoop == 2){
        ans.comb = Combs::set;
    } else if(sizeLoop == 3){
        ans.comb = Combs::kare;
    } else{
        return ResultHand{};
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

ResultHand checkStraightFlash(vector<Hand> cart){
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
        return ResultHand{vector<Hand>{}, Hand{}, Combs{0}, 0};
    }

    for(size_t i = 0; i <= cart.size() - 5; i++){
        int j = i + 1;
        for(j; j < i + 5; j++){
            if(cart[j].cart != cart[j - 1].cart - 1){
                break;
            }
        }   
        if(j == i + 5){
            return ResultHand{vector<Hand>{cart[i]}, Hand{}, Combs::straight_flash, 1};
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
                return ResultHand{vector<Hand>{cart[i + 4]}, Hand{}, Combs::straight_flash, 1};
            }
        }
    }

        return ResultHand{vector<Hand>{}, Hand{}, Combs{0}, 0};
}

ResultHand getCombs(Hand& hand, Hand& hand1, vector<Hand>& table){
    vector<Hand> allCart = table;
    allCart.push_back(hand);
    allCart.push_back(hand1);

    ResultHand ans;

    sort(allCart.begin(), allCart.end(), sortCart);
    
    ans = checkStraightFlash(allCart);
    if(ans.count == 1){ return ans; }

    ans = countLoop(allCart, 3);
    if(ans.count >= 1){ return ans; }
    
    ResultHand isSet = countLoop(allCart, 2);
    ResultHand isPair = countLoop(allCart, 1); 

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

    return ResultHand{
        vector<Hand>{allCart[0]},
        allCart[1],
        Combs::older,
        1
    };
}

int winHand(ResultHand one, ResultHand two){   
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

void duel(Hand one1, Hand one2, Hand two1, Hand two2, Statistic& stat, vector<Hand> freeCart){
    
    for(long i1 = 0; i1 < freeCart.size() - 4; i1++){
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
                        auto combs1 = getCombs(one1, one2, table);
                        auto combs2 = getCombs(two1, two2, table);
                        auto win = winHand(combs1, combs2);
                        if(win == 1){
                            stat.win++;
                        } else if(win == 0){
                            stat.draw++;
                        }
                        else{
                            stat.lose++;
                        }
                    }
                }
            }
        }
        stat.print();
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    application A;
    A.WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    return 0;
}
/*
int main(){

    vector<Hand> freeCartOnePlayer;
    for(int i = Cart::two; i <= Cart::A; i++){
        for(int j = Suit::spades; j <= Suit::clubs; j++){
            freeCartOnePlayer.push_back({Cart{i}, Suit{j}});
        } 
    }

    string file = "all.txt";
    file = PATH_TO_DB + file;
    DataBase A(file);

    for(int i1 = 0; i1 < freeCartOnePlayer.size() - 3; i1++){
        Hand hand1 = freeCartOnePlayer[i1];
        for(int i2 = i1 + 1; i2 < freeCartOnePlayer.size() - 2; i2++){
            Hand hand2 = freeCartOnePlayer[i2];
            for(int i3 = i2 + 1; i3 < freeCartOnePlayer.size() - 1; i3++){
                Hand hand3 = freeCartOnePlayer[i3];
                for(int i4 = i3 + 1; i4 < freeCartOnePlayer.size(); i4++){
                    Hand hand4 = freeCartOnePlayer[i4];

                    std::set<Hand> handCart;
                    handCart.insert(hand1);
                    handCart.insert(hand2);
                    handCart.insert(hand3);
                    handCart.insert(hand4);

                    vector<Hand> freeCart;
                    for(int i = Cart::two; i <= Cart::A; i++){
                        for(int j = Suit::spades; j <= Suit::clubs; j++){
                            if(handCart.count({Cart{i}, Suit{j}}) == 0){
                                freeCart.push_back({Cart{i}, Suit{j}});
                            }
                        }
                    }

                    Statistic stat;
                    duel(hand1, hand2, hand3, hand4, stat, freeCart);

                    A.addCart(stat, hand1, hand2);
                    A.addCart(stat, hand2, hand1);

                    auto temp = stat.lose;
                    stat.lose = stat.win;
                    stat.win = temp;

                    A.addCart(stat, hand3, hand4);
                    A.addCart(stat, hand4, hand3);

                }
            } 
            A.createReserve(file);
            cout << "complete" << endl;
            hand1.print();
            hand2.print();
            cout << "save" << endl << endl;
        }
    }

    return 0;
}
*/
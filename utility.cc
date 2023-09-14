#include "utility.h"

Order::Order() {
    username = "";
    side = "";
    asset = "";
    amount = 0;
    price = 0;
}

Order::Order(string user, string market, string asset_type, int amt, int money){
    username = user;
    side = market;
    asset = asset_type;
    amount = amt;
    price = money;
}

Order& Order::operator=(const Order& o){
    username = o.username;
    side = o.side;
    asset = o.asset;
    amount = o.amount;
    price = o.price;
    return *this;
}

ostream& operator<<(ostream& out, const Order& o){
    //"Buy 100 BTC at 60000 USD by Nahum"
    out << o.side << " " << o.amount << " " << o.asset << " at " 
        << o.price << " USD by " << o.username;
    return out;
}
bool Order::operator==(const Order& o) const{
    if (username == o.username && side == o.side && asset == o.asset 
        && amount == o.amount && price == o.price) {
        return true;
    }
    return false;
}

Trade::Trade(string buy_user, string sell_user, string asset_type, int amt, 
             int money){
    buyer_username = buy_user;
    seller_username = sell_user;
    asset = asset_type;
    amount = amt;
    price = money;
}

Trade::Trade(){
    buyer_username = "";
    seller_username = "";
    asset = "";
    amount = 0;
    price = 0;
}

Trade& Trade::operator=(const Trade& t){
    buyer_username = t.buyer_username;
    seller_username = t.seller_username;
    asset = t.asset;
    amount = t.amount;
    price = t.price;
    return *this;
}

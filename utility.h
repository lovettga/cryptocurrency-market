#pragma once
#include <iostream>
#include <string>
using std::string; using std::ostream;

class Order {
  public:
    std::string username;
    std::string side;  // Can be "Buy" or "Sell"
    std::string asset;
    int amount;
    int price;
    Order();
    Order(string, string, string, int, int);
    friend ostream& operator<<(ostream&, const Order&);
    bool operator==(const Order&) const;
    Order& operator=(const Order&);
};
ostream& operator<<(ostream&, const Order&);

class Trade {
  public:
    std::string buyer_username;
    std::string seller_username;
    std::string asset;
    int amount;
    int price;
    Trade& operator=(const Trade& t);
    Trade();
    Trade(string, string, string, int, int);
};
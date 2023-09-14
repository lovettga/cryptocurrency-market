#include "exchange.h"
#include <iostream>
#include <sstream>
#include <string>

int main() {
    //below is an example of orders being made and filled with this program
    Exchange e;
    e.MakeDeposit("Nahum", "BTC", 1000);
    e.MakeDeposit("Nahum", "USD", 100000);
    e.MakeDeposit("Dolson", "USD", 555555);
    e.MakeDeposit("Ofria", "ETH", 678);
    e.MakeDeposit("Zaabar", "USD", 12121212);
    e.MakeDeposit("Zaabar", "LTC", 4563);
    std::cout << "User's portfolios before trades occur:" << std::endl;
    e.PrintUserPortfolios(std::cout);
    e.AddOrder({"Nahum", "Sell", "BTC", 5, 1100});
    e.AddOrder({"Nahum", "Sell", "BTC", 100, 1200});
    e.AddOrder({"Nahum", "Buy", "BTC", 7, 800});
    e.AddOrder({"Dolson", "Buy", "BTC", 1, 950});
    e.AddOrder({"Ofria", "Sell", "ETH", 12, 156});
    e.AddOrder({"Ofria", "Sell", "ETH", 10, 160});
    e.AddOrder({"Zaabar", "Sell", "LTC", 10, 550});
    e.AddOrder({"Zaabar", "Buy", "LTC", 10, 450});
    e.AddOrder({"Nahum", "Buy", "LTC", 55, 600});
    e.AddOrder({"Nahum", "Buy", "ETH", 30, 158});
    e.AddOrder({"Ofria", "Sell", "ETH", 10, 140});
    e.AddOrder({"Zaabar", "Buy", "BTC", 10, 1500});
    e.AddOrder({"Zaabar", "Buy", "ETH", 200, 1200});
    e.AddOrder({"Nahum", "Sell", "BTC", 30, 158});
    e.AddOrder({"Dolson", "Buy", "BTC", 20, 2000});
    e.AddOrder({"Nahum", "Sell", "ETH", 1, 1423});
    std::cout << std::endl << "User's portfolios after trades occur:" 
        << std::endl;
    e.PrintUserPortfolios(std::cout);
    std::cout << std::endl;
    e.PrintUsersOrders(std::cout);
    std::cout << std::endl;
    e.PrintBidAskSpread(std::cout);
    return 0;
}

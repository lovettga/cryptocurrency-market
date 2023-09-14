#include "exchange.h"
#include "utility.h"
#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
using std::string; using std::vector;


//this function should take a username (e.g. "Nahum"), an asset (e.g. "BTC"), 
//and an amount (e.g. 100). The function doesn't return anything, but  
//stores the information that the deposit occured to a user's portfolio. 
void Exchange::MakeDeposit(const std::string &username, const std::string &asset,
                           int amount) {
    //get the existing portfolio (vector) associated with the user
    vector<std::pair<string, int>> user_portfolio = users[username];
    //check if the asset already exists and add amount
    int success = 0;
    for (auto& ele: user_portfolio) {
        if (ele.first == asset) {
            ele.second = ele.second + amount;
            success++;
        }
    }
    //create asset within portfolio if it doesn't exist
    if (success == 0) {
        std::pair<string, int> to_deposit(asset, amount);
        user_portfolio.push_back(to_deposit);
    }
    //sort the vector portfolio by asset
    std::sort(user_portfolio.begin(), user_portfolio.end());
    users[username] = user_portfolio;
    //if user doesn't have order_book, create one
    auto itr = order_book.find(username);
    if (itr == order_book.end()) {
        std::pair<vector<Order>,vector<Order>> empty_order;
        order_book[username] = empty_order;
    }
}

//this function takes a reference to a std::ostream and prints out each 
//user's porfolio (ordered alphabetically by user's name, and then asset). 
void Exchange::PrintUserPortfolios(std::ostream &os) const{
    os << "User Portfolios (in alphabetical order):";
    for (auto ele1: users) {
        os << "\n" << ele1.first << "'s Portfolio: ";
        for (auto ele2: ele1.second) {
            //ele2.second = amount and ele2.first = asset
            os << ele2.second << " " << ele2.first << ", ";
        }
    }
    os << "\n";
}

//this function has the same parameters as MakeDeposit, but instead of depositing 
//funds in the exchange, the user is removing already deposited funds. The 
//function returns true if the withdrawl was successful. A withdrawl can only be 
//unsuccessful if there aren't sufficient assets in a user's portfolio.
bool Exchange::MakeWithdrawal(const std::string &username, 
                              const std::string &asset, int amount){
    auto itr = users.find(username);
    //if the user doesn't have a portfolio
    if (itr == users.end()) {
        return false;
    }
    vector<std::pair<string, int>> user_portfolio = users[username];
    int pos = -1;
    //determine if the asset is in the user's portfolio
    for (auto& ele: user_portfolio) {
        pos++;
        if (ele.first == asset) {
            //determine if asset has enough amount to withdraw
            if (ele.second >= amount) {
                ele.second = ele.second - amount;
                if (ele.second == 0) {
                    user_portfolio.erase(user_portfolio.begin()+pos);
                }
                users[username] = user_portfolio;
                return true;
            }
        }
    }
    return false;
}

//helper function for AddOrder to process a buy order and determine if the 
//trade is valid
bool Exchange::BuyOrder(const Order &order) {
    vector<std::pair<string, int>> user_portfolio = users[order.username];
    int total_cost = order.price * order.amount;
    int pos = -1;
    for (auto& ele: user_portfolio) {
        pos++;
        if (ele.first == "USD") {
            //determine if asset has enough USD to withdraw
            if (ele.second >= total_cost) {
                ele.second = ele.second - total_cost;
                if (ele.second == 0) {
                    user_portfolio.erase(user_portfolio.begin()+pos);
                }
                users[order.username] = user_portfolio;
                //update the order book
                for (auto& ele: order_book) {
                    if (ele.first == order.username) {
                        ele.second.first.push_back(order);
                    }
                }
                return true;
            }
        }
    }
    return false;
}

//helper function for AddOrder to process a sell order and determine if the 
//trade is valid
bool Exchange::SellOrder(const Order &order) {
    vector<std::pair<string, int>> user_portfolio = users[order.username];
    int pos = -1;
    for (auto& ele: user_portfolio) {
        pos++;
        if (ele.first == order.asset) {
            //determine if asset has enough of the asset to withdraw
            if (ele.second >= order.amount) {
                ele.second = ele.second - order.amount;
                if (ele.second == 0) {
                    user_portfolio.erase(user_portfolio.begin()+pos);
                }
                users[order.username] = user_portfolio;
                //update the order book
                for (auto& ele: order_book) {
                    if (ele.first == order.username) {
                        ele.second.first.push_back(order);
                    }
                }
                return true;
            }
        }
    }
    return false;
}

//takes buy order and sell order and creates Trade
Trade Exchange::ConductTrade(const Order& maker, Order& taker) {
    //determine which order is limiting
    Order limiting_order;
    if (maker.amount > taker.amount) {
        limiting_order = taker;
    }
    else {
        limiting_order = maker;
    }
    Trade new_trade;
    //determine which order is buying vs selling
    if (maker.side == "Buy") {
        Trade temp_trade(maker.username, taker.username, maker.asset, 
                         limiting_order.amount, taker.price);
        new_trade = temp_trade;
        //give the buyer the asset and the seller the USD
        MakeDeposit(maker.username, taker.asset, limiting_order.amount);
        MakeDeposit(taker.username, "USD", (limiting_order.amount* taker.price));
    }
    //if taker is Buy
    else {
        Trade temp_trade(taker.username, maker.username, maker.asset, 
                         limiting_order.amount, taker.price);
        new_trade = temp_trade;
        //give the buyer the asset and the seller the USD
        MakeDeposit(taker.username, taker.asset, limiting_order.amount);
        MakeDeposit(maker.username, "USD", (limiting_order.amount* taker.price));
    }
    //update the open order and filled orders for the maker and taker
    bool update_maker = true;
    bool update_taker = true;
    for (auto& ele: order_book) {
        int pos = -1;
        //loop through every user's Open Orders
        for (auto& open_order: ele.second.first) {
            pos++;
            if (open_order == maker && update_maker) {
                update_maker = false;
                open_order.amount = open_order.amount - limiting_order.amount;
                Order new_order(maker.username, maker.side, maker.asset, 
                                limiting_order.amount, taker.price);
                //update the filled order
                ele.second.second.push_back(new_order);
            }
            else if (open_order == taker && update_taker) {
                update_taker = false;
                open_order.amount = open_order.amount - limiting_order.amount;
                Order new_order(taker.username, taker.side, taker.asset, 
                                limiting_order.amount, taker.price);
                //update the filled order
                ele.second.second.push_back(new_order);
                //update taker order
                taker = open_order;
            }
        }
    }
    return new_trade;
}

//takes an Order (declared in utility.h) and returns true if the order was 
//successfully placed. The only reason an order could not be placed is if there 
//were insufficient assets to cover the trade.
bool Exchange::AddOrder(Order order){
    vector<std::pair<string, int>> user_portfolio = users[order.username];
    string task = order.side;
    bool success;
    if (task == "Buy") {
        success = BuyOrder(order);
    }
    //otherwise task == "Sell"
    else {
        success = SellOrder(order);
    }
    if (success == true) {
        vector<Order> possible_orders;
        //check to see if Trade can occur (loop through all Open Orders)
        for (auto& ele : order_book) {
            //if there are two people selling below your buying price, the 
            //lowest seller's order is filled first. if you are selling and 
            //there are two buyers above, the higher buyer is sold to first.
            for (auto& open_order: ele.second.first) {
                if (open_order.side != order.side && 
                    open_order.asset == order.asset) {
                    //determine buyer to figure out buyer price
                    Order buy_order, sell_order;
                    if (open_order.side == "Buy") {
                        buy_order = open_order;
                        sell_order = order;
                    }
                    else {
                        buy_order = order;
                        sell_order = open_order;
                    }
                    //buyer.price is maximum price the buyer will pay, they 
                    //will pay for anything lower
                    if (sell_order.price <= buy_order.price && 
                        open_order.amount != 0 && order.amount != 0) {
                        possible_orders.push_back(open_order);
                    }
                }
            }
            if (order.side == "Sell") {
                //sorts largest to smallest for Seller
                std::sort(possible_orders.begin(), possible_orders.end(), 
                    [](Order a, Order b) {if (a.price == b.price) {return false;} 
                                          else if (a.price > b.price) {return true;}
                                          else {return false;}});
            }
            else if (order.side == "Buy") {
                //sorts smallest to largest for buyer
                std::sort(possible_orders.begin(), possible_orders.end(), 
                    [](Order a, Order b)  {if (a.price == b.price) {return false;} 
                                          else if (a.price < b.price) {return true;}
                                          else {return false;}});
            }
            int num_orders = static_cast<int>(possible_orders.size());
            for (int i = 0; i < num_orders; i++) {
                std::vector<Order>::iterator it;
                it = std::find(ele.second.first.begin(), ele.second.first.end(), 
                               possible_orders[i]);
                if (it != ele.second.first.end()) {
                    //if trade can occur update trade log
                    if ((*it).amount != 0 && order.amount != 0) {
                        Trade trade = ConductTrade(*it, order);
                        trade_log.push_back(trade);
                    }
                }
            }
        }
    }
    //erase every open_order that has 0 amount of an asset left
    for (auto& ele: order_book) {
        //loop through every user's Open Orders
        int pos = -1;
        for (auto& open_order: ele.second.first) {
            pos++;
            if (open_order.amount == 0) {
                ele.second.first.erase(ele.second.first.begin() + pos);
            }
        }
    }
    return success;
}

//for each user (ordered alphabetically), this function prints the open and 
//filled trades (ordered chronologiclly as each trade was opened/filled) to the 
//referenced std::ostream.
void Exchange::PrintUsersOrders(std::ostream &os) const{
    os << "Users Orders (in alphabetical order):\n";
    for (auto ele1: order_book) {
        os << ele1.first << "'s Open Orders (in chronological order):\n";
        for (auto ele2: ele1.second.first) {
            //format for printing: Sell 4 BTC at 1000 USD by Nahum
            os << ele2.side << " " << ele2.amount << " " << ele2.asset << " at " 
               << ele2.price << " USD by " << ele2.username << "\n";
        }
        os << ele1.first << "'s Filled Orders (in chronological order):\n";
        //loops through the user's Filled Orders
        for (auto ele3: ele1.second.second) {
            //format for printing: Sell 4 BTC at 1000 USD by Nahum
            os << ele3.side << " " << ele3.amount << " " << ele3.asset << " at " 
               << ele3.price << " USD by " << ele3.username << "\n";
        }
    }
}

//prints a record of each trade (chronologically) that occured to the 
//provided reference to std::ostream.
void Exchange::PrintTradeHistory(std::ostream &os) const{
    os << "Trade History (in chronological order):\n";
    for (auto ele: trade_log) {
        os << ele.buyer_username << " Bought " << ele.amount << " of " << ele.asset 
            << " From " << ele.seller_username << " for " << ele.price << " USD\n";
    }
}

//for each asset (ordered alphabetically), print the bid ask spread dictated 
//by the open orders to the reference to std::ostream. 
void Exchange::PrintBidAskSpread(std::ostream &os) {
    //loop through every open order, determine the bid_ask_spread for every asset
    for (auto ele: order_book) {
        //loops through every open order once
        for (auto open_order: ele.second.first) {
            //in every open order determine asset and related price
            bool spread_exists = false;
            for (auto& spread: bid_ask_spread) {
                if (open_order.asset == spread.first) {
                    spread_exists = true;
                    //update the spread based on buy/sell
                    if (open_order.side == "Buy") {
                        for (auto each_order : ele.second.first) {
                            if (each_order.side == "Buy" && 
                                each_order.asset == open_order.asset){
                                if (each_order.price > spread.second.first || 
                                    spread.second.first == 0) {
                                    bid_ask_spread[open_order.asset].first = 
                                        each_order.price; 
                                }
                            }
                        }
                    }
                    else if (open_order.side == "Sell") {
                        for (auto each_order : ele.second.first) {
                            if (each_order.side == "Sell" && each_order.asset == 
                                open_order.asset ){
                                if (each_order.price < spread.second.second || 
                                    spread.second.second == 0) {
                                    bid_ask_spread[open_order.asset].second = 
                                        each_order.price; 
                                }
                            }
                        } 
                    }
                }
            }
            if (spread_exists == false) {
                if (open_order.side == "Buy") {
                    std::pair<int, int> new_spread(open_order.price, 0);
                    bid_ask_spread[open_order.asset] = new_spread;     
                }
                else {
                    std::pair<int, int> new_spread(0, open_order.price);
                    bid_ask_spread[open_order.asset] = new_spread;     
                }
            }
        }
    }
    os << "Asset Bid Ask Spread (in alphabetical order):\n";
    //format: LTC: Highest Open Buy = 600 USD and Lowest Open Sell = NA USD\n"
    for (auto ele : bid_ask_spread) {
        os << ele.first << ": Highest Open Buy = ";
        if (ele.second.first == 0) {
            os << "NA USD";
        }
        else {
            os << ele.second.first << " USD";
        }
        os << " and Lowest Open Sell = ";
        if (ele.second.second == 0) {
            os << "NA USD";
        }
        else {
            os << ele.second.second << " USD";
        }
        os << "\n";
    }
}

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
using std::string; using std::vector;

#include "utility.h"

class Exchange {
 public:
  //map to hold the users porfolio: map<username, vector<pair<asset, amount>>>
  std::map<string, vector<std::pair<string, int>>> users;
  //order book to keep track of all orders:
  //map<username, std::pair<vector<OpenOrder>,vector<FilledOrder>>>
  std::map<string, std::pair<vector<Order>,vector<Order>>> order_book;
  //trade log to hold all the trades that occur in chronological order
  vector<Trade> trade_log;
  //bid-ask log to hold highest open buy and lowest open sell: 
  //map<asset, pair<highest Buy USD, lowest Sell USD>>
  std::map<string, std::pair<int, int>> bid_ask_spread;
  void MakeDeposit(const std::string &username, const std::string &asset,
                   int amount);
  void PrintUserPortfolios(std::ostream &os) const;
  bool MakeWithdrawal(const std::string &username, const std::string &asset,
                      int amount);
  bool AddOrder(Order order);
  bool BuyOrder(const Order &order);
  bool SellOrder(const Order &order);
  Trade ConductTrade(const Order& maker, Order& taker);
  void PrintUsersOrders(std::ostream &os) const;
  void PrintTradeHistory(std::ostream &os) const;
  void PrintBidAskSpread(std::ostream &os);
};

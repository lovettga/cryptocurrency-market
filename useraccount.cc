#include "useraccount.h"

/*
                                                                                | << this is 80 line limit */


//Nahum's Portfolio: 10 BTC, 4 ETH, 9000 USD,
//initializes UserAccount with params username and pair<amount, currancy type> 
//as a deposit to add to the portfolio
UserAccount::UserAccount(const string& user, const pair<int,string>& deposit) {
    username = user;
    portfolio.push_back(deposit);
}
//takes a pair<amount, currancy type> and adds it to the account portfolio
void UserAccount::UpdatePortfolio(UserAccount& person, const pair<int,string>& deposit) {
    person.portfolio.push_back(deposit);
}


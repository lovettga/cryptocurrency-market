#pragma once
#include <string>
#include <vector>
#include <utility>
using std::string; using std::vector; using std::pair;

/*
                                                                                | << this is 80 line limit */


class UserAccount{
    public:
        string username;
        //holds all of the currency deposits with <amt,currency_type>
        vector<pair<int,string>> portfolio;

        UserAccount(const string& user, const pair<int,string>& deposit);
        void UpdatePortfolio(UserAccount& person, 
                             const pair<int,string>& deposit);
        //next function
};
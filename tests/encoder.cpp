/**
 * @file encoder.cpp
 * @author Dion Ong
 * @brief For encoding an edge table (input through stdin)
 * @date 2022-12-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include<bits/stdc++.h>
using namespace std;

string chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+/";

string convert(int x) {
    bool negative = false;
    if (x == 0) return "0";
    if (x < 0) {
        negative = true;
        x = -x;
    }
    string ret = "";
    while(x > 0) {
        ret.push_back(chars[x % 64]);
        x /= 64;
    }
    if(negative) {
        ret.push_back('-');
    }
    reverse(all(ret));
    return ret;
}

int main(){
    vector<int> table;
    int x;
    int maxi = INT_MIN;
    int mini = INT_MAX;
    map<int, int> mp;
    while(cin>>x) {
        mp[x]++;
        table.push_back(x);
    }
    vector<pair<int, int>> v(mp.begin(), mp.end());
    sort(all(v), [&](auto x, auto y) {
        return x.S > y.S;
    });
    map<int, int> idx;
    map<int, int> rev;
    int cnt = 0;
    for(auto [x, y] : v) {
        idx[x] = cnt++;
        rev[idx[x]] = x;
    }

    // generate the values array
    for (int i = 0; i < cnt; i++) {
        cout << rev[i] << ", ";
    }
    cout << "\n";

    // generate the encoded table
    for(int i=0; i<table.size(); i++) {
        if (i and table[i] == table[i-1]) {
            cout << "|";
            assert(table[i] == table[i+1]);
            i++;
        }
        else {
            cout << convert(idx[table[i]]);
            if (i+1 != table.size() and table[i] != table[i+1]) {
                cout << ",";
            }
        }
    }
}

#ifndef USR_RECORD_H
#define USR_RECORD_H

#include <iostream>
#include <string>
#include <stack>
#include <QString>

using namespace std;

class UsrRecord
{
private:
    string fname;
    string lname;
    int phone;
    int id;
    double balance;
    stack<pair<int, int>> transactions;

public:
    UsrRecord();
    UsrRecord(string msg);
    string get_fname();
    string get_lname();
    int get_phone();
    int get_id();
    string get_balance();
    void set_balance(int bal);
};

#endif // USR_RECORD_H

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
    float balance;
    stack<float> transactions;

public:
    UsrRecord();
    UsrRecord(string msg);
    string get_fname();
    string get_lname();
    int get_phone();
    int get_id();
    string get_balance();
    void set_balance(float bal);
    stack<float> get_transactions();
    float cancel_last_trans();
};

#endif // USR_RECORD_H

#ifndef USR_RECORD_H
#define USR_RECORD_H

#include <iostream>
#include <string>
#include <QString>

using namespace std;

class UsrRecord
{
private:
    string fname;
    string lname;
    int phone;
    int id;

public:
    UsrRecord();
    UsrRecord(string msg);
    string get_fname();
    int get_phone();
};

#endif // USR_RECORD_H

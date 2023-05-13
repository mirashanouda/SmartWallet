#include "usr_record.h"

UsrRecord::UsrRecord()
{

}

UsrRecord::UsrRecord(string msg)
{
    int commas = 0;
    string s_phone = "",
           s_id = "";
    for(int i = 0; i < msg.size(); i++){
        if (msg[i] == ',') {
            commas++;
            continue;
        }
        if(commas == 0) fname += msg[i];
        else if(commas == 1) lname += msg[i];
        else if(commas == 2) s_phone += msg[i];
        else if(commas == 3) s_id += msg[i];
    }
    phone = stoi(s_phone);
    id = stoi(s_id);
}

string UsrRecord::get_fname()
{
    return fname;
}

int UsrRecord::get_phone()
{
    return phone;
}

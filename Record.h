//
// Created by 沈慧昌 on 2023/3/10.
//
#ifndef CHARGINGMANAGEMENTSYSTEM_RECORD_H
#define CHARGINGMANAGEMENTSYSTEM_RECORD_H
#include <bits/stdc++.h>
using namespace std;

class Record {
public:
    string cardId;
    time_t beginTime;
    time_t endTime;
    double money;
public:
    Record(string cardId, time_t beginTime, time_t endTime, double money);

    Record();
};


#endif //CHARGINGMANAGEMENTSYSTEM_RECORD_H

//
// Created by 沈慧昌 on 2023/2/24.
//
#ifndef CHARGINGMANAGEMENTSYSTEM_MACHINE_H
#define CHARGINGMANAGEMENTSYSTEM_MACHINE_H
#include <bits/stdc++.h>
using namespace std;


class Machine {
public:
    /**
     * 编号
     */
     int id;
    /**
     * 是否空闲
     */
    bool isFree;
    /**
     * 上机时间
     */
     time_t time;
     /**
      * 卡号
      */
      string cardId;

public:
    Machine(int id);
    Machine();
};


#endif //CHARGINGMANAGEMENTSYSTEM_MACHINE_H

//
// Created by 沈慧昌 on 2023/2/24.
//
#ifndef CHARGINGMANAGEMENTSYSTEM_CARD_H
#define CHARGINGMANAGEMENTSYSTEM_CARD_H
#include <bits/stdc++.h>
using namespace std;


class Card {
public:
    /**
     * 卡号
     */
    string cardId;
    /**
     * 密码
     */
    string password;
    /**
     * 卡内金额
     */
    double money;
    /**
     * 累计金额
     */
    double historyMoney;
    /**
    * 状态（0表示注销，1表示未上机，2表示上机）
    */
    int state;
    /**
    * 使用次数
    */
    int frequency;
    /**
    * 上次使用时间
    */
    time_t time;
    /**
     * 开卡时间
     */
    time_t startTime;

public:
    Card(string id, string password, double money);
    Card();
    friend ostream &operator<<(ostream &os, const Card &card);
};


#endif //CHARGINGMANAGEMENTSYSTEM_CARD_H

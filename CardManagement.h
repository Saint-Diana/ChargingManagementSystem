//
// Created by 沈慧昌 on 2023/2/24.
//
#ifndef CHARGINGMANAGEMENTSYSTEM_CARDMANAGEMENT_H
#define CHARGINGMANAGEMENTSYSTEM_CARDMANAGEMENT_H
#include "Card.h"
#include "Constant.h"
#include "Machine.h"
#include "Admin.h"
#include "Record.h"
using namespace std;

class CardManagement {
public:
    int cardNum;
    Card* cards;
    Machine* machines;
    Admin admin;
    Record* records;
    int recordNum;
    double price_per_second;
public:
    CardManagement();
    ~CardManagement();
    void addCard(string cardId, string password, double money);
    bool isExist(string cardId);
    bool isExistFreeMachine();
    bool accurateQuery(string cardId);
    bool fuzzyQuery(string cardId);
    void login(string cardId, const string& password);
    void logout(string cardId, const string& password);
    void charge(string cardId, const string& password, double money);
    void refund(string cardId, const string& password, double money);
    void deleteCard(string cardId, const string& password);
    //以下为管理员功能
    //1、恢复注销卡
    void recoverCard(string adminName, string adminPassword, string cardId, const string& password);
    //2、查看所有机器的信息
    void printAllMachines(string username, string password);
    //3、查看所有卡的信息
    void printAllCards(string username, string password);
    //4、统计某时间段内的开卡记录
    void cardRecords(time_t beginTime, time_t endTime);
    //5、统计某时间段内的消费记录
    void consumeRecords(time_t beginTime, time_t endTime);
    //6、修改单价
    void changePrice(string username, string password, double price);
    //以下为IO操作
private:
    void saveCards();
    void readCards();
    void readAdmin();
    void saveMachines();
    void readMachines();
    void readRecords();
    void saveRecords();
    void readPrice();
    void savePrice();
};


#endif //CHARGINGMANAGEMENTSYSTEM_CARDMANAGEMENT_H

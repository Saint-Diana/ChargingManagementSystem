#pragma once
#include <bits/stdc++.h>
#include "CardManagement.h"
using namespace std;

CardManagement management;

const vector<string> choices = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "a", "b", "c"};

void showMenu(){
    printf("----------菜单----------\n");
    printf("1、添加卡\n");
    printf("2、查询卡\n");
    printf("3、上机\n");
    printf("4、下机\n");
    printf("5、充值\n");
    printf("6、退费\n");
    printf("7、查询统计\n");
    printf("8、注销卡\n");
    printf("9、恢复注销卡（管理员功能）\n");
    printf("a、查看所有机器信息（管理员功能）\n");
    printf("b、查看所有卡信息（管理员功能）\n");
    printf("c、修改单价（管理员功能）\n");
    printf("0、退出\n");
    printf("请选择菜单项编号：");
}

time_t string2time_t(const string& string_time)
{
    struct tm tm1;
    memset(&tm1, 0, sizeof(tm1));
    time_t time1;

    sscanf_s(string_time.c_str(), "%4d-%2d-%2d %2d:%2d:%2d",
             &(tm1.tm_year),
             &(tm1.tm_mon),
             &(tm1.tm_mday),
             &(tm1.tm_hour),
             &(tm1.tm_min),
             &(tm1.tm_sec));

    tm1.tm_year -= 1900;
    tm1.tm_mon -= 1;

    time1 = mktime(&tm1);

    return time1;

}


int main() {
    printf("欢迎进入计费管理系统\n");
    string choice;
    while (true){
        showMenu();
        cin >> choice;
        //当用户输入不为0~8时
        while(std::find(choices.begin(), choices.end(), choice) == choices.end()){
            printf("输入错误，请重新输入！\n");
            printf("请选择菜单项编号：");
            cin >> choice;
        }
        switch (choice[0]) {
            case '1':{
                string cardId, password;
                double money;
                printf("请输入卡号（1~18位数字串）：");
                cin >> cardId;
                while (cardId.length() > 18){
                    printf("卡号过长，请重新输入：");
                    cin >> cardId;
                }
                while (management.isExist(cardId)){
                    printf("已存在相同卡号，请重新输入：");
                    cin >> cardId;
                }
                printf("请输入密码（1~18位数字串）：");
                cin >> password;
                printf("请输入您的开卡金额：");
                cin >> money;
                while (money <= 0){
                    printf("输入金额错误，请重新输入：");
                    cin >> money;
                }
                management.addCard(cardId, password, money);
                break;
            }
            case '2':{
                string c;
                printf("请选择准确查询（1），还是模糊查询（2）：");
                cin >> c;
                while (c != "1" && c != "2"){
                    printf("输入错误，请重新输入：");
                    cin >> c;
                }
                string cardId;
                printf("请输入查询的卡号：");
                cin >> cardId;
                if(c == "1"){
                    management.accurateQuery(cardId);
                }else if(c == "2"){
                    management.fuzzyQuery(cardId);
                }
                break;
            }
            case '3':{
                string cardId;
                string password;
                printf("请输入卡号（1~18位数字串）：");
                cin >> cardId;
                printf("请输入密码：");
                cin >> password;
                management.login(cardId, password);
                break;
            }
            case '4':{
                string cardId;
                string password;
                printf("请输入卡号（1~18位数字串）：");
                cin >> cardId;
                printf("请输入密码：");
                cin >> password;
                management.logout(cardId, password);
                break;
            }
            case '5':{
                string cardId;
                string password;
                double money;
                printf("请输入需要充值的卡号（1~18位数字串）：");
                cin >> cardId;
                printf("请输入密码：");
                cin >> password;
                printf("请输入充值金额：");
                cin >> money;
                while (money <= 0){
                    printf("输入金额错误，请重新输入：");
                    cin >> money;
                }
                management.charge(cardId, password, money);
                break;
            }
            case '6':{
                string cardId;
                string password;
                double money;
                printf("请输入需要退费的卡号（1~18位数字串）：");
                cin >> cardId;
                printf("请输入密码：");
                cin >> password;
                printf("请输入退费金额：");
                cin >> money;
                while (money <= 0){
                    printf("输入金额错误，请重新输入：");
                    cin >> money;
                }
                management.refund(cardId, password, money);
                break;
            }
            case '7':{
                printf("1、开卡记录\n");
                printf("2、消费记录\n");
                printf("请选择菜单编号：");
                string c;
                cin >> c;
                while (c != "1" && c != "2"){
                    cout << "输入错误，请重新输入：";
                    cin >> c;
                }
                if(c == "1"){
                    time_t beginTime;
                    time_t endTime;
                    cout << "请输入统计的开始时间：";
                    char temp[30];
                    cin.getline(temp, 30);
                    cin.getline(temp, 30);
                    beginTime = string2time_t(temp);
                    cout << "请输入统计的结束时间：";
                    cin.getline(temp, 30);
                    endTime = string2time_t(temp);
                    management.cardRecords(beginTime, endTime);
                }else {
                    time_t beginTime;
                    time_t endTime;
                    cout << "请输入统计的开始时间：";
                    char temp[30];
                    cin.getline(temp, 30);
                    cin.getline(temp, 30);
                    beginTime = string2time_t(temp);
                    cout << "请输入统计的结束时间：";
                    cin.getline(temp, 30);
                    endTime = string2time_t(temp);
                    management.consumeRecords(beginTime, endTime);
                }
                break;
            }
            case '8':{
                string cardId;
                string password;
                printf("请输入需要注销的卡号（1~18位数字串）：");
                cin >> cardId;
                printf("请输入密码：");
                cin >> password;
                management.deleteCard(cardId, password);
                break;
            }
            case '9':{
                string username;
                string adminPassword;
                string cardId;
                string password;
                printf("请输入管理员用户名：");
                cin >> username;
                printf("请输入管理员密码：");
                cin >> adminPassword;
                printf("请输入需要注销的卡号（1~18位数字串）：");
                cin >> cardId;
                printf("请输入密码：");
                cin >> password;
                management.recoverCard(username, adminPassword, cardId, password);
                break;
            }
            case 'a':{
                string username;
                string password;
                printf("请输入管理员用户名：");
                cin >> username;
                printf("请输入管理员密码：");
                cin >> password;
                management.printAllMachines(username, password);
                break;
            }
            case 'b':{
                string username;
                string password;
                printf("请输入管理员用户名：");
                cin >> username;
                printf("请输入管理员密码：");
                cin >> password;
                management.printAllCards(username, password);
                break;
            }
            case 'c':{
                string username;
                string password;
                printf("请输入管理员用户名：");
                cin >> username;
                printf("请输入管理员密码：");
                cin >> password;
                printf("请输入新的单价：");
                double price;
                cin >> price;
                management.changePrice(username, password, price);
                break;
            }
            case '0':{
                printf("期待您的下次使用，再见！");
                return 0;
            }
        }
    }
}

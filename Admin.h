//
// Created by 沈慧昌 on 2023/2/25.
//
#ifndef CHARGINGMANAGEMENTSYSTEM_ADMIN_H
#define CHARGINGMANAGEMENTSYSTEM_ADMIN_H
#include <bits/stdc++.h>
using namespace std;

class Admin {
public:
    string username;
    string password;
public:
    Admin();
    Admin(string username, string password);
    bool isLegal(string username, string password);
};

#endif //CHARGINGMANAGEMENTSYSTEM_ADMIN_H


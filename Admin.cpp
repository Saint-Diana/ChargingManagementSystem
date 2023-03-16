//
// Created by 沈慧昌 on 2023/2/25.
//

#include "Admin.h"

Admin::Admin() {

}

Admin::Admin(string username, string password) {
    this->username = username;
    this->password = password;
}

bool Admin::isLegal(string username, string password) {
    if(username == this->username && password == this->password){
        return true;
    } else{
        return false;
    }
}

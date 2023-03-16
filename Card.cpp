//
// Created by 沈慧昌 on 2023/2/24.
//

#include "Card.h"

Card::Card(string id, string password, double money) {
    this->cardId = id;
    this->password = password;
    this->money = money;
    this->historyMoney = money;
    this->state = 1;
    this->frequency = 0;
    auto now = std::chrono::system_clock::now();
    this->time = std::chrono::system_clock::to_time_t(now);
    this->startTime = std::chrono::system_clock::to_time_t(now);
}

ostream &operator<<(ostream &os, const Card &card) {
    os << "cardId: " << card.cardId << " password: " << card.password << " money: " << card.money << " historyMoney: "
       << card.historyMoney << " state: " << card.state << " frequency: " << card.frequency << " time: " << card.time
            << " startTime: " << card.startTime;
    return os;
}

Card::Card() {
    this->state = 1;
    this->frequency = 0;
    auto now = std::chrono::system_clock::now();
    this->time = std::chrono::system_clock::to_time_t(now);
    this->startTime = std::chrono::system_clock::to_time_t(now);
}

//
// Created by 沈慧昌 on 2023/2/24.
//

#include "Machine.h"

Machine::Machine(int id) : id(id) {
    this->isFree = true;
    auto now = std::chrono::system_clock::now();
    this->time = std::chrono::system_clock::to_time_t(now);
}

Machine::Machine() {
    this->isFree = true;
    auto now = std::chrono::system_clock::now();
    this->time = std::chrono::system_clock::to_time_t(now);
}

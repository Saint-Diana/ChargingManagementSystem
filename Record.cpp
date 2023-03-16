//
// Created by 沈慧昌 on 2023/3/10.
//

#include "Record.h"

Record::Record(string cardId, time_t beginTime, time_t endTime, double money) : cardId(cardId), beginTime(beginTime),
                                                                             endTime(endTime), money(money) {}

Record::Record() {}

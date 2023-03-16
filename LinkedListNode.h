//
// Created by 沈慧昌 on 2023/3/16.
//

#ifndef CHARGINGMANAGEMENTSYSTEM_LINKEDLISTNODE_H
#define CHARGINGMANAGEMENTSYSTEM_LINKEDLISTNODE_H


template<typename T>
class LinkedListNode {
public:
    T data;
    LinkedListNode* next;
    explicit LinkedListNode(T data) : data(data), next(nullptr) {}
};


#endif //CHARGINGMANAGEMENTSYSTEM_LINKEDLISTNODE_H

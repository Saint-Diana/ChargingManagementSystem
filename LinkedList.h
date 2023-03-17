//
// Created by 沈慧昌 on 2023/3/16.
//

#ifndef CHARGINGMANAGEMENTSYSTEM_LINKEDLIST_H
#define CHARGINGMANAGEMENTSYSTEM_LINKEDLIST_H

#include <stdexcept>
#include <iostream>
#include "LinkedListNode.h"

using namespace std;

template<typename T>
class LinkedList {
public:
    LinkedListNode<T>* head;
    LinkedListNode<T>* tail;
    int size;
public:
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}

    LinkedList(int n, T value) : head(nullptr), tail(nullptr), size(0) {
        for (int i = 0; i < n; ++i) {
            add(value);
        }
    }

    void add(T data) {
        auto* node = new LinkedListNode<T>(data);
        if (head == nullptr) {
            head = tail = node;
        }else {
            tail->next = node;
            tail = node;
        }
        size++;
    }

    T& operator[](int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        LinkedListNode<T>* node = head;
        for (int i = 0; i < index; i++) {
            node = node->next;
        }
        return node->data;
    }

    void remove(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        LinkedListNode<T>* nodeToDelete = nullptr;
        if (index == 0) {
            nodeToDelete = head;
            head = head->next;
        }else {
            LinkedListNode<T>* prev = head;
            for (int i = 0; i < index - 1; i++) {
                prev = prev->next;
            }
            nodeToDelete = prev->next;
            prev->next = nodeToDelete->next;
        }
        if (tail == nodeToDelete) {
            tail = prev;
        }
        delete nodeToDelete;
        size--;
    }

    void set(int index, T data) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        LinkedListNode<T>* node = head;
        for (int i = 0; i < index; i++) {
            node = node->next;
        }
        node->data = data;
    }

    void print() const {
        for (LinkedListNode<T>* node = head; node != nullptr; node = node->next) {
            std::cout << node->data << " ";
        }
        std::cout << std::endl;
    }
};


#endif //CHARGINGMANAGEMENTSYSTEM_LINKEDLIST_H

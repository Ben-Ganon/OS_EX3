//
// Created by ben on 6/1/22.
//

#ifndef OS_EX3_BOUNDED_BUFFER_H
#define OS_EX3_BOUNDED_BUFFER_H

#include <semaphore.h>
#include <mutex>
#include <thread>
#include <iostream>
#include<pthread.h>
#include "queue"
#include "Article.h"

using namespace std;

class Bounded_Buffer {
    int id = 0;
    int capacity;
    sem_t full;
    sem_t empty;
    pthread_mutex_t mtx;
    queue<Article> q;
public:
    bool isActive;
    virtual ~Bounded_Buffer() {

    }


    Bounded_Buffer(int size){
        isActive = true;
        capacity = size;
        pthread_mutex_init(&mtx, NULL);
        sem_init(&empty, 0, size);
        sem_init(&full, 0, 0);
    }
    void insert(char * s) {
        int val;
        Article a = Article(s);
        sem_wait(&empty);
        pthread_mutex_lock(&mtx);
        q.push(a);
        pthread_mutex_unlock(&mtx);
        sem_post(&full);
    }
    void insert(Article a){
        sem_wait(&empty);
        pthread_mutex_lock(&mtx);
        q.push(a);
        pthread_mutex_unlock(&mtx);
        sem_post(&full);
    }
    string remove() {
        sem_wait(&full);
        pthread_mutex_lock(&mtx);
        Article a = q.front();
        q.pop();
        string s = a.content;
        pthread_mutex_unlock(&mtx);
        sem_post(&empty);
        return s;
    }

    int getId() const {
        return id;
    }

    void setId(int id) {
        Bounded_Buffer::id = id;
    }

    int getCapacity() const {
        return capacity;
    }

    void setCapacity(int capacity) {
        Bounded_Buffer::capacity = capacity;
    }

    const sem_t &getFull() const {
        return full;
    }

    void setFull(const sem_t &full) {
        Bounded_Buffer::full = full;
    }

    const sem_t &getEmpty() const {
        return empty;
    }

    void setEmpty(const sem_t &empty) {
        Bounded_Buffer::empty = empty;
    }

    const pthread_mutex_t &getMtx() const {
        return mtx;
    }

    void setMtx(const pthread_mutex_t &mtx) {
        Bounded_Buffer::mtx = mtx;
    }

    const queue<Article> &getQ() const {
        return q;
    }

    void setQ(const queue<Article> &q) {
        Bounded_Buffer::q = q;
    }

};


#endif //OS_EX3_BOUNDED_BUFFER_H

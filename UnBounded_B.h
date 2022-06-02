//
// Created by ben on 6/1/22.
//

#ifndef OS_EX3_UNBOUNDED_B_H
#define OS_EX3_UNBOUNDED_B_H


#include "Article.h"
#include <semaphore.h>
#include <mutex>
#include <thread>
#include <iostream>
#include<pthread.h>
#include "queue"
#include "Article.h"

using namespace std;

class UnBounded_B {
    int id = 0;
    sem_t full;
    pthread_mutex_t mtx;
    queue<string> q;
public:
    UnBounded_B(){
        pthread_mutex_init(&mtx, NULL);
        sem_init(&full, 0, 0);
    }
    void insert(char * s) {
        int val;
        Article a = Article(s);
        pthread_mutex_lock(&mtx);
        q.push(s);
        pthread_mutex_unlock(&mtx);
        sem_post(&full);
    }
    void insert(string a){
        pthread_mutex_lock(&mtx);
        q.push(a);
        pthread_mutex_unlock(&mtx);
        sem_post(&full);
    }
    string remove() {
        sem_wait(&full);
        pthread_mutex_lock(&mtx);
        string a = q.front();
        q.pop();
        pthread_mutex_unlock(&mtx);
        return a;
    }

    int getId() const {
        return id;
    }

    void setId(int id) {
        this->id = id;
    }
};


#endif //OS_EX3_UNBOUNDED_B_H

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
    queue<Article> q;
public:
    UnBounded_B(){
        pthread_mutex_init(&mtx, NULL);
        sem_init(&full, 0, 0);
    }
    void insert(char * s) {
        int val;
        Article a = Article(s);
        pthread_mutex_lock(&mtx);
        q.push(a);
        pthread_mutex_unlock(&mtx);
        sem_post(&full);
    }
    void insert(Article a){
        pthread_mutex_lock(&mtx);
        q.push(a);
        pthread_mutex_unlock(&mtx);
        sem_post(&full);
    }
    char * remove() {
        sem_wait(&full);
        pthread_mutex_lock(&mtx);
        Article a = q.front();
        q.pop();
        const char * s = a.content.c_str();
        pthread_mutex_unlock(&mtx);
        return const_cast<char *>(s);
    }

    int getId() const {
        return id;
    }

    void setId(int id) {
        this->id = id;
    }


    const sem_t &getFull() const {
        return full;
    }


    const pthread_mutex_t &getMtx() const {
        return mtx;
    }

    const queue<Article> &getQ() const {
        return q;
    }

};


#endif //OS_EX3_UNBOUNDED_B_H

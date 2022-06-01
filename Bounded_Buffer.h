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
    int id;
    int capacity;
    sem_t full;
    sem_t empty;
    mutex mtx;
    queue<Article> q;
public:
    Bounded_Buffer(int size){
        capacity = size;
        sem_init(&empty, 0, size);
        sem_init(&full, 0, 0);
    }
    void insert(char * s) {
        int val;
        Article a = Article(s);
        sem_wait(&empty);
        mtx.lock();
        q.push(a);
        mtx.unlock();
        sem_post(&full);
    }
    void insert(Article a){
        sem_wait(&empty);
        mtx.lock();
        q.push(a);
        mtx.unlock();
        sem_post(&full);
    }
    char * remove() {
        sem_wait(&full);
        mtx.lock();
        Article a = q.front();
        q.pop();
        const char * s = a.content.c_str();
        mtx.unlock();
        sem_post(&empty);
        return const_cast<char *>(s);
    }


};


#endif //OS_EX3_BOUNDED_BUFFER_H

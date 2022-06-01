//
// Created by ben on 6/1/22.
//

#ifndef OS_EX3_PRODUCER_H
#define OS_EX3_PRODUCER_H
#import <list>
#import "Article.h"
#include "Bounded_Buffer.h"
#import <vector>
#include <random>
#include <iostream>




class Producer {
    int id;
    int Qsize;
    deque<Article> articles;


public:
    int articleNum;

    Producer(int id, int size, int articleNum) {

        this->id = id;
        this->Qsize = size;
        this->articleNum = articleNum;
        srand((unsigned)time(0));
//        for (int i = 0; i < articleNum; ++i) {
//            articles.push_back(GenerateArticle(rand()%3));
//        }
    }


    Article Pop() {
        if(this->articles.empty())
            return Article(nullptr);
        Article a = this->articles.front();
        this->articles.pop_front();
    }
    void ShowArticles() {
        for(Article a : this->articles) {
            cout << "Producer: " << a.content << endl;
        }
        cout << "Total: " << endl;
//        cout << "  SPORTS: " << this->articleCounters[0] << endl;
//        cout << "  NEWS:  " << this->articleCounters[1] << endl;
//        cout << "  WEATHER:  " << this->articleCounters[2] << endl;
    }
};


#endif //OS_EX3_PRODUCER_H

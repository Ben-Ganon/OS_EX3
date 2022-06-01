//
// Created by ben on 6/1/22.
//

#ifndef OS_EX3_PRODUCER_H
#define OS_EX3_PRODUCER_H
#import <list>
#import "Article.h"
#import <vector>
#include <random>
#include <iostream>




class Producer {
    int id;
    int Qsize;
    int articleNum;
    vector<Article *> articles;
    int articleCounters[3] = {0, 0, 0};
    string articleTypes[3] = {"SPORTS", "NEWS", "WEATHER"};

public:
    Producer(int id, int size, int articleNum) {
        this->id = id;
        this->Qsize = size;
        this->articleNum = articleNum;
        articles.reserve(articleNum);
        srand((unsigned)time(0));
        for (int i = 0; i < articleNum; ++i) {
            articles.push_back(GenerateArticle(rand()%3));
        }
    }
    ~Producer(){
        for(Article* a : this->articles){
            delete a;
        }
    }
    Article* GenerateArticle(int randT) {

        string type = this->articleTypes[randT];
        auto ret = new Article(id, type, this->articleCounters[randT]);
        this->articleCounters[randT]+=1;
        return ret;

    }
    void ShowArticles() {
        for(Article* a : this->articles) {
            cout << "Producer: " << a->content << endl;
        }
        cout << "Total: " << endl;
        cout << "  SPORTS: " << this->articleCounters[0] << endl;
        cout << "  NEWS:  " << this->articleCounters[1] << endl;
        cout << "  WEATHER:  " << this->articleCounters[2] << endl;
    }
};


#endif //OS_EX3_PRODUCER_H

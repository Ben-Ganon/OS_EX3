//
// Created by ben on 6/1/22.
//

#ifndef OS_EX3_ARTICLE_H
#define OS_EX3_ARTICLE_H
#include <string>

using namespace std;
class Article {
public:
    string content;
public:

    Article(int producer, string type, int id) {
        content = "Producer: " + to_string(producer) + " Type: " + type + "  ID: " + to_string(id);
    }
    Article(char * s){
        if(s == nullptr)
            content = "";
        else content = string(s);
    }
    Article(string s){
        content = s;
    }
};


#endif //OS_EX3_ARTICLE_H

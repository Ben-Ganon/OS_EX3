#include <iostream>
#include <fstream>
#include <csignal>
#include "Producer.h"
#include "UnBounded_B.h"
#include <chrono>
#include <thread>

typedef struct producerInfo{
    int id;
    int productNum;
    int qSize;
}producerInfo;


using namespace std;

int coEditorQ;
vector<Bounded_Buffer> ProducerQs = vector<Bounded_Buffer>();
UnBounded_B Buff_Sports;
UnBounded_B Buff_News;
UnBounded_B Buff_Weather;
Bounded_Buffer* Co_Editor_Shared;

string articleTypes[3] = {"SPORTS", "NEWS", "WEATHER"};

void DispatcherT(){
    bool noProducerAlive = false;
    while(!noProducerAlive){
        noProducerAlive = true;
        for (int i = 0; i < ProducerQs.size(); i++) {
            Bounded_Buffer b = ProducerQs[i];
            if(!b.isActive)
                continue;
            string str = b.remove();
            Article article(str);
            if(article.content == "-1"){
                b.isActive = false;
                continue;
            }
            noProducerAlive = false;
            if(article.content.find("SPORTS")){
                cout << "Found sports article" << article.content << endl;
            } else if(article.content.find("NEWS")) {
                cout << "Found news article" << article.content << endl;
            }else if(article.content.find("WEATHER")) {
                cout << "Found weather article" << article.content << endl;
            }
        }
        if(noProducerAlive)
            return;
    }

}

void ProducerT(int id,  int qSize, int productNum) {
    srand((unsigned)time(0));
    for (int i = 0; i < productNum; ++i) {
        /*Article a = p.Pop ();
        if(a.content == "")
            return;*/
        string type = articleTypes[rand()%3];
        Article a(id, type, i);
        ProducerQs.at(id).insert(a);
        usleep(10000);
    }
    Article end("-1");
    ProducerQs.at(id).insert(end);
}

vector<producerInfo> readFile(char * dir){
    vector<producerInfo> prods;
    ifstream config(dir);
    string id;
    string numItems;
    string qSize;
    string space;
    while(config) {
        getline(config, id);
        getline(config, numItems);
        getline(config, qSize);
        getline(config, space);
        if(equal(numItems.begin(), numItems.end(),"")){
            coEditorQ = stoi(id);
            return prods;
        }else {
            producerInfo p = producerInfo{stoi(id), stoi(numItems), stoi(qSize)};
            prods.push_back(p);
        }
        id.clear();
        numItems.clear();
        qSize.clear();
    }
}


int main(int argc, char * argv[]) {
    vector<producerInfo> producerData= readFile(argv[1]);
    Co_Editor_Shared = new Bounded_Buffer(coEditorQ);
    vector<thread> producerThreads;
    for (producerInfo p : producerData) {
        Bounded_Buffer b (p.qSize);
        ProducerQs.push_back(b);
    }

    for(int i = 0; i < producerData.size(); i++) {
        producerInfo b = producerData[i];
        thread t(ProducerT, i, b.qSize, b.productNum);
        producerThreads.push_back(move(t));
    }
    thread Dispatcht(DispatcherT);
    
//    auto* p1 = new Producer(1, 5, 30);
//    p1->ShowArticles();
//    delete p1;
    for (int j = 0; j < producerThreads.size(); ++j) {
        producerThreads[j].join();
    }
    Dispatcht.join();
return 0;
}

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
vector<UnBounded_B> CoEditorQs{Buff_Sports, Buff_News, Buff_Weather};

Bounded_Buffer* Co_Editor_Shared;

string articleTypes[3] = {"SPORTS", "NEWS", "WEATHER"};

void DispatcherT(){
    bool noProducerAlive = false;
    while(!noProducerAlive){
        noProducerAlive = true;
        for (int i = 0; i < ProducerQs.size(); i++) {
            if(!ProducerQs[i].isActive)
                continue;
            string str = ProducerQs[i].remove();
            if(str== "-1"){
                ProducerQs[i].isActive = false;
                continue;
            }
            noProducerAlive = false;
            if(str.find("SPORTS")){
                CoEditorQs[0].insert(str);
            } else if(str.find("NEWS")) {
                CoEditorQs[1].insert(str);
            }else if(str.find("WEATHER")) {
                CoEditorQs[2].insert(str);
            }
        }
        if(noProducerAlive){
            string end = "-1";
            CoEditorQs[0].insert(end);
            CoEditorQs[1].insert(end);
            CoEditorQs[2].insert(end);
            return;
        }

    }

}

void CoEditorT(int type) {
    while(true) {
        string article = CoEditorQs[type].remove();
        if(article == "-1"){
            string end = "-1";
            Co_Editor_Shared->insert(end);
            return;
        }
        Co_Editor_Shared->insert(article);
    }

}

void ScreenManagerT() {
    int endCount = 0;
    while(true) {
        if(endCount == 3){
            delete Co_Editor_Shared;
            cout << "DONE" << endl;
            return;
        }
        string article = Co_Editor_Shared->remove();
        if(article == "-1"){
            endCount++;
            continue;
        }

        cout << article << endl;
    }
}

void ProducerT(int id,  int qSize, int productNum) {
    srand((unsigned)time(0));
    for (int i = 0; i < productNum; ++i) {
        string type = articleTypes[rand()%3];
        string article = "Producer " + to_string(id) + " TYPE: " + type + " ID: " + to_string(i);
        ProducerQs.at(id).insert(article);
        usleep(300000);
    }
    string end = "-1";
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
        thread t(ProducerT, b.id, b.qSize, b.productNum);
        producerThreads.push_back(move(t));
    }
    thread Dispatcht(DispatcherT);
    thread CoEditSport(CoEditorT, 0);
    thread CoEditNews(CoEditorT, 1);
    thread CoEditWeather(CoEditorT, 2);
    thread ScreenT(ScreenManagerT);
//    auto* p1 = new Producer(1, 5, 30);
//    p1->ShowArticles();
//    delete p1;
    for (int j = 0; j < producerThreads.size(); ++j) {
        producerThreads[j].join();
    }
    Dispatcht.join();
    CoEditSport.join();
    CoEditNews.join();
    CoEditWeather.join();
    ScreenT.join();
return 0;
}

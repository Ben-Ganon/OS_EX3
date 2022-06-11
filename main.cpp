#include <iostream>
#include <fstream>
#include <csignal>
#include <chrono>
#include <thread>
#include <semaphore.h>
#include <mutex>
#include <iostream>
#include<pthread.h>
#include <queue>
#include <unistd.h>

using namespace std;

class Bounded_Buffer {
    int id = 0;
    int capacity;
    sem_t full;
    sem_t empty;
    pthread_mutex_t mtx;
    queue<string> q;
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
    void insert(string s) {
        int val;
        sem_wait(&empty);
        pthread_mutex_lock(&mtx);
        q.push(s);
        pthread_mutex_unlock(&mtx);
        sem_post(&full);
    }

    string remove() {
        sem_wait(&full);
        pthread_mutex_lock(&mtx);
        string a = q.front();
        q.pop();
        pthread_mutex_unlock(&mtx);
        sem_post(&empty);
        return a;
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



};

class UnBounded_B {
    int id = 0;
    sem_t full;
    pthread_mutex_t mtx;
    queue<string> q;
public:
    UnBounded_B() {
        pthread_mutex_init(&mtx, NULL);
        sem_init(&full, 0, 0);
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
        usleep(300000);
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

void ProducerT(int id,  int qSize, int productNum, int typeCount) {
    int sportCount = 0, newsCount = 0, weatherCount = 0;
    int artId =0;
    for (int i = 0; i < productNum; ++i) {
        string type = articleTypes[typeCount%3];
        typeCount++;
        if(type == "SPORT"){
            artId = sportCount;
            sportCount++;
        } else if(type == "NEWS") {
            artId = newsCount;
            newsCount++;
        }else if(type == "WEATHER") {
            artId = weatherCount;
            weatherCount++;
        }
        string article = "Producer " + to_string(id) + " " + type + " " + to_string(artId);
        ProducerQs.at(id -1 ).insert(article);
        usleep(300000);
    }
    string end = "-1";
    ProducerQs.at(id -1).insert(end);
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
            producerInfo p = producerInfo{stoi(id) , stoi(numItems), stoi(qSize)};
            prods.push_back(p);
        }
        id.clear();
        numItems.clear();
        qSize.clear();
    }
    return prods;
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
        thread t(ProducerT, b.id, b.qSize, b.productNum, i%3);
        producerThreads.push_back(move(t));
    }
    thread Dispatcht(DispatcherT);
    thread CoEditSport(CoEditorT, 0);
    thread CoEditNews(CoEditorT, 1);
    thread CoEditWeather(CoEditorT, 2);
    thread ScreenT(ScreenManagerT);

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

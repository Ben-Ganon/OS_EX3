#include <iostream>
#include <fstream>
#include "Producer.h"
#include "UnBounded_B.h"

typedef struct producerInfo{
    int id;
    int productNum;
    int qSize;
}producerInfo;


using namespace std;

int coEditorQ;

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

vector<Bounded_Buffer> ProducerQs = vector<Bounded_Buffer>();
UnBounded_B Co_Editor_Sports();
int main(int argc, char * argv[]) {
    vector<producerInfo> producers= readFile(argv[1]);

    for (producerInfo p : producers) {
        Bounded_Buffer b (p.qSize);
        ProducerQs.push_back(b);
    }
    int i = 0;
    for(Bounded_Buffer b : ProducerQs) {
        cout << "ID: "<< i<< " capacity: "<< b.getCapacity() << endl;
        i++;
    }
//    auto* p1 = new Producer(1, 5, 30);
//    p1->ShowArticles();
//    delete p1;
    return 0;
}

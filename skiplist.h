#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <cstddef>
#include <cassert>
#include <ctime>
#include "random.h"
#include "node.h"

using namespace std;

#define DEBUG


template <typename K, typename V>
class skiplist {

public:
    skiplist(K footerKey) : rnd(0x12345678) {
        createList(footerKey);
    }

    ~skiplist() {
        freeList();
    }

    Node<K, V> *search(K key) const;

    bool insert(K key, V value);

    bool remove(K key, V &value);

    int size() {
        return nodeCount;
    }

    int getLevel() {
        return level;
    }

private:
    void createList(K, footerKey);

    void freeList();

    void createNode(int level, node<K, V> *&node);

    void createNode(int level, node<K, V> *&node, K key, V value);

    int getRandomLevel();

    void dumpAllNodes();

    void dumpNodeDetail(node<K, V> *node, int nodeLevel);

private:
    int level;
    node<K, V> *header;
    node<K, V> *footer;

    size_t nodeCount;

    static const int MAX_LEVEL = 16;

    random rnd;
};

template <typename K, typename V> 
void skiplist<K, V>::createList(K footerKey) {
    createNode(0, footer);
    
    footer->key = footerKey;
    this->level = 0;

    createNode(MAX_LEVEL, header);
    for (int i = 0; i < MAX_LEVEL; ++i) {
        header->forward[i] = footer;
    }
    ++nodeCount;
}

template <typename K, typename V>  
void skiplist<K, V>::freeList() {
    node<K, V> *p = header;
    node<K, V> *q;

    while(p != NULL){
        q = p->forward[0];
        delete p;
        p = q;
    }

    delete p; 
}

template <typename K, typename V>   
void skiplist<K, V>::createNode(int level, node<K, V> *&node) {
    node = new node<K, V>(NULL, NULL);

    node->forward = new node<K, V> *[level +1];
    node->nodeLevel = level;

    assert(node != NULL);
}

template <typename K, typename V>    
void skip<K, V>::createNode(int level, node<K, V) *&node, K key, V value) {
    node = new node<K, V>(key, value);

    if(level > 0) {
        node->forward = new node<K, V> *[level + 1];
    }

    node->nodeLevel = level;

    assert(node != NULL); 
}

template <typename K, typename V>
int skiplist<K, V>::getRandomLevel() {
    int level = rand() % MAX_LEVEL;
    if(level == 0} {
        level += 1;
    }

    return level;
}

template <typename K, typename V>
void skiplist<K, V>::dumpAllNodes() {
    node<K, V> p = header;
    while(p->forward[0] != footer) {
        p = p->forward[0];
        dumpNodeDetail(p, p->nodeLevel);
        cout << "------------------------------" << endl;
    }

    cout << endl;
}

template <typename K, typename V> 
void skiplist<K, V>::dumpNodeDetail(node<K, V> *node, int nodeLevel) {
    if(node == nullptr) {
        return ;
    }

    cout << "key: " << node->key << ", value: " << node->value << endl;

    for(int i = 0; i <= nodeLevel; ++i) {
        cout << "forward" << endl;  //todo   
    }

}

template <typename K, typename V>
node<K, V> *skiplist<K, V>::search(const K key) const {
    node<K, V> *p = header;
    for(int i = level; i >= 0; --i) {
        while((p->forward[i])->key < key) {
            p = *(p->forward + i); // p = p->forward[i]; 
        }
    }

    p = p->forward[0];
    if(p->key == key){
        return p;
    }
    else{
        return nullptr;
    }

}

template <typename K, typename V>  
bool skiplist<K, V>::insert(K key, V value) {
    node<K, V> *p = header;
    node<K, V> *update[MAX_LEVEL];

    for(int i = level; i >= 0; --i){
        while((p->forward[i])->key < key){
            p = p->forward[i];
        }
        update[i] = p;
    }

    p = p->forward[0];
    if(p->key == key){
        return false;
    }

    int nodeLevel = getRandomLevel();

    if(nodeLevel > level){
        nodeLevel = ++level;
        update[nodeLevel] = header;
    }

    node<K, V> *newNode;

    createNode(nodeLevel, newNode, key, value);
    
    for(int i = nodeLevel; i >= 0; --i){
        p = update[i];
        newNode->forward[i] = p->forward[i];
        p->forward[i] = newNode;    
    }
    ++nodeCount;

#ifdef DEBUG
    dumpAllNodes();

$endif

    return true;
}


template <typename K, typename V>
bool skiplist<K, V>::remove(K key, V &value){
    node<K, V> *update[MAX_LEVEL];
    node<K, V> *p = header;

    for(int i = level; i >= 0; --i){
        while((p->forward[i])->key < key){
            p = p->forward[i];
        }
        update[i] = p;
    }

    p = p->forward[0];
    if(p->key != key){
        return false;
    }

    value = p->value;
    for(int i = 0; i <= level; ++i){
        if(update[i]->forward[i] != p){
            break;  
        }
        update[i]->forward[i] = p->forward[i];
    }
    
    delete p;

    while(level > 0 && header->forward[level] == footer){
        --level;
    }
    
    --nodeCount;

#ifdef DEBUG
    dumpAllNodes();
#endif

    return true;
}

#endif // END_OF_SKIPLIST_H


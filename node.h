#ifndef NODE_H
#define NODE_H

//forward declaration
template <typename K, typename V>
class skiplist;

template <typename K, typename V>
class node{

    friend class skiplist<K, V>;
public:
    node() {}
    node(K k, V v);
    ~node();
    K getKey() const;
    V getValue() const;    

private:
    K key;
    V value;
    node<K, V> **forward;
    int nodeLevel;
};

template <typename K, typename V> 
node<K, V>::node(const K k, const V v){
    key = k;
    value = v;
}

template <typename K, typename V>  
node<K, V>::~node(){
    delete[]forward;
}

template <typename K, typename V>   
K node<K, V>::getKey() const {
    return key;
}

template <typename K, typename V>    
V node<K, V>::getValue() const {
    return value;
}

#endif // END_OF_NODE_H

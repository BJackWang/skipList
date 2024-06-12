
/**
 * @file skiplist.h
 * @author Baoj Wang (you@domain.com)
 * @brief the header file for the skiplist
 * @version 0.1
 * @date 2024-06-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>

std::string delimiter = ":";

/**
 * @brief  definition of Node
 * @note
 * @retval None
 */
template <typename K, typename V>
class Node {

public:
    Node() {}

    Node(K key, V value, int level);

    ~Node();

    K get_key() const;

    V get_value() const;

    void set_value(V);

    // the array of pointers to the next node of different levels
    Node<K, V> **forward;

    int nodeLevel;

private:
    K key;
    V value;
};

template <typename K, typename V>
Node<K, V>::Node(K key, V value, int level) {

    this->key = key;
    this->value = value;
    this->nodeLevel = level;

    // level + 1, because the array index is 0 based
    this->forward = new Node<K, V> *[level + 1];

    // fill the array with 0(NULL)
    memset(this->forward, 0, sizeof(Node<K, V> *) * (level + 1));
};

template <typename K, typename V>
Node<K, V>::~Node() {
    delete[] forward;
};

template <typename K, typename V>
K Node<K, V>::get_key() const {
    return key;
};

template <typename K, typename V>
V Node<K, V>::get_value() const {
    return value;
};

template <typename K, typename V>
void Node<K, V>::set_value(V value) {
    this->value = value;
};

/**
 * @brief  definition of SkipList
 * @note
 * @retval None
 */
template <typename K, typename V>
class SkipList {

private:
    int maxLevel;

    int SkipListLevel;

    int elementCount;

    Node<K, V> *header;

    // std::ofstream fileWriter;
    // std::ifstream fileReader;

private:
    bool is_valid_string(const std::string &str);
    void get_key_value_from_string(const std::string &str, std::string *key, std::string *value);

public:
    SkipList(int);
    ~SkipList();
    void clear(Node<K, V> *);

    int size();
    int get_random_level();

    Node<K, V> *create_node(K, V, int);

    bool search_element(K);

    bool insert_element(K, V);
    void delete_element(K);

    void display_list();

    // void dump_file();
    // void load_file();
};

/**
 * @brief  return the size of SkipLists
 * @note
 * @retval (int)size
 */
template <typename K, typename V>
int SkipList<K, V>::size() {
    return this->elementCount;
}

/**
 * @brief  return the randomLevel of newNode
 * @note
 * @retval (int)randomLevel
 */
template <typename K, typename V>
int SkipList<K, V>::get_random_level() {
    int level = 0;
    while (rand() % 2 && level < maxLevel) {
        level++;
    }
    return level;
}

/**
 * @brief  create newNode of the SkipList
 * @note
 * @param  key: the key of Node
 * @param  value: the value of Node
 * @param  level: the level of Node
 * @retval return Node<K, V>*
 */
template <typename K, typename V>
Node<K, V> *SkipList<K, V>::create_node(const K key, const V value, int level) {
    Node<K, V> *node = new Node<K, V>(key, value, level);
    return node;
};

/**
 * @brief  return the value of searchKey
 * @note
 * @param  key: searchKey
 * @retval true and print if existed or false if not exist
 */
template <typename K, typename V>
bool SkipList<K, V>::search_element(const K key) {

    Node<K, V> *node = this->header;

    for (int i = this->SkipListLevel; i >= 0; i--) {
        while (node->forward[i] != nullptr && node->forward[i]->get_key() < key) {
            node = node->forward[i];
        }
    }

    node = node->forward[0];

    if (node != nullptr && node->get_key() == key) {
        std::cout << "Found key: " << key << ",value: " << node->get_value() << std::endl;
        return false;
    }

    std::cout << "Not Found key: " << key << std::endl;
    return false;
}

/**
 * @brief  insert Node into the SkipList with <K key, V value>
 * @note
 * @param  key: typename K
 * @param  value: typename V
 * @retval false if existed(change the value) or true if inserted
 */
template <typename K, typename V>
bool SkipList<K, V>::insert_element(const K key, const V value) {

    Node<K, V> *update[this->maxLevel + 1];
    Node<K, V> *node = this->header;

    /**
     * @brief  search from the highest level of skipList
     * @note
     * @param  i:level from high to 0
     * @retval node:node->forward should be inserted node
     */
    for (int i = SkipListLevel; i >= 0; i--) {
        while (node->forward[i] != nullptr && node->forward[i]->get_key() < key) {
            node = node->forward[i];
        } // node->key < searchKey <= node->forward[i]->key
        update[i] = node;
    }

    node = node->forward[0];

    // if the node of K key has existed -> change value only
    if (node != nullptr && node->get_key() == key) {
        node->set_value(value);
        return false;
    }

    // if the node has not existed -> create and splice
    // or if the key of the node is not equal to the required-key
    if (node == nullptr || node->get_key() != key) {
        int level = this->get_random_level();
        if (level > this->SkipListLevel) {
            for (int i = this->SkipListLevel + 1; i <= level; i++) {
                update[i] = this->header;
            }
            this->SkipListLevel = level;
        } // if the level of newNode > curLevel of the SkipLists
        Node<K, V> *newNode = this->create_node(key, value, level);
        for (int i = 0; i <= level; i++) {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
        std::cout << "Successfully inserted key:" << key << ",value:" << value << std::endl;
        this->elementCount++;
    }
    return true;
};

/**
 * @brief  delete the node with K key
 * @note
 * @param  key: deleteKey
 * @retval return delete message according to the delete result
 */
template <typename K, typename V>
void SkipList<K, V>::delete_element(K key) {

    Node<K, V> *update[this->maxLevel + 1];
    Node<K, V> *node = this->header;

    for (int i = this->SkipListLevel; i >= 0; i--) {
        while (node->forward[i] && node->forward[i]->get_key() < key) {
            node = node->forward[i];
        }
        update[i] = node;
    }

    node = node->forward[0];
    if (node != nullptr && node->get_key() == key) {
        for (int i = 0; i <= this->SkipListLevel; i++) {
            if (update[i]->forward[i] != node) {
                break;
            } // node only have level between 0 and i

            update[i]->forward[i] = node->forward[i];
        } // delete from the bottom and update the linking between nodes

        // update the current SkipLists Level after deleting node with high level
        while (this->SkipListLevel > 0 && this->header->forward[this->SkipListLevel] == nullptr) {
            SkipListLevel--;
        }

        std::cout << "Successfully deleted key:" << key << std::endl;
        delete node;
        this->elementCount--;
    } else {
        std::cout << "Not found element with key:" << key << std::endl;
    }
    return;
}

/**
 * @brief  print the SkipLists from top to bottom
 * @note
 * @retval none
 */
template <typename K, typename V>
void SkipList<K, V>::display_list() {

    const int width = (int)log10(this->SkipListLevel) + 1;
    std::cout << "\n*****Skip Lists*****" << std::endl;
    for (int i = this->SkipListLevel; i >= 0; i--) {
        Node<K, V> *node = this->header->forward[i];
        std::cout << "Level " << std::left << std::setw(width) << i << ": ";
        while (node != nullptr) {
            std::cout << "[" << node->get_key() << ", " << node->get_value() << "] -> ";
            node = node->forward[i];
        }
        std::cout << "NULLPTR" << std::endl
                  << std::endl;
    }
    return;
}

/**
 * @brief  constuctor of SkipList
 * @note
 * @param  maxLevel: the max Level of SkipList
 * @retval constructed SkipList
 */
template <typename K, typename V>
SkipList<K, V>::SkipList(int maxLevel) {

    this->maxLevel = maxLevel;
    this->SkipListLevel = 0;
    this->elementCount = 0;

    K k;
    V v;
    this->header = new Node<K, V>(k, v, maxLevel);
}

/**
 * @brief  to be called by destructor to clear skipLists recursively
 * @note
 * @param  head: the head of SkipLists
 * @retval  none
 */
template <typename K, typename V>
void SkipList<K, V>::clear(Node<K, V> *head) {
    if (head->forward[0] != nullptr) {
        clear(head->forward[0]);
    }
    delete (head);
}

/**
 * @brief  definition of destructor
 * @note
 * @retval  none
 */
template <typename K, typename V>
SkipList<K, V>::~SkipList() {

    if (this->header->forward[0] != nullptr) {
        clear(this->header->forward[0]);
    }
    delete (this->header);
}

/**
 * @brief  judge the validaty of string
 * @note
 * @param  &str: to be judged str
 * @retval  true(valid) or false(invalid)
 */
template <typename K, typename V>
bool SkipList<K, V>::is_valid_string(const std::string &str) {

    if (str.empty()) {
        return false;
    }
    if (str.find(delimiter) == std::string::npos) {
        return false;
    }
    return true;
}

/**
 * @brief  split the string and find <K, V>
 * @note
 * @param  &str:to be splitted string
 * @param  key:
 * @param  value:
 * @retval  pair of <K, V>
 */
template <typename K, typename V>
void SkipList<K, V>::get_key_value_from_string(const std::string &str, std::string *key, std::string *value) {

    if (!is_valid_string(str)) {
        std::cout << "\nInvalid string!" << std::endl;
        return;
    }
    *key = str.substr(0, str.find(delimiter));
    *value = str.substr(str.find(delimiter) + 1, str.length());
}

// template <typename K, typename V>
// void SkipList<K, V>::dump_file() {
// }

// template <typename K, typename V>
// void SkipList<K, V>::load_file() {
// }
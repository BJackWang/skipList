#include "skiplist.h"
#include <iostream>

int main(void) {

    SkipList<int, std::string> skipList(6);
    skipList.insert_element(1, "hello world");
    skipList.insert_element(2, "first program");
    skipList.insert_element(3, "glad to read the paper");
    skipList.insert_element(5, "finish the code");
    skipList.insert_element(8, "today summer");
    skipList.insert_element(13, "2024/6/12");
    skipList.insert_element(21, "tomorrow exam");
    skipList.insert_element(34, "believe myself");

    std::cout << "\nskipList size:" << skipList.size() << std::endl;

    skipList.display_list();

    skipList.search_element(1);
    skipList.search_element(34);

    skipList.delete_element(8);
    skipList.delete_element(21);

    std::cout << "\nskipList size:" << skipList.size() << std::endl;

    skipList.display_list();
}
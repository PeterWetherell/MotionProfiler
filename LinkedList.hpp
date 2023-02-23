/* CSCI 200: Final Project
 *
 * Author: Peter Wetherell
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *     // list here any outside assistance you used/received while following the
 *     // CS@Mines Collaboration Policy and the Mines Academic Code of Honor
 *
 * This class deals with making a LinkedList and all of the associated functionality
 */

#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <exception>
#include <iostream>
#include <string>

template<typename T>
struct Node {
    T value;
    Node<T> *pNext;
    Node<T> *pPrev;
};

template<typename T> class LinkedList{
    public:
    LinkedList();
    ~LinkedList();
    T get(const int POS);
    int find(const T TARGET);
    void insert(const int POS, const T VALUE);
    T max();
    T min();
    void remove(const int POS);
    void set(const int POS, const T VALUE);
    unsigned int size();
    Node<T>* mMakeNodeForValue(const T VALUE);
    LinkedList<T>* sublist(int start, int end);
    LinkedList<T>* mergeList(LinkedList<T>* merge);
    void merge_sort(LinkedList<T>& pList);
    private:
    Node<T>* mpHead;
    Node<T>* mpTail;
    unsigned int mSize;
    void linked_list_add_value_to_front(Node<T>*& pHead, Node<T>*& pTail, const T VALUE);
    void linked_list_set_value_at_position(Node<T>* const P_head, Node<T>* const P_tail, const int POS, const T VALUE);
    void linked_list_remove_node_at_front(Node<T>*& pHead, Node<T>*& pTail);
    void linked_list_add_value_to_back(Node<T>*& pHead, Node<T>*& pTail, const T VALUE);
    void linked_list_remove_node_at_back(Node<T>*& pHead, Node<T>*& pTail);
    void linked_list_add_value_before_position(Node<T>*& pHead, Node<T>*& pTail, const int POS, const T VALUE);
    void linked_list_remove_node_at_position(Node<T>*& pHead, Node<T>*& pTail, const int POS);
};



template<typename T> Node<T>* LinkedList<T>::mMakeNodeForValue(const T VALUE){
    Node<T>* pNode = new Node<T>;
    pNode->value = VALUE;
    pNode->pNext = nullptr;
    pNode->pPrev = nullptr;
    return pNode;
}

template<typename T> void LinkedList<T>::linked_list_add_value_to_front(Node<T>*& pHead, Node<T>*& pTail, const T VALUE) {
    Node<T>* pNewNode = mMakeNodeForValue(VALUE);
    pNewNode->pNext = pHead;
    pNewNode->pPrev = NULL;
    if (pHead != NULL){
        pHead->pPrev = pNewNode;
        pHead = pNewNode;
    }      
    else{
        pHead = pNewNode;
        pTail = pNewNode;
    }
}

template<typename T> void LinkedList<T>::linked_list_set_value_at_position(Node<T>* const P_head, Node<T>* const P_tail, const int POS, const T VALUE) {
    Node<T>* pCurrNode = P_head;
    int counter = 0;
    if(POS < 0){
        return;
    }
    while (pCurrNode != NULL){
        if (counter == POS){
            pCurrNode->value = VALUE;
            return;
        }
        pCurrNode = pCurrNode->pNext;
        counter ++;
    }
}

template<typename T> void LinkedList<T>::linked_list_remove_node_at_front(Node<T>*& pHead, Node<T>*& pTail) {
    if(pHead == nullptr) {return;}                
    Node<T>*temp = pHead;
    pHead = pHead->pNext;
    if (pHead == nullptr){
        pTail = nullptr;
    }
    else{
        pHead->pPrev = nullptr;
    }
    delete temp;
}

template<typename T> void LinkedList<T>::linked_list_add_value_to_back(Node<T>*& pHead, Node<T>*& pTail, const T VALUE) {
    Node<T>* pNewNode = mMakeNodeForValue(VALUE);
    pNewNode->pPrev = pTail;
    pNewNode->pNext = NULL;
    if (pTail != NULL){
        pTail->pNext = pNewNode;
        pTail = pNewNode;
    }      
    else{
        pHead = pNewNode;
        pTail = pNewNode;
    }
}

template<typename T> void LinkedList<T>::linked_list_remove_node_at_back(Node<T>*& pHead, Node<T>*& pTail) {
    if(pTail == nullptr) return;
    Node<T>* temp = pTail;
    pTail = pTail->pPrev;
    if (pTail == nullptr){
        pHead = nullptr;
    }
    else{
        pTail->pNext = nullptr;
    }
    delete temp;
}

template<typename T> void LinkedList<T>::linked_list_add_value_before_position(Node<T>*& pHead, Node<T>*& pTail, const int POS, const T VALUE) {
    if(POS <= 0 || pHead == nullptr) {
        linked_list_add_value_to_front(pHead, pTail, VALUE);
    } else {
        Node<T>* pCurrNode = pHead;
        int counter = 0;
        while (pCurrNode != nullptr){
            if (POS == counter){
                break;
            }
            pCurrNode = pCurrNode->pNext;
            counter ++;
        }
        if(pCurrNode == nullptr) {
            linked_list_add_value_to_back(pHead, pTail, VALUE);
        } else {
            Node<T>* pNewNode = mMakeNodeForValue(VALUE);
            Node<T>* front = pCurrNode->pPrev;
            pNewNode->pNext = pCurrNode;
            pCurrNode->pPrev = pNewNode;
            pNewNode->pPrev = front;
            front->pNext = pNewNode;
        }
    }
}


template<typename T> void LinkedList<T>::linked_list_remove_node_at_position(Node<T>*& pHead, Node<T>*& pTail, const int POS) {
    if(pHead == nullptr) {
        return;
    } 
    else if(POS <= 0) {
        linked_list_remove_node_at_front(pHead, pTail);
    } else {
        Node<T>* pCurrNode = pHead;
        int counter = 0;
        while (pCurrNode != nullptr){
            if (POS == counter){
                break;
            }
            pCurrNode = pCurrNode->pNext;
            counter ++;
        }
        if(pCurrNode == nullptr || pCurrNode == pTail) {
            linked_list_remove_node_at_back(pHead, pTail);
        } else {
            Node<T>* temp = pCurrNode;

            Node<T>* first = pCurrNode->pPrev;
            Node<T>* second = pCurrNode->pNext;

            first->pNext = second;
            second->pPrev = first;

            delete temp;
        }
    }
}

//CODE HERE

template<typename T> LinkedList<T>::LinkedList(){
    mpHead = nullptr;
    mpTail = nullptr;
    mSize = 0;
}

template<typename T> LinkedList<T>::~LinkedList(){
    for (unsigned int i = 0; i < mSize; i ++){
        remove(0);
    }
}


template<typename T> T LinkedList<T>::get(const int POS){
    const Node<T>* pCURR_Node = mpHead;
    int counter = 0;
    // advance through list until end and count number of jumps
    
    if(POS < 0){
        throw std::out_of_range("negative POS");
    }
    else if ((unsigned)POS >= mSize){
        throw std::out_of_range("POS too big");
    }
    while (pCURR_Node != NULL){
        if (counter == POS){
            return pCURR_Node->value;
        }
        pCURR_Node = pCURR_Node->pNext;
        counter ++;
    }
    return mpHead->value;
}

template<typename T> int LinkedList<T>::find(const T TARGET){
    const Node<T>* pCURR_Node = mpHead;
    int counter = 0;
    // advance through list until end and target is found or end reached
    while (pCURR_Node != NULL){
        if (pCURR_Node->value == TARGET){
            return counter;
        }
        pCURR_Node = pCURR_Node->pNext;
        counter ++;
    }
    // return found position or -1 if end of list reached
    return -1;
}

template<typename T> void LinkedList<T>::insert(const int POS, const T VALUE){
    linked_list_add_value_before_position(mpHead,mpTail,POS, VALUE);
    mSize ++;
}

template<typename T> T LinkedList<T>::max(){
    const Node<T>* pCURR_Node = mpHead;
    
    // if list is empty
    if(pCURR_Node == nullptr) {
        // throw exception
        throw std::out_of_range("list is empty");
    }

    // traverse list tracking max value present

    T maxVal = pCURR_Node->value;
    
    while (pCURR_Node != NULL){
        if (pCURR_Node->value > maxVal){
            maxVal = pCURR_Node->value;
        }
        pCURR_Node = pCURR_Node->pNext;
    }

    return maxVal;
}

template<typename T> T LinkedList<T>::min(){
    const Node<T>* pCURR_Node = mpHead;
    
    // if list is empty
    if(pCURR_Node == nullptr) {
        // throw exception
        throw std::out_of_range("list is empty");
    }

    // traverse list tracking max value present

    T minVal = pCURR_Node->value;
    
    while (pCURR_Node != NULL){
        if (pCURR_Node->value < minVal){
            minVal = pCURR_Node->value;
        }
        pCURR_Node = pCURR_Node->pNext;
    }

    return minVal;
}

template<typename T> void LinkedList<T>::remove(const int POS){
    if (mpHead != nullptr){
        mSize --;
    }
    linked_list_remove_node_at_position(mpHead,mpTail,POS);
}

template<typename T> void LinkedList<T>::set(const int POS, const T VALUE){
    linked_list_set_value_at_position(mpHead, mpTail,POS, VALUE);
}

template<typename T> unsigned int LinkedList<T>::size(){
    return mSize;
}


template<typename T> LinkedList<T>* LinkedList<T>::sublist(int start, int end){
    LinkedList<T>* newList = new LinkedList<T>();
    for (int i = start; i < end; i ++){
        newList->insert(i,get(i));
    }
    return newList;
}

template<typename T> LinkedList<T>* LinkedList<T>::mergeList(LinkedList<T>* merge){
    unsigned int k = 0;
    for (unsigned int i = 0; i < mSize && k < merge->mSize; i ++){
        if (merge->get(k) < get(i)){
            insert(i,merge->get(k));
            k++;
        }
    }
    for (unsigned int j = k; j < merge->mSize; j ++){
        insert(mSize + merge->mSize,merge->get(j));
    }
    return this;
}

template<typename T> void LinkedList<T>::merge_sort(LinkedList<T>& pList) {
    if(pList.size() <= 1) {return;}
    LinkedList<T> *pLeft, *pRight;
    pLeft = pList.sublist(0, pList.size()/2);
    pRight = pList.sublist(pList.size()/2, pList.size());
    merge_sort(*pLeft);
    merge_sort(*pRight);
    pList = *pLeft->mergeList(pRight);
}

#endif
#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>
#include <functional>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a list.
 */
template<typename T>
class list {
protected:
    class node {
    public:
        /**
         * add data members and constructors & destructor
         */
        T *data;
        node *prev;
        node *next;
        
        // Sentinel node constructor (no data)
        node() : data(nullptr), prev(nullptr), next(nullptr) {}
        
        // Data node constructor
        node(const T &value) : data(new T(value)), prev(nullptr), next(nullptr) {}
        
        ~node() {
            if (data) {
                delete data;
            }
        }

    };

protected:
    /**
     * add data members for linked list as protected members
     */
    node *head;  // sentinel node before first element
    node *tail;  // sentinel node after last element
    size_t count;

    /**
     * insert node cur before node pos
     * return the inserted node cur
     */
    node *insert(node *pos, node *cur) {
        cur->prev = pos->prev;
        cur->next = pos;
        pos->prev->next = cur;
        pos->prev = cur;
        count++;
        return cur;
    }
    
    /**
     * remove node pos from list (no need to delete the node)
     * return the removed node pos
     */
    node *erase(node *pos) {
        pos->prev->next = pos->next;
        pos->next->prev = pos->prev;
        count--;
        return pos;
    }

public:
    class const_iterator;
    class iterator {
    friend class list;
    friend class const_iterator;
    private:
        /**
         * TODO add data members
         *   just add whatever you want.
         */
        node *ptr;
        const list *container;

    public:
        iterator() : ptr(nullptr), container(nullptr) {}
        iterator(node *p, const list *c) : ptr(p), container(c) {}
        
        /**
         * iter++
         */
        iterator operator++(int) {
            iterator temp = *this;
            ptr = ptr->next;
            return temp;
        }
        
        /**
         * ++iter
         */
        iterator & operator++() {
            ptr = ptr->next;
            return *this;
        }
        
        /**
         * iter--
         */
        iterator operator--(int) {
            iterator temp = *this;
            ptr = ptr->prev;
            return temp;
        }
        
        /**
         * --iter
         */
        iterator & operator--() {
            ptr = ptr->prev;
            return *this;
        }
        
        /**
         * TODO *it
         * remember to throw if iterator is invalid
         */
        T & operator *() const {
            if (ptr == nullptr || ptr->data == nullptr) {
                throw invalid_iterator();
            }
            return *(ptr->data);
        }
        
        /**
         * TODO it->field
         * remember to throw if iterator is invalid
         */
        T * operator ->() const {
            if (ptr == nullptr || ptr->data == nullptr) {
                throw invalid_iterator();
            }
            return ptr->data;
        }
        
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }
    };
    
    /**
     * TODO
     * has same function as iterator, just for a const object.
     * should be able to construct from an iterator.
     */
    class const_iterator {
    friend class list;
    friend class iterator;
    private:
        node *ptr;
        const list *container;
    
    public:
        const_iterator() : ptr(nullptr), container(nullptr) {}
        const_iterator(node *p, const list *c) : ptr(p), container(c) {}
        const_iterator(const iterator &it) : ptr(it.ptr), container(it.container) {}
        
        /**
         * iter++
         */
        const_iterator operator++(int) {
            const_iterator temp = *this;
            ptr = ptr->next;
            return temp;
        }
        
        /**
         * ++iter
         */
        const_iterator & operator++() {
            ptr = ptr->next;
            return *this;
        }
        
        /**
         * iter--
         */
        const_iterator operator--(int) {
            const_iterator temp = *this;
            ptr = ptr->prev;
            return temp;
        }
        
        /**
         * --iter
         */
        const_iterator & operator--() {
            ptr = ptr->prev;
            return *this;
        }
        
        /**
         * TODO *it
         * remember to throw if iterator is invalid
         */
        const T & operator *() const {
            if (ptr == nullptr || ptr->data == nullptr) {
                throw invalid_iterator();
            }
            return *(ptr->data);
        }
        
        /**
         * TODO it->field
         * remember to throw if iterator is invalid
         */
        const T * operator ->() const {
            if (ptr == nullptr || ptr->data == nullptr) {
                throw invalid_iterator();
            }
            return ptr->data;
        }
        
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }

    };
    
    /**
     * TODO Constructs
     * Atleast two: default constructor, copy constructor
     */
    list() {
        head = new node();
        tail = new node();
        head->next = tail;
        tail->prev = head;
        count = 0;
    }
    
    list(const list &other) {
        head = new node();
        tail = new node();
        head->next = tail;
        tail->prev = head;
        count = 0;
        
        for (node *cur = other.head->next; cur != other.tail; cur = cur->next) {
            push_back(*(cur->data));
        }
    }
    
    /**
     * TODO Destructor
     */
    virtual ~list() {
        clear();
        delete head;
        delete tail;
    }
    
    /**
     * TODO Assignment operator
     */
    list &operator=(const list &other) {
        if (this == &other) {
            return *this;
        }
        
        clear();
        for (node *cur = other.head->next; cur != other.tail; cur = cur->next) {
            push_back(*(cur->data));
        }
        
        return *this;
    }
    
    /**
     * access the first / last element
     * throw container_is_empty when the container is empty.
     */
    const T & front() const {
        if (count == 0) {
            throw container_is_empty();
        }
        return *(head->next->data);
    }
    
    const T & back() const {
        if (count == 0) {
            throw container_is_empty();
        }
        return *(tail->prev->data);
    }
    
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(head->next, this);
    }
    
    const_iterator cbegin() const {
        return const_iterator(head->next, this);
    }
    
    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(tail, this);
    }
    
    const_iterator cend() const {
        return const_iterator(tail, this);
    }
    
    /**
     * checks whether the container is empty.
     */
    virtual bool empty() const {
        return count == 0;
    }
    
    /**
     * returns the number of elements
     */
    virtual size_t size() const {
        return count;
    }

    /**
     * clears the contents
     */
    virtual void clear() {
        while (head->next != tail) {
            node *temp = head->next;
            erase(temp);
            delete temp;
        }
    }
    
    /**
     * insert value before pos (pos may be the end() iterator)
     * return an iterator pointing to the inserted value
     * throw if the iterator is invalid
     */
    virtual iterator insert(iterator pos, const T &value) {
        if (pos.container != this) {
            throw invalid_iterator();
        }
        node *new_node = new node(value);
        insert(pos.ptr, new_node);
        return iterator(new_node, this);
    }
    
    /**
     * remove the element at pos (the end() iterator is invalid)
     * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
     * throw if the container is empty, the iterator is invalid
     */
    virtual iterator erase(iterator pos) {
        if (count == 0) {
            throw container_is_empty();
        }
        if (pos.container != this || pos.ptr == head || pos.ptr == tail) {
            throw invalid_iterator();
        }
        
        node *next_node = pos.ptr->next;
        erase(pos.ptr);
        delete pos.ptr;
        return iterator(next_node, this);
    }
    
    /**
     * adds an element to the end
     */
    void push_back(const T &value) {
        node *new_node = new node(value);
        insert(tail, new_node);
    }
    
    /**
     * removes the last element
     * throw when the container is empty.
     */
    void pop_back() {
        if (count == 0) {
            throw container_is_empty();
        }
        node *last = tail->prev;
        erase(last);
        delete last;
    }
    
    /**
     * inserts an element to the beginning.
     */
    void push_front(const T &value) {
        node *new_node = new node(value);
        insert(head->next, new_node);
    }
    
    /**
     * removes the first element.
     * throw when the container is empty.
     */
    void pop_front() {
        if (count == 0) {
            throw container_is_empty();
        }
        node *first = head->next;
        erase(first);
        delete first;
    }
    
    /**
     * sort the values in ascending order with operator< of T
     */
    void sort() {
        if (count <= 1) return;
        
        // Convert to array for sorting
        T *arr = new T[count];
        size_t idx = 0;
        for (node *cur = head->next; cur != tail; cur = cur->next) {
            arr[idx++] = *(cur->data);
        }
        
        // Sort using provided algorithm
        sjtu::sort<T>(arr, arr + count, [](const T &a, const T &b) { return a < b; });
        
        // Copy back to list
        idx = 0;
        for (node *cur = head->next; cur != tail; cur = cur->next) {
            *(cur->data) = arr[idx++];
        }
        
        delete[] arr;
    }
    
    /**
     * merge two sorted lists into one (both in ascending order)
     * compare with operator< of T
     * container other becomes empty after the operation
     * for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
     * the order of equivalent elements of *this and other does not change.
     * no elements are copied or moved
     */
    void merge(list &other) {
        if (this == &other) return;
        
        node *cur1 = head->next;
        node *cur2 = other.head->next;
        
        while (cur2 != other.tail) {
            if (cur1 == tail || *(cur2->data) < *(cur1->data)) {
                // Take node from other and insert before cur1
                node *next2 = cur2->next;
                
                // Remove from other
                cur2->prev->next = cur2->next;
                cur2->next->prev = cur2->prev;
                other.count--;
                
                // Insert before cur1
                cur2->prev = cur1->prev;
                cur2->next = cur1;
                cur1->prev->next = cur2;
                cur1->prev = cur2;
                count++;
                
                cur2 = next2;
            } else {
                cur1 = cur1->next;
            }
        }
    }
    
    /**
     * reverse the order of the elements
     * no elements are copied or moved
     */
    void reverse() {
        if (count <= 1) return;
        
        node *cur = head->next;
        while (cur != tail) {
            node *temp = cur->next;
            cur->next = cur->prev;
            cur->prev = temp;
            cur = temp;
        }
        
        // Swap head and tail connections
        node *temp = head->next;
        head->next = tail->prev;
        tail->prev = temp;
        
        head->next->prev = head;
        tail->prev->next = tail;
    }
    
    /**
     * remove all consecutive duplicate elements from the container
     * only the first element in each group of equal elements is left
     * use operator== of T to compare the elements.
     */
    void unique() {
        if (count <= 1) return;
        
        node *cur = head->next;
        while (cur != tail && cur->next != tail) {
            if (*(cur->data) == *(cur->next->data)) {
                node *dup = cur->next;
                erase(dup);
                delete dup;
            } else {
                cur = cur->next;
            }
        }
    }
};

}

#endif //SJTU_LIST_HPP

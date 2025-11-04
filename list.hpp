#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>
#include <functional>

namespace sjtu {
template<typename T>
class list {
protected:
    class node {
    public:
        T *data;
        node *prev;
        node *next;
        
        node() : data(nullptr), prev(nullptr), next(nullptr) {}
        node(const T &value) : data(new T(value)), prev(nullptr), next(nullptr) {}
        
        ~node() {
            if (data) {
                delete data;
            }
        }
    };

protected:
    node *head;
    node *tail;
    size_t count;

    node *insert(node *pos, node *cur) {
        cur->prev = pos->prev;
        cur->next = pos;
        pos->prev->next = cur;
        pos->prev = cur;
        count++;
        return cur;
    }
    
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
        node *ptr;
        const list *container;

    public:
        iterator() : ptr(nullptr), container(nullptr) {}
        iterator(node *p, const list *c) : ptr(p), container(c) {}
        
        iterator operator++(int) {
            if (ptr == nullptr || container == nullptr) {
                throw invalid_iterator();
            }
            // Check if we're at end (tail)
            if (ptr == container->tail) {
                throw invalid_iterator();
            }
            iterator temp = *this;
            ptr = ptr->next;
            return temp;
        }
        
        iterator & operator++() {
            if (ptr == nullptr || container == nullptr) {
                throw invalid_iterator();
            }
            // Check if we're at end (tail)
            if (ptr == container->tail) {
                throw invalid_iterator();
            }
            ptr = ptr->next;
            return *this;
        }
        
        iterator operator--(int) {
            if (ptr == nullptr || container == nullptr) {
                throw invalid_iterator();
            }
            // Check if we're at begin
            if (ptr == container->head->next) {
                throw invalid_iterator();
            }
            iterator temp = *this;
            ptr = ptr->prev;
            return temp;
        }
        
        iterator & operator--() {
            if (ptr == nullptr || container == nullptr) {
                throw invalid_iterator();
            }
            // Check if we're at begin
            if (ptr == container->head->next) {
                throw invalid_iterator();
            }
            ptr = ptr->prev;
            return *this;
        }
        
        T & operator *() const {
            if (ptr == nullptr || ptr->data == nullptr) {
                throw invalid_iterator();
            }
            return *(ptr->data);
        }
        
        T * operator ->() const {
            if (ptr == nullptr || ptr->data == nullptr) {
                throw invalid_iterator();
            }
            return ptr->data;
        }
        
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }
    };
    
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
        
        const_iterator operator++(int) {
            if (ptr == nullptr || container == nullptr) {
                throw invalid_iterator();
            }
            if (ptr == container->tail) {
                throw invalid_iterator();
            }
            const_iterator temp = *this;
            ptr = ptr->next;
            return temp;
        }
        
        const_iterator & operator++() {
            if (ptr == nullptr || container == nullptr) {
                throw invalid_iterator();
            }
            if (ptr == container->tail) {
                throw invalid_iterator();
            }
            ptr = ptr->next;
            return *this;
        }
        
        const_iterator operator--(int) {
            if (ptr == nullptr || container == nullptr) {
                throw invalid_iterator();
            }
            if (ptr == container->head->next) {
                throw invalid_iterator();
            }
            const_iterator temp = *this;
            ptr = ptr->prev;
            return temp;
        }
        
        const_iterator & operator--() {
            if (ptr == nullptr || container == nullptr) {
                throw invalid_iterator();
            }
            if (ptr == container->head->next) {
                throw invalid_iterator();
            }
            ptr = ptr->prev;
            return *this;
        }
        
        const T & operator *() const {
            if (ptr == nullptr || ptr->data == nullptr) {
                throw invalid_iterator();
            }
            return *(ptr->data);
        }
        
        const T * operator ->() const {
            if (ptr == nullptr || ptr->data == nullptr) {
                throw invalid_iterator();
            }
            return ptr->data;
        }
        
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }
    };
    
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
    
    virtual ~list() {
        clear();
        delete head;
        delete tail;
    }
    
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
    
    iterator begin() {
        return iterator(head->next, this);
    }
    
    const_iterator cbegin() const {
        return const_iterator(head->next, this);
    }
    
    iterator end() {
        return iterator(tail, this);
    }
    
    const_iterator cend() const {
        return const_iterator(tail, this);
    }
    
    virtual bool empty() const {
        return count == 0;
    }
    
    virtual size_t size() const {
        return count;
    }

    virtual void clear() {
        while (head->next != tail) {
            node *temp = head->next;
            erase(temp);
            delete temp;
        }
    }
    
    virtual iterator insert(iterator pos, const T &value) {
        if (pos.container != this) {
            throw invalid_iterator();
        }
        node *new_node = new node(value);
        insert(pos.ptr, new_node);
        return iterator(new_node, this);
    }
    
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
    
    void push_back(const T &value) {
        node *new_node = new node(value);
        insert(tail, new_node);
    }
    
    void pop_back() {
        if (count == 0) {
            throw container_is_empty();
        }
        node *last = tail->prev;
        erase(last);
        delete last;
    }
    
    void push_front(const T &value) {
        node *new_node = new node(value);
        insert(head->next, new_node);
    }
    
    void pop_front() {
        if (count == 0) {
            throw container_is_empty();
        }
        node *first = head->next;
        erase(first);
        delete first;
    }
    
    void sort() {
        if (count <= 1) return;
        
        T **arr = new T*[count];
        size_t idx = 0;
        
        for (node *cur = head->next; cur != tail; cur = cur->next) {
            arr[idx++] = cur->data;
        }
        
        sjtu::sort<T*>(arr, arr + count, [](T* const &a, T* const &b) { return *a < *b; });
        
        idx = 0;
        for (node *cur = head->next; cur != tail; cur = cur->next) {
            cur->data = arr[idx++];
        }
        
        delete[] arr;
    }
    
    void merge(list &other) {
        if (this == &other) return;
        
        node *cur1 = head->next;
        node *cur2 = other.head->next;
        
        while (cur2 != other.tail) {
            if (cur1 == tail || *(cur2->data) < *(cur1->data)) {
                node *next2 = cur2->next;
                
                cur2->prev->next = cur2->next;
                cur2->next->prev = cur2->prev;
                other.count--;
                
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
    
    void reverse() {
        if (count <= 1) return;
        
        node *cur = head->next;
        while (cur != tail) {
            node *temp = cur->next;
            cur->next = cur->prev;
            cur->prev = temp;
            cur = temp;
        }
        
        node *temp = head->next;
        head->next = tail->prev;
        tail->prev = temp;
        
        head->next->prev = head;
        tail->prev->next = tail;
    }
    
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

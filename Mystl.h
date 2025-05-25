#pragma once

#include <cstddef>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <utility>
#include <memory>


template <typename T>
class myVector {
public:
    myVector() : data_(nullptr), sz_(0), cap_(0) {}
    explicit myVector(size_t n) : data_(nullptr), sz_(0), cap_(0) {
        reserve(n);
        sz_ = n;
        for (size_t i = 0; i < sz_; ++i) data_[i] = T();
    }
    ~myVector() { delete[] data_; }
    myVector(const myVector& other) : data_(nullptr), sz_(0), cap_(0) {
        reserve(other.cap_);
        sz_ = other.sz_;
        std::copy(other.data_, other.data_ + sz_, data_);
    }
    myVector& operator=(const myVector& other) {
        if (this != &other) {
            delete[] data_;
            reserve(other.cap_);
            sz_ = other.sz_;
            std::copy(other.data_, other.data_ + sz_, data_);
        }
        return *this;
    }
    T& operator[](size_t index) {
        if (index >= sz_) throw std::out_of_range("Index out of range");
        return data_[index];
    }
    const T& operator[](size_t index) const {
        if (index >= sz_) throw std::out_of_range("Index out of range");
        return data_[index];
    }
    void push_back(const T& value) {
        if (sz_ == cap_) reallocate(cap_ ? cap_ * 2 : 1);
        data_[sz_++] = value;
    }
    void pop_back() { if (sz_) --sz_; }
    void erase(size_t idx) {
        if (idx >= sz_) throw std::out_of_range("Index out of range");
        for (size_t i = idx + 1; i < sz_; ++i) data_[i - 1] = data_[i];
        --sz_;
    }
    void clear() { delete[] data_; data_ = nullptr; sz_ = cap_ = 0; }
    T& front() { if (!sz_) throw std::out_of_range("empty"); return data_[0]; }
    T& back() { if (!sz_) throw std::out_of_range("empty"); return data_[sz_ - 1]; }
    size_t size()   const { return sz_; }
    size_t capacity() const { return cap_; }
    bool empty()    const { return sz_ == 0; }
private:
    T* data_;
    size_t sz_, cap_;
    void reserve(size_t c) { if (c > cap_) reallocate(c); }
    void reallocate(size_t c) {
        T* nd = new T[c];
        size_t limit = (c < sz_) ? c : sz_;
        for (size_t i = 0; i < limit; ++i) nd[i] = std::move(data_[i]);
        sz_ = limit;
        delete[] data_;
        data_ = nd;
        cap_ = c;
    }
};


template<typename T>
struct Node {
    T val;
    Node<T>* next;
    Node(T v) : val(v), next(nullptr) {}
};


template<typename T>
class Mystack {
private:
    size_t Size;
    Node<T>* top_;
public:
    Mystack() : Size(0), top_(nullptr) {}
    ~Mystack() { clear(); }
    void push(T v) {
        Node<T>* n = new Node<T>(v);
        n->next = top_;
        top_ = n;
        ++Size;
    }
    void pop() {
        if (empty()) return;
        Node<T>* t = top_;
        top_ = top_->next;
        delete t;
        --Size;
    }
    T Top() {
        if (empty()) throw std::out_of_range("empty");
        return top_->val;
    }
    bool empty() const { return Size == 0; }
    size_t size() const { return Size; }
    void clear() { while (!empty()) pop(); }
};


template<typename T>
class MyQueue {
private:
    size_t Size;
    Node<T>* front_;
    Node<T>* tail_;
public:
    MyQueue() : Size(0), front_(nullptr), tail_(nullptr) {}
    ~MyQueue() { clear(); }
    void push(T v) {
        Node<T>* n = new Node<T>(v);
        if (!front_) front_ = tail_ = n;
        else { tail_->next = n; tail_ = n; }
        ++Size;
    }
    void pop() {
        if (!Size) return;
        Node<T>* t = front_;
        front_ = front_->next; delete t; --Size;
        if (!Size) front_ = tail_ = nullptr;
    }
    T Front() { if (!Size) throw std::out_of_range("empty"); return front_->val; }
    T Back() { if (!Size) throw std::out_of_range("empty"); return tail_->val; }
    size_t size() const { return Size; }
    bool empty() const { return Size == 0; }
    void clear() { while (!empty()) pop(); }
};


template<typename T>
struct DoubleNode {
    T val;
    DoubleNode<T>* left;
    DoubleNode<T>* right;
    DoubleNode(T v) : val(v), left(nullptr), right(nullptr) {}
};


template<typename T>
class MyList {
private:
    DoubleNode<T>* head;
    DoubleNode<T>* tail;
    size_t Size;
public:
    MyList() : head(nullptr), tail(nullptr), Size(0) {}
    ~MyList() { clear(); }
    void insert(size_t pos, T v) {
        pos = std::min(pos, Size);
        DoubleNode<T>* n = new DoubleNode<T>(v);
        if (!Size) head = tail = n;
        else if (!pos) { n->right = head; head->left = n; head = n; }
        else if (pos == Size) { tail->right = n; n->left = tail; tail = n; }
        else {
            DoubleNode<T>* it = head;
            for (size_t i = 0; i < pos - 1; ++i) it = it->right;
            n->right = it->right; n->left = it;
            it->right->left = n; it->right = n;
        }
        ++Size;
    }
    void erase(size_t pos) {
        if (pos >= Size) return;
        DoubleNode<T>* it = head;
        for (size_t i = 0; i < pos; ++i) it = it->right;
        if (it == head && it == tail) head = tail = nullptr;
        else if (it == head) { head = head->right; head->left = nullptr; }
        else if (it == tail) { tail = tail->left; tail->right = nullptr; }
        else { it->left->right = it->right; it->right->left = it->left; }
        delete it; --Size;
    }
    void pop_front() { if (Size) erase(0); }
    void pop_back() { if (Size) erase(Size - 1); }
    T front() { if (!Size) throw std::out_of_range("empty"); return head->val; }
    T back() { if (!Size) throw std::out_of_range("empty"); return tail->val; }
    bool empty() const { return Size == 0; }
    size_t size() const { return Size; }
    void clear() { while (!empty()) pop_front(); }
};


template<typename T>
class Mydeque {
private:
    DoubleNode<T>* head;
    DoubleNode<T>* tail;
    size_t Size;
public:
    Mydeque() : head(nullptr), tail(nullptr), Size(0) {}
    void push_head(T v) {
        DoubleNode<T>* n = new DoubleNode<T>(v);
        if (!head) head = tail = n;
        else { head->left = n; n->right = head; head = n; }
        ++Size;
    }
    void push_tail(T v) {
        DoubleNode<T>* n = new DoubleNode<T>(v);
        if (!head) head = tail = n;
        else { tail->right = n; n->left = tail; tail = n; }
        ++Size;
    }
    void pop_head() {
        if (!head) return;
        DoubleNode<T>* t = head; head = head->right;
        if (head) head->left = nullptr; else tail = nullptr;
        delete t; --Size;
    }
    void pop_tail() {
        if (!tail) return;
        DoubleNode<T>* t = tail; tail = tail->left;
        if (tail) tail->right = nullptr; else head = nullptr;
        delete t; --Size;
    }
    bool empty() const { return Size == 0; }
    size_t size() const { return Size; }
    T& gethead() { if (empty()) throw std::out_of_range("empty"); return head->val; }
    T& gettail() { if (empty()) throw std::out_of_range("empty"); return tail->val; }
};


template<typename T>
class My_priority_queue {
private:
    myVector<T> array;
    bool is_max_heap;
    bool cmp(T a, T b) { return is_max_heap ? a > b : a < b; }
    void up(size_t p) {
        while (p) {
            size_t pa = (p - 1) / 2;
            if (cmp(array[p], array[pa])) { std::swap(array[p], array[pa]); p = pa; }
            else break;
        }
    }
    void down(size_t p) {
        size_t n = array.size();
        while (true) {
            size_t l = 2 * p + 1, r = l + 1, t = p;
            if (l < n && cmp(array[l], array[t])) t = l;
            if (r < n && cmp(array[r], array[t])) t = r;
            if (t == p) break;
            std::swap(array[p], array[t]);
            p = t;
        }
    }
public:
    My_priority_queue(bool max_heap = true) : is_max_heap(max_heap) {}
    void push(const T& v) { array.push_back(v); up(array.size() - 1); }
    void pop() {
        if (array.empty()) return;
        array[0] = array.back();
        array.pop_back();
        if (!array.empty()) down(0);
    }
    T& top() { if (array.empty()) throw std::out_of_range("empty"); return array[0]; }
    bool empty() const { return array.empty(); }
    size_t size() const { return array.size(); }
};


template<class Key,
    class Compare = std::less<Key>,
    class Alloc = std::allocator<Key>>
    class rb_set {
    struct Node {
        Key    val;
        bool   red;               // true = RED, false = BLACK
        Node* parent, * left, * right;

        explicit Node(const Key& v)
            : val(v), red(true), parent(nullptr), left(nullptr), right(nullptr) {
        }
    };

    using alloc_traits = std::allocator_traits<Alloc>;

    using node_alloc = typename alloc_traits::template rebind_alloc<Node>;
    using node_alloc_traits = std::allocator_traits<node_alloc>;


    Node* root_; 
    Node* nil_;   
    std::size_t    size_;
    Compare        comp_;
    node_alloc     alloc_;

    template<typename... Args>
    Node* create_node(Args&&... args) {
        Node* p = node_alloc_traits::allocate(alloc_, 1);
        node_alloc_traits::construct(alloc_, p, std::forward<Args>(args)...);
        p->left = p->right = p->parent = nil_;
        return p;
    }
    void destroy_node(Node* p) {
        node_alloc_traits::destroy(alloc_, p);
        node_alloc_traits::deallocate(alloc_, p, 1);
    }

    
    bool is_red(Node* n) const { return n != nil_ && n->red; }

    void left_rotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != nil_) y->left->parent = x;
        y->parent = x->parent;

        if (x->parent == nil_)          root_ = y;
        else if (x == x->parent->left)  x->parent->left = y;
        else                            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }
    void right_rotate(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right != nil_) y->right->parent = x;
        y->parent = x->parent;

        if (x->parent == nil_)          root_ = y;
        else if (x == x->parent->right) x->parent->right = y;
        else                            x->parent->left = y;

        y->right = x;
        x->parent = y;
    }

    void fix_insert(Node* z) {
        while (is_red(z->parent)) {
            Node* gp = z->parent->parent;
            if (z->parent == gp->left) {
                Node* y = gp->right;
                if (is_red(y)) {                       // Case 1
                    z->parent->red = y->red = false;
                    gp->red = true;
                    z = gp;
                }
                else {
                    if (z == z->parent->right) {       // Case 2
                        z = z->parent;
                        left_rotate(z);
                    }
                    z->parent->red = false;            // Case 3
                    gp->red = true;
                    right_rotate(gp);
                }
            }
            else {                                   // Mirror
                Node* y = gp->left;
                if (is_red(y)) {
                    z->parent->red = y->red = false;
                    gp->red = true;
                    z = gp;
                }
                else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        right_rotate(z);
                    }
                    z->parent->red = false;
                    gp->red = true;
                    left_rotate(gp);
                }
            }
        }
        root_->red = false;
    }

    void fix_erase(Node* x) {
        while (x != root_ && !is_red(x)) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;
                if (is_red(w)) {                       // Case 1
                    w->red = false;
                    x->parent->red = true;
                    left_rotate(x->parent);
                    w = x->parent->right;
                }
                if (!is_red(w->left) && !is_red(w->right)) { // Case 2
                    w->red = true;
                    x = x->parent;
                }
                else {
                    if (!is_red(w->right)) {           // Case 3
                        if (w->left != nil_) w->left->red = false;
                        w->red = true;
                        right_rotate(w);
                        w = x->parent->right;
                    }
                    w->red = x->parent->red;           // Case 4
                    x->parent->red = false;
                    if (w->right != nil_) w->right->red = false;
                    left_rotate(x->parent);
                    x = root_;
                }
            }
            else {                                   // Mirror
                Node* w = x->parent->left;
                if (is_red(w)) {
                    w->red = false;
                    x->parent->red = true;
                    right_rotate(x->parent);
                    w = x->parent->left;
                }
                if (!is_red(w->right) && !is_red(w->left)) {
                    w->red = true;
                    x = x->parent;
                }
                else {
                    if (!is_red(w->left)) {
                        if (w->right != nil_) w->right->red = false;
                        w->red = true;
                        left_rotate(w);
                        w = x->parent->left;
                    }
                    w->red = x->parent->red;
                    x->parent->red = false;
                    if (w->left != nil_) w->left->red = false;
                    right_rotate(x->parent);
                    x = root_;
                }
            }
        }
        x->red = false;
    }


    void transplant(Node* u, Node* v) {
        if (u->parent == nil_)          root_ = v;
        else if (u == u->parent->left)  u->parent->left = v;
        else                            u->parent->right = v;
        v->parent = u->parent;
    }

    
    Node* minimum(Node* x) const {
        while (x->left != nil_) x = x->left;
        return x;
    }

    
    void clear_subtree(Node* n) {
        if (n == nil_) return;
        clear_subtree(n->left);
        clear_subtree(n->right);
        destroy_node(n);
    }

    public:
        
        explicit rb_set(const Compare& comp = Compare(),
            const Alloc& alloc = Alloc())
            : root_(nullptr), size_(0), comp_(comp), alloc_(alloc) {

            nil_ = node_alloc_traits::allocate(alloc_, 1);
            nil_->red = false;
            nil_->left = nil_->right = nil_->parent = nil_;

            root_ = nil_;
        }

        ~rb_set() { clear(); node_alloc_traits::deallocate(alloc_, nil_, 1); }

       
        bool empty()  const noexcept { return size_ == 0; }
        std::size_t size() const noexcept { return size_; }

        void clear() {
            clear_subtree(root_);
            root_ = nil_;
            size_ = 0;
        }

       
        bool find(const Key& key) const {
            Node* cur = root_;
            while (cur != nil_) {
                if (!comp_(key, cur->val) && !comp_(cur->val, key)) return true;
                cur = comp_(key, cur->val) ? cur->left : cur->right;
            }
            return false;
        }

       
        void insert(const Key& key) {
            Node* y = nil_;
            Node* x = root_;
            while (x != nil_) {               
                y = x;
                if (!comp_(key, x->val) && !comp_(x->val, key)) return; 
                x = comp_(key, x->val) ? x->left : x->right;
            }

            Node* z = create_node(key);
            z->parent = y;

            if (y == nil_)          root_ = z;       
            else if (comp_(key, y->val)) y->left = z;
            else                        y->right = z;

            ++size_;
            fix_insert(z);
        }

        void erase(const Key& key) {
            Node* z = root_;
            while (z != nil_) {
                if (!comp_(key, z->val) && !comp_(z->val, key)) break;
                z = comp_(key, z->val) ? z->left : z->right;
            }
            if (z == nil_) return;            

            Node* y = z;                       
            Node* x = nullptr;                  
            bool y_original_red = y->red;

            if (z->left == nil_) {
                x = z->right;
                transplant(z, z->right);
            }
            else if (z->right == nil_) {
                x = z->left;
                transplant(z, z->left);
            }
            else {
                y = minimum(z->right);       
                y_original_red = y->red;
                x = y->right;
                if (y->parent == z)
                    x->parent = y;
                else {
                    transplant(y, y->right);
                    y->right = z->right;
                    y->right->parent = y;
                }
                transplant(z, y);
                y->left = z->left;
                y->left->parent = y;
                y->red = z->red;
            }

            destroy_node(z);
            --size_;

            if (!y_original_red) fix_erase(x);
        }

        void print() const { inorder(root_); std::cout << '\n'; }

    private:
        void inorder(Node* n) const {
            if (n == nil_) return;
            inorder(n->left);
            std::cout << n->val << ' ';
            inorder(n->right);
        }
};





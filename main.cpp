#include <iostream>
#include <functional>
#include <algorithm>
#include <sstream>

template <typename T>
class stack {
public:
    virtual void add_first(T e) = 0;
    virtual void remove_first() = 0;
    virtual T get_first() = 0;
    virtual bool is_empty() const = 0;
    virtual ~stack() = default;
};

template <typename T>
class linked_list : public stack<T> {
    class node {
    public:
        T value;
        node* next = nullptr;
        node(T val, node* next) : value(val), next(next) {}

        ~node() {
            delete next;
        }
    };
    node* head = nullptr;
    node* tail = nullptr;
public:
    ~linked_list() override {
        delete head;
    }

    linked_list() = default;

    linked_list(const linked_list<T>& that) {
        for (T e : that) {
            this->add_last(e);
        }
    }

    linked_list(linked_list<T>&& that) noexcept {
        swap(*this, that);
    }

    linked_list<T>& operator=(linked_list<T> that) {
        swap(*this, that);
        return *this;
    }

    friend void swap(linked_list<T>& l, linked_list<T>& r) {
        using std::swap;
        swap(l.head, r.head);
        swap(l.tail, r.tail);
    }

    void add_first(T e) override {
        head = new node(e, head);
        if (tail == nullptr) {
            tail = head;
        }
    }

    void add_last(T e) {
        if (tail == nullptr) {
            add_first(e);
        } else {
            tail = tail->next = new node(e, nullptr);
        }
    }

    void remove_first() override {
        if (head != nullptr) {
            node* p = head;
            head = head->next;
            p->next = nullptr;
            delete p;
        }
    }

    T get_first() override {
        return head->value;
    }

    class cpp_iter {
        node* cur;
    public:
        cpp_iter(node* n) : cur(n) {}

        cpp_iter& operator++() {
            cur = cur->next;
            return *this;
        }
        bool operator== (const cpp_iter& that) const {
            return this->cur == that.cur;
        }
        bool operator!= (const cpp_iter& that) const {
            return this->cur != that.cur;
        }
        T operator*() const {
            return cur->value;
        }
        T* operator->() const {
            return &cur->value;
        }
    };

    cpp_iter begin() const {
        return cpp_iter(head);
    }

    cpp_iter end() const {
        return cpp_iter(nullptr);
    }

    bool is_empty() const override {
        return head == nullptr;
    }

    friend std::ostream& operator<<(std::ostream& os, const linked_list<T>& obj) {
        node* p = obj.head;
        while (p != nullptr) {
            os << p->value << " ";
            p = p->next;
        }
        os << std::endl;
        return os;
    }
    friend std::istream& operator>>(std::istream& is, linked_list<T>& obj) {
        using namespace std;
        while (true) {
            string s;
            is >> s;
            if (s == "end") {
                break;
            }
            stringstream ss(s);
            T v;
            ss >> v;
            obj.add_last(v);
        }
        return is;
    }
};

int main() {
    int i,j;
    std::cin >> i >> j;

    linked_list<int> l;
    l.add_last(i);
    l.add_last(j);

    std::cout << l;

    std::cin >> l;
    std::cout << l;
}

#include <iostream>
#include <functional>
#include <algorithm>
#include <sstream>

#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

template <typename T>
class stack {
public:
    virtual void add_first(T e) = 0;
    virtual void remove_first() = 0;
    virtual T get_first() = 0;
    virtual bool is_empty() = 0;
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

    std::recursive_mutex m;
    std::condition_variable_any element_added;

public:
    ~linked_list() override {
        delete head;
    }

    linked_list() = default;

    linked_list(linked_list<T>& that) {
        std::lock_guard lock(that.m);
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
        std::scoped_lock lock(l.m, r.m);

        swap(l.head, r.head);
        swap(l.tail, r.tail);

        l.element_added.notify_all();
        r.element_added.notify_all();
    }

    void add_first(T e) override {
        std::lock_guard lock(m);
        head = new node(e, head);
        if (tail == nullptr) {
            tail = head;
        }
        element_added.notify_one();
    }

    void add_last(T e) {
        std::lock_guard lock(m);
        if (tail == nullptr) {
            add_first(e);
        } else {
            tail = tail->next = new node(e, nullptr);
            element_added.notify_one();
        }
    }

    void remove_first() override {
        std::lock_guard lock(m);
        if (head != nullptr) {
            node* p = head;
            head = head->next;

            p->next = nullptr;
            delete p;

            if (head == nullptr) {
                tail = nullptr;
            }
        }
    }

    T get_first() override {
        std::lock_guard lock(m);
        return head->value;
    }

    T pop() {
        std::lock_guard lock(m);
        T res = get_first();
        remove_first();
        return res;
    }

private:
    bool no_more_elements = false;

public:
    struct stop_polling : std::exception {
        stop_polling() : std::exception("list stopped polling") {}
    };

    T poll() {
        std::unique_lock lock(m);
        while (is_empty()) {
            if (no_more_elements) {
                throw stop_polling();
            }
            element_added.wait(lock);
        }
        return pop();
    }

    void set_no_more_elements() {
        std::lock_guard lock(m);
        no_more_elements = true;
        element_added.notify_all();
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

    bool is_empty() override {
        std::lock_guard lock(m);
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

linked_list<int> l;
std::mutex cout_mutex;

void body(const std::string& name, int iter) {
    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << name << " has started" << std::endl;
    }

    try {
        for (int i = 0; i < iter; i++) {
            int n = l.poll();
            {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cout << name << ": " << n << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    } catch (const linked_list<int>::stop_polling& s) {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << name << " caught stop_polling: " << s.what() << std::endl;
    }
}

int main() {

    std::vector<std::thread> threads;
    for (int i = 0; i < 5; i++) {
        std::thread th(body, "t" + std::to_string(i), 100);
        threads.push_back(std::move(th));
    }
    //std::generate(threads.begin(), threads.end(),
    //              [k=0] () mutable{ return std::move(std::thread(body, "t" + std::to_string(k++), 100)); }
    //              );

    int n;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        l.add_last(i);
    }

    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << l;
    }

    std::cin >> l;

    l.set_no_more_elements();
    //std::cout << l;

    for (std::thread& th : threads) {
        th.join();
    }

    std::cout << l;
}

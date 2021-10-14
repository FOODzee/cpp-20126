#include <iostream>
#include <fstream>
#include <algorithm>

class queue {
public:
    virtual void add_last(int e) = 0;
    virtual void remove_first() = 0;
    virtual int get_first() = 0;
    virtual bool is_empty() const = 0;
};

template <typename T>
class stack {
public:
    virtual void add_first(T e) = 0;
    virtual void remove_first() = 0;
    virtual T get_first() = 0;
    virtual bool is_empty() const = 0;
};

template <typename T>
class linked_list : public stack<T> {
    class node {
    public:
        T value;
        node* next = nullptr;
        node(T val, node* next) : value(val), next(next) {}
    };
    node* head = nullptr;
public:
    void add_first(T e) override {
        head = new node(e, head);
    }

    void remove_first() override {
        if (head != nullptr) {
            // todo mem leak
            head = head->next;
        }
    }

    T get_first() override {
        return head->value;
    }

    bool is_empty() const override;

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
        T v;
        while (is >> v) {
            obj.add_first(v);
        }
        return is;
    }
};

template <typename R>
bool linked_list<R>::is_empty() const {
    return head == nullptr;
}

class arr_list : public queue {
    int first;
    int last;
    int size;
    int capacity;
    int* array;

    void arr_realloc(int add) {
        if (size + add > capacity) {
            int old_capacity = capacity;
            capacity *= 2;

            int* new_arr = new int[capacity];
            for (int i = 0; i < old_capacity; i++) {
                new_arr[i] = array[i];
            }
            delete[] array;
            array = new_arr;

            if (first > last) {
                for (int i = first; i < old_capacity; i++) {
                    array[i + old_capacity] = array[i];
                }
                first += old_capacity;
            }
        }
    }

public:

  arr_list(int cap) :
        first(),
        last(0),
        size(0),
        capacity(cap),
        array(new int[capacity]) {
  }

    arr_list() : arr_list(32) {}

    arr_list(const arr_list& orig) :
        first(orig.first),
        last(orig.last),
        size(orig.size),
        capacity(orig.capacity),
        array(new int[capacity]) {

        for (int i = 0; i < size; i++) {
            array[i] = orig.array[i];
        }
    }

    ~arr_list() {
        delete[] array;
    }

    friend void swap(arr_list& a1, arr_list& a2) {
        using std::swap;
        swap(a1.array, a2.array);
        swap(a1.size, a2.size);
        swap(a1.first, a2.first);
        swap(a1.last, a2.last);
        swap(a1.capacity, a2.capacity);
    }

    arr_list& operator=(arr_list rhs) {
        swap(*this, rhs);
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const arr_list& obj) {
        os << "arr_list.size: " << obj.size << std::endl;
        int i = 0;
        while (i < obj.size) {
            os << obj.array[i++];
        }
        return os;
    }
    friend std::istream& operator>>(std::istream& is, arr_list& obj) {
        int v;
        while (is >> v) {
            obj.add_last(v);
        }
        return is;
    }

    bool is_empty() const {
      return size == 0;
    }

    operator bool() const {
        return is_empty();
    }

    int get_size() {
        return size;
    }

    int get_first() {
        if (size == 0) {
            // TODO
        }
        return array[first];
    }

    void add_first(int el) {
        std::cout << "arr_list" << std::endl;
        if (size == 0) {
            array[first] = el;
            size++;
        } else {
            arr_realloc(1);
            first = (first + capacity - 1) % capacity;
            array[first] = el;
            size++;
        }
    }

    void add_last(int el) {
        if (size == 0) {
            array[last] = el;
            size++;
        } else {
            arr_realloc(1);
            last = (last + 1) % capacity;
            array[last] = el;
            size++;
        }
    }

    void remove_first() {
        if (size == 0) {
            return;
        }
        array[first] = 0;
        first = (first + 1) % capacity;
        size--;
    }
};

template<typename R>
void foo(R r) {
    r.print();
}

int main() {
    linked_list<int> li;
    linked_list<double> ld;
    linked_list<linked_list<int>> lli;

    using namespace std;

    cin >> li;
    cout << "li: " << li << std::endl;

    cin >> ld;
    cout << "ld: " << ld << std::endl;

    cin >> lli;
    cout << "lli: " << lli << std::endl;
}

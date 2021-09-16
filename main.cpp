#include <iostream>

class arr_list {
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

  bool is_empty() {
      return size == 0;
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

    int get_last() {
        if (size == 0) {
            // TODO
        }
        return array[last];
    }

    void add_first(int el) {
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

    void remove_last() {
        if (size == 0) {
            return;
        }
        array[last] = 0;
        last = (last + capacity - 1) % capacity;
        size--;
    }
};

void foo(arr_list& x) {
    int z;
    std::cin >> z;
    x.add_first(z);
    std::cout << x.get_first() << " " << x.get_size() << std::endl;
}

void swap(int* a, int* b) {
    int c = *a;
    *a = *b;
    *b = c;
}

void swap(int& a, int& b) {
    int c = a;
    a = b;
    b = c;
}

int main() {
    arr_list list;

    int x = 5 , y = 8;
    swap(&x, &y);
    swap(x, y);
    std::cin >> x >> y;
    list.add_first(x);
    list.add_last(y);

    std::cout << list.get_first() << " " << list.get_size() << std::endl;
    foo(list);
    std::cout << list.get_first() << " " << list.get_size() << std::endl;
}

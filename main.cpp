#include <stdlib.h>

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

int main() {
    arr_list * cpplist = new arr_list();

    arr_list cpplist2;
    arr_list cpplist32(32);

    cpplist2.get_first();

    delete cpplist;
}

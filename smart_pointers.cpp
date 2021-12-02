#include <iostream>

struct test {
    int i;
    test(int i) : i(i) {}

    ~test() {
        std::cout << "  ~test  " << i;
    }
};

int main2() {
    auto sp = std::make_shared<test>(1);
    {
        std::shared_ptr<test> spc = sp;

        std::cout << "15: " << sp.use_count() << std::endl;
    }
    std::cout << "17: " << sp.use_count() << std::endl;
    sp = std::make_shared<test>(2);
    std::cout << "17: " << sp.use_count() << std::endl;
    return 0;
}

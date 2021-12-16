#include <iostream>

struct base {
    int i;
    explicit base(int i) : i(i) {}

    virtual ~base() {
        std::cout << "  base  " << i;
    }
};

struct der : public base {
    int j;
    der(int i, int j) : base(i), j(j) {}
};

std::shared_ptr<base> create(bool b) {
    return !b ? std::make_shared<base>(42) : std::make_shared<der>(32, 42);
}

int main() {
    bool b;
    std::cin >> b;
    std::cout << b << std::endl;

    auto o = create(b);
    std::cout << o->i << std::endl;

    base* bp = o.get();

    der* dp2 = dynamic_cast<der*>(bp);
    if (dp2 != nullptr) {
        std::cout << dp2->j << std::endl;
    }

    auto d = std::dynamic_pointer_cast<der>(o);
    if (d) {
        std::cout << d->j << std::endl;
    }

    return 0;
}

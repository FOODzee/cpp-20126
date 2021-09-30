#include <iostream>
#include <fstream>
#include <algorithm>


class Person {
public:
    int age;

    virtual void print() {
        std::cout << "age: " << age << std::endl;
    }
};

class Student : public Person {
public:
    int group;

    void print() override {
        Person::print();
        std::cout << "group: " << group << std::endl;
    }
};

class Pervak : public Student {
public:
    int health;

    void print() override {
        Student::print();
        std::cout << "h: " << health << std::endl;
    }
};

void foo(Person& p) {
    p.print();
}

int main() {
    Person p;
    p.age = 12;
    Student s;
    s.age = 12;
    s.group = 13;

    Person& pp = p;
    Student& ss = s;
    Person& ps = s;

    ss.group = 8;
    ps.age = 7;

    Pervak per;
    per.health = 6;
    per.age = 50;
    per.group = 1;

    foo(s);
    foo(per);
    foo(p);
}
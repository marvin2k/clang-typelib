#ifndef TEST_HEADER_DATA_EXAMPLES_H
#define TEST_HEADER_DATA_EXAMPLES_H

// anonymous namespace, just for the sake of it?
namespace {
    struct ExampleData {
        int A;
        double B;
        char C;
    };
}


// packed does also works
struct PackedExample {
    uint8_t start;
    uint16_t next;
    int64_t after_next;
    int8_t more_of_it;
    float something;
    uint16_t and_the_last_one;
} __attribute__((packed));

// and nested ones
namespace nsX {
    struct simpleStruct {
        int member1;
        int member2;
    };
    struct nestedStruct {
        int member1;
        simpleStruct member2;
    };
}

struct MultiArrayStruct {
    int array[2][2];
};

namespace virtualClasses {

    class A {
        virtual void xxx() = 0;
        int aaa;
    };

    class B : public A {
        virtual void xxx() {
        };
        int lkjbl;
    };

}


#endif /*TEST_HEADER_DATA_EXAMPLES_H*/

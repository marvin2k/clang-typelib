#ifndef MINIMAL_HEADER_H
#define MINIMAL_HEADER_H

// minimal test-cases

#include <stdint.h>

namespace SomeSpace {

    struct ForwardDeclaration;

    // the simplest case in any world
    struct ExampleData {
        int A;
        double B;
        char C;
    };

    //  little bit more complex
    struct MoreExample {
        char Val;
        // array with odd offset and length
        char Arr[7];
        // nested
        struct ExampleData member;
    };

    class A {
        virtual void xxx() = 0;
        int aaa;
    };
    class B : public A {
        virtual void xxx() {
        };
        int lkjbl;
    };

    struct ForwardDeclaration {
        int content;
        int moreContent;
    };

    // packed does also works
    struct PackedExample {
        uint8_t start;
        uint16_t next;
        int64_t after_next;
        int8_t more_of_it;
        float something;
        uint16_t and_the_last_one;
    } __attribute__((packed));

}

#endif//MINIMAL_HEADER_H

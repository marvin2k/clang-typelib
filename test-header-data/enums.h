#ifndef TEST_DATA_HEADER_ENUMS_H
#define TEST_DATA_HEADER_ENUMS_H

namespace enums {

    enum E1 {
        E1_VAL1 = 42,
        E1_VAL2,
        E1_VAL3,
        E1_VAL4
    };

    enum E2 {
        E2_VAL1,
        E2_VAL2,
        E2_VAL3,
        E2_VAL4
    };
    typedef enum E2 E2_t;

    struct S1 {
        int A;
        enum E1 B;
    };

    struct S2 {
        enum E2 A;
    };

    class S3 {
        E2_t A;
    };
}

#endif /*TEST_DATA_HEADER_ENUMS_H*/


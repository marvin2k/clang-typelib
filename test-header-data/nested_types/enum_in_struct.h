#ifndef TEST_HEADER_DATA_ENUM_IN_STRUCT_H
#define TEST_HEADER_DATA_ENUM_IN_STRUCT_H

struct Root {
    enum Bla {
        VALUE = 5
    };
};
struct thing {
    Root::Bla stugg;
};

#endif /*TEST_HEADER_DATA_ENUM_IN_STRUCT_H*/

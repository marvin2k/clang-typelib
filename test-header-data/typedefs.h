#ifndef TEST_HEADER_DATA_TYPEDEFS_H
#define TEST_HEADER_DATA_TYPEDEFS_H

namespace ns {

    typedef int intTypedef;
    typedef intTypedef intTypedefTypedef;

    class aClass {
        int A;
    };

    typedef aClass aClassTypedef;

    struct test {
        char C;
        aClassTypedef A;
        intTypedef B;
        intTypedefTypedef B;
    };
}


#endif /*TEST_HEADER_DATA_TYPEDEFS_H*/

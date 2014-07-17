#ifndef TYPELIBBUILDER_H
#define TYPELIBBUILDER_H
#include <map>
#include <string>
#include <stdint.h>
#include <vector>

namespace clang {
class CXXRecordDecl;
}
class TypelibBuilder
{
    class TemporaryFieldType {
    public:
        std::string typeName;
        std::string fieldName;
        uint32_t offsetInBits;
    };
    
    class TemporaryType {
    public:
        std::string typeName;
        
        uint32_t sizeInBytes;
        
        std::vector<TemporaryFieldType> fields;
        
    };
public:
    void registerType(const clang::CXXRecordDecl *type);
    
private:
    std::map<std::string, TemporaryType> typeMap;
    
};

#endif // TYPELIBBUILDER_H

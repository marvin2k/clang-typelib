#include "TypelibBuilder.hpp"
#include "../../llvm/tools/clang/include/clang/AST/DeclCXX.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecordLayout.h"
#include "clang/AST/DeclCXX.h"
#include <iostream>

void TypelibBuilder::registerType(const clang::CXXRecordDecl* type)
{
    if(!type)
    {
        std::cerr << "Warning, got NULL Type" << std::endl;
        return;
    }

    if(type->isPolymorphic() || type->isAbstract())
    {
        std::cout << "Ignoring Type " << type->getQualifiedNameAsString() << " as it is polymorphic" << std::endl;
        return;
    }

    if(!type->getDefinition())
    {
        //ignore incomplete / forward declared types
        return;
    }

    const clang::ASTRecordLayout &typeLayout(type->getASTContext().getASTRecordLayout(type));

    //check if we allready know the type
    std::string typeName = type->getQualifiedNameAsString();
    
    
    if(typeMap.find(typeName) != typeMap.end())
    {
        //type allready known return;
        return;
    }
    
    TemporaryType tempType;
    tempType.typeName = typeName;
    tempType.sizeInBytes = typeLayout.getSize().getQuantity();

    for(clang::RecordDecl::field_iterator fit = type->field_begin(); fit != type->field_end(); fit++)
    {
        TemporaryFieldType fieldType;
        clang::SplitQualType T_split = fit->getType().getCanonicalType().split();
        fieldType.typeName = clang::QualType::getAsString(T_split);
        fieldType.fieldName = fit->getNameAsString();
        fieldType.offsetInBits = typeLayout.getFieldOffset(fit->getFieldIndex());
        
        tempType.fields.push_back(fieldType);
    }

    //put type into map
    typeMap[typeName] = tempType;
    
    
    std::cout << "Type " << tempType.typeName <<std::endl;
    std::cout << "  Size" << tempType.sizeInBytes << std::endl;

    if(!tempType.fields.empty())
    {
        std::cout << "  Members :" << std::endl;
    
        for(std::vector<TemporaryFieldType>::const_iterator it = tempType.fields.begin(); it != tempType.fields.end(); it++)
        {
            std::cout << "    FieldName " << it->fieldName << std::endl;
            std::cout << "    TypeName  " << it->typeName << std::endl;
            std::cout << "    Offset    " << it->offsetInBits << std::endl;
            std::cout << std::endl;
        }
    }    
}
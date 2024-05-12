#include <tuple>
#include <iostream>
#include <string>

#include "Reflection.h"

struct AggregateStruct
{
    int a = 0;
    const float m_member = 4;
};

struct AggregateStruct2
{
private:
    int a = 3;
    float m_megaSuperMember = 5.65f;
    std::string c = "val";

public:
    // custom-provided info about struct
    struct info
    {
        static constexpr auto value = std::tuple(
                &AggregateStruct2::a,
                &AggregateStruct2::m_megaSuperMember,
                &AggregateStruct2::c
        );
    };
};

struct AggregateStruct3
{
    int a = 3;
    const float m_superMember = 3.14f;
    std::string m_megaMember = "val";
};

int main()
{
    AggregateStruct3 s3;

    // compiletime meta
    auto meta = makeMetaInfo(s3);
    // runtime meta
    auto runtimeMeta = meta.asRuntime();

    runtimeMeta.members[2].setValue<std::string>("changed by runtime reflection");

    std::cout << "isconst: " << runtimeMeta.members[2].is_const << std::endl;

    auto& memberInfo = meta.get<2>();

    // memberInfo.value = "changed by compile-time reflection";

    std::cout <<
    "\tmember type name: " << memberInfo.unmangled_type_name << ",\n" <<
    "\tindex in class: " << memberInfo.index << ",\n" <<
    "\tvalue: " << memberInfo.value << ",\n" <<
    "\tis const: " << memberInfo.is_const << ",\n" <<
    "\tis volatile: " << memberInfo.is_volatile << ",\n" <<
    "\tis pointer: " << memberInfo.is_pointer << ",\n" <<
    "\tis reference: " << memberInfo.is_reference << ",\n" <<
    "\tmember name: " << memberInfo.unmangled_name <<
    std::endl;

    return 0;
}

#include <tuple>
#include <iostream>
#include <string>

#include "Reflection.h"
#include "MemberFunction.h"

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

    void fun()
    {

    }

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
private:
    int a = 3;
    const float m_superMember = 3.14f;
    std::string m_megaMember = "val";

    void fun(const std::string& str)
    {
        std::cout << str << std::endl;
    }

    member_func_t<&AggregateStruct3::fun> functionewrwr = bindFunction<&AggregateStruct3::fun>(this);

public:
    // custom-provided info about struct
    struct info
    {
        static constexpr auto value = std::tuple(
                &AggregateStruct3::a,
                &AggregateStruct3::m_superMember,
                &AggregateStruct3::m_megaMember,
                &AggregateStruct3::functionewrwr
        );
    };
};

int main()
{
    static constexpr AggregateStruct s3 { };

    // compiletime meta
    constexpr auto meta = makeMetaInfo(s3);
    // runtime meta
    auto runtimeMeta = meta.asRuntime();

    // runtimeMeta.findMember("m_megaMember")->setValue<std::string>("changed by runtime reflection");

    std::cout << "isconst: " << runtimeMeta.members[1].is_const << std::endl;

    auto& memberInfo = meta.get<0>();

    // memberInfo.value("hello from compile-time reflected function");
    // memberInfo.value = 2;

    runtimeMeta.members[0].setValue(6);

    /*auto* foundMember = runtimeMeta.findMember("functionewrwr");
    (*foundMember->getValue<MemberFunction<void(const std::string&)>>())("hello from runtime reflected function");*/

    std::cout <<
    "\tmember type name: " << memberInfo.unmangled_name << ",\n" <<
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

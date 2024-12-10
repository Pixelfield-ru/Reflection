#include <tuple>
#include <iostream>
#include <string>

#include "Reflection.h"
#include "MemberFunction.h"

struct AggregateStruct
{
    int a = 0;
    const float m_member = 4;
    std::string b = "";
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

constexpr bool strings_equal(char const * a, char const * b) {
    return std::string_view(a)==b;
}

consteval bool str(std::string_view a, std::string_view b)
{
    if(a == b)
    {
        return true;
    }
    else
    {
        return false;
    }
}

enum class MyEnum
{
    VALUE0,
    VALUE1,
    VALUE2,
    VALUE3
};

template<typename T>
struct ABC
{
    constexpr ABC(T t)
    {
        val = t;
    }

    virtual void d()
    {

    }

// private:
    T val;
};

int main()
{
    static constexpr AggregateStruct s3 { };

    // compiletime meta
    constexpr auto meta = makeMetaInfo(s3);
    // runtime meta
    auto runtimeMeta = meta.asRuntime();

    meta.iterateThroughMembers([](auto memberInfo) {
        std::cout << memberInfo.unmangled_name << std::endl;
    });

    if constexpr(std::remove_reference_t<decltype(makeMetaInfo<const AggregateStruct>(AggregateStruct { }).template get<2>())>::unmangled_name == "b")
    {
        std::cout << "AAAABBBB" << std::endl;
    }

    /*if constexpr(getUnMangledValueName<enum_reflect::getEnumMembers<MyEnum>()[3]>() == "MyEnum::VALUE2")
    {
        std::cout << "AAAABBBBcccccc" << std::endl;
    }*/

    constexpr auto enumMeta = enum_reflect::getEnumMembersInfo<MyEnum>();

    enum_reflect::iterateThroughEnumMembers<MyEnum>([](auto memberInfo) {
        std::cout << memberInfo.unmangled_name << std::endl;
    });

    std::cout << "Runtime name: " << enum_reflect::generateRuntimeNamesMarkup<MyEnum>()[2] << std::endl;

    /*enum_reflect::iterateThroughEnumMembers<MyEnum>([](auto memberValue) {
        using t = std::underlying_type_t<decltype(memberValue)>;
        // std::cout << static_cast<t>(memberValue) << std::endl;
        if constexpr(memberValue == MyEnum::VALUE0)
        {

        }
    });*/

    // std::cout << getEnumMembersCount<MyEnum>() << std::endl;

    //std::cout << enumSize<MyEnum>() << std::endl;
    // std::cout << getEnumMembersCount<MyEnum>() << std::endl;
    // std::cout << getEnumMembersCount<MyEnum>() << std::endl;

    // std::cout << std::remove_reference_t<decltype(makeMetaInfo<const AggregateStruct>(AggregateStruct { }).template get<0>())>::unmangled_name << std::endl;

    // static_assert(str("abc", "abc"), "strings are equal");

    // auto& mem = meta.getByName("a");

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

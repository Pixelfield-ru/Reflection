#include <tuple>
#include <iostream>
#include <string>

#include "Reflection.h"
#include "MemberFunction.h"

// aggregate
struct MyStruct0
{
    int m_myInt = 0;
    const float m_myFloat = 4;
};

struct MyStruct1
{
private:
    int a = 3;
    const float m_superMember = 3.14f;
    std::string m_megaMember = "val";

    void func(const std::string& str)
    {
        std::cout << str << std::endl;
    }

    member_func_t<&MyStruct1::func> m_myFunction = bindFunction<&MyStruct1::func>(this);

public:
    // custom-provided info about struct
    struct info
    {
        static constexpr auto value = std::tuple(
                &MyStruct1::a,
                &MyStruct1::m_superMember,
                &MyStruct1::m_megaMember,
                &MyStruct1::m_myFunction
        );
    };
};

enum class MyEnum
{
    VALUE0 = 10,
    VALUE1 = 15,
    VALUE2 = 20,
    VALUE3 = 21
};

template<typename T>
using get_class_meta_as_type = std::remove_reference_t<decltype(makeMetaInfo<const T>(T { }))>;

template<size_t Idx, typename ClassT>
using get_class_member = std::tuple_element_t<Idx, typename get_class_meta_as_type<ClassT>::as_members_tuple_t>;

template<basic_constexpr_string Str, typename ClassT>
using get_class_member_by_name = std::remove_reference_t<decltype(makeMetaInfo<const ClassT>(ClassT { }).template getByName<Str>())>;

void testCompileTimeMeta_IterateCompileTimeObject()
{
    std::cout << "--------------------- testCompileTimeMeta_CompileTimeObject" << std::endl;

    static constexpr MyStruct0 myStruct0 { };

    static constexpr auto meta = makeMetaInfo(myStruct0);

    meta.iterateThroughMembers([](auto memberInfo) {
        std::cout << std::format(
            "member: index: {}, name: '{}', typename: '{}', value: '{}', is const: {}, is volatile: {}, is pointer: {}, is reference: {}",
            memberInfo.index, memberInfo.demangled_name, memberInfo.demangled_type_name, memberInfo.value,
            memberInfo.is_const, memberInfo.is_volatile, memberInfo.is_pointer, memberInfo.is_reference) << std::endl;
    });
}

void testCompileTimeMeta_IterateRuntimeObject()
{
    std::cout << "--------------------- testCompileTimeMeta_RuntimeObject" << std::endl;

    MyStruct0 myStruct0 { };

    auto meta = makeMetaInfo(myStruct0);
    // meta.getByName<"m_myInt">().value = 10;

    meta.iterateThroughMembers([](auto memberInfo) {
        std::cout << std::format(
            "member: index: {}, name: '{}', typename: '{}', value: '{}', is const: {}, is volatile: {}, is pointer: {}, is reference: {}",
            memberInfo.index, memberInfo.demangled_name, memberInfo.demangled_type_name, memberInfo.value,
            memberInfo.is_const, memberInfo.is_volatile, memberInfo.is_pointer, memberInfo.is_reference) << std::endl;
    });
}

void testCompileTimeMeta_IterateRuntimeObjectWithCustomInfo()
{
    std::cout << "--------------------- testCompileTimeMeta_RuntimeObjectWithCustomInfo" << std::endl;

    MyStruct1 myStruct1 { };

    auto meta = makeMetaInfo(myStruct1);
    // meta.getByName<"m_megaMember">().value = "hello from introspection";

    meta.iterateThroughMembers([](auto memberInfo) {
        // skipping function =)
        if constexpr(decltype(memberInfo)::index != 3)
        {
            std::cout << std::format(
                "member: index: {}, name: '{}', typename: '{}', value: '{}', is const: {}, is volatile: {}, is pointer: {}, is reference: {}",
                memberInfo.index, memberInfo.demangled_name, memberInfo.demangled_type_name, memberInfo.value,
                memberInfo.is_const, memberInfo.is_volatile, memberInfo.is_pointer, memberInfo.is_reference) << std::endl;
        }
    });
}

void testCompileTimeMeta_GetSetValueByName()
{
    std::cout << "--------------------- testCompileTimeMeta_GetSetValueByName" << std::endl;

    MyStruct0 myStruct0 { };

    auto meta = makeMetaInfo(myStruct0);

    std::cout << "setting m_myInt value to '8'..." << std::endl;
    meta.getByName<"m_myInt">().value = 8;
    std::cout << "m_myInt value: " << meta.getByName<"m_myInt">().value << std::endl;
}

void testCompileTimeMeta_GetSetValueByIndex()
{
    std::cout << "--------------------- testCompileTimeMeta_GetSetValueByIndex" << std::endl;

    MyStruct0 myStruct0 { };

    auto meta = makeMetaInfo(myStruct0);

    std::cout << "setting member with index 0 ('m_myInt') value to '8'..." << std::endl;
    meta.get<0>().value = 8;
    std::cout << "m_myInt value: " << meta.get<0>().value << std::endl;
}

void testCompileTimeMeta_FunctionCall()
{
    std::cout << "--------------------- testCompileTimeMeta_FunctionCall" << std::endl;

    MyStruct1 myStruct1 { };

    auto meta = makeMetaInfo(myStruct1);
    meta.getByName<"m_myFunction">().value("hello from introspection");
}


// ==================================================================

void testRunTimeMeta_GetSetValueByName()
{
    std::cout << "--------------------- testRunTimeMeta_GetSetValueByName" << std::endl;

    MyStruct0 myStruct0 { };

    auto meta = makeMetaInfo(myStruct0).asRuntime();

    std::cout << "setting m_myInt value to '8'..." << std::endl;
    meta.findMember("m_myInt")->setValue<int>(8);
    std::cout << "m_myInt value: " << *meta.findMember("m_myInt")->getValue<int>() << std::endl;
}

void testRunTimeMeta_GetSetValueByIndex()
{
    std::cout << "--------------------- testRunTimeMeta_GetSetValueByIndex" << std::endl;

    MyStruct0 myStruct0 { };

    auto meta = makeMetaInfo(myStruct0).asRuntime();

    std::cout << "setting member with index 0 ('m_myInt') value to '8'..." << std::endl;
    meta.members[0].setValue<int>(8);
    std::cout << "m_myInt value: " << *meta.members[0].getValue<int>() << std::endl;
}

void testRunTimeMeta_FunctionCall()
{
    std::cout << "--------------------- testRunTimeMeta_FunctionCall" << std::endl;

    MyStruct1 myStruct1 { };

    auto meta = makeMetaInfo(myStruct1).asRuntime();
    (*meta.findMember("m_myFunction")->getValue<MemberFunction<void(const std::string&)>>())("hello from introspection");
}

void testEnumReflect_Iterate()
{
    std::cout << "--------------------- testEnumReflect_Iterate" << std::endl;

    enum_reflect::iterateThroughEnumMembers<MyEnum>([](auto memberInfo) {
        std::cout << std::format("enum member index: {}, value: '{}', name: '{}'", memberInfo.index, std::to_underlying(memberInfo.value), memberInfo.demangled_name) << std::endl;
    });
}

void testEnumReflect_Get0Member()
{
    std::cout << "--------------------- testEnumReflect_Get0Member" << std::endl;

    static constexpr auto meta = enum_reflect::getEnumMembersInfo<MyEnum>();
    static constexpr auto firstMember = std::get<0>(meta);

    std::cout << std::format("enum member index: {}, value: '{}', name: '{}'", firstMember.index, std::to_underlying(firstMember.value), firstMember.demangled_name) << std::endl;
}

void testEnumReflect_Count()
{
    std::cout << "--------------------- testEnumReflect_Count" << std::endl;

    static constexpr size_t count = enum_reflect::getEnumMembersCount<MyEnum>();

    std::cout << "enum values count: " << count << std::endl;
}

int main()
{
    testCompileTimeMeta_IterateCompileTimeObject();
    testCompileTimeMeta_IterateRuntimeObject();
    testCompileTimeMeta_IterateRuntimeObjectWithCustomInfo();
    testCompileTimeMeta_GetSetValueByName();
    testCompileTimeMeta_GetSetValueByIndex();
    testCompileTimeMeta_FunctionCall();
    testRunTimeMeta_GetSetValueByName();
    testRunTimeMeta_GetSetValueByIndex();
    testRunTimeMeta_FunctionCall();

    std::cout << "======================================================" << std::endl;

    static constexpr auto m_myFloatMember_name = get_class_member_by_name<"m_myFloat", MyStruct0>::demangled_name;

    std::cout << "m_myFloatMember_name: " << m_myFloatMember_name << std::endl;

    using meta_as_type = get_class_meta_as_type<MyStruct0>;

    std::cout << "member with index 0 from meta as type: " << std::remove_reference_t<decltype(meta_as_type { }.get<0>())>::demangled_name << std::endl;

    std::cout << "======================================================" << std::endl;

    testEnumReflect_Iterate();
    testEnumReflect_Get0Member();
    testEnumReflect_Count();

    return 0;
}

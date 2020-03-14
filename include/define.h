#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <functional>

typedef std::function<void(const char*)> CallbackFun;

struct Person {
    std::string head_image_path;
    std::string id;
    std::string name;
};

struct SpecialPerson {
    Person person;
    std::string remark;
};

struct PassingInfo {
    Person person;
    std::string time;
};

struct Exception {
    SpecialPerson special_person;
    std::string time;
};

#endif // _DEFINE_H_

#ifndef INFOPROCESS_H
#define INFOPROCESS_H

#include <set>
#include <map>
#include <string>

#include "define.h"

enum PersonInfo {
    SPECIAL_PERSON = 0,
    COMMUNITY_PERSON = 1,
    UNKNOWN_PERSON = 3,
};

class InfoProcess
{
public:
    InfoProcess();

    void update();
    PersonInfo find(std::string id);

    Person getPerson(std::string id);
    SpecialPerson getSpecialPerson(std::string id);
private:
    void init();

    void loadCommunityPersonnelData();
    void loadSpecialPersonnelData();

    std::string community_personnel_data_file_;
    std::string special_personnel_data_file_;

    std::set<std::string> community_personnel_set_;
    std::set<std::string> special_personnel_set_;

    std::map<std::string, Person> community_person_map_;
    std::map<std::string, SpecialPerson> special_person_map_;
};

#endif // INFOPROCESS_H

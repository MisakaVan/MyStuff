//
// Created by Misakavan on 2023/7/6.
//

#include <algorithm>
#include <set>
#include <map>
#include <tuple>
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>
#include "utilities.h"
#include "SkillCollection.h"

using std::any_of;
using std::pair;
using std::set;
using std::vector;
using std::unique_ptr;
using std::tuple;
using std::get;


/// Helper struct for std::algorithm,
/// a callable struct that predicates whether %other% is equal to the given %some%
/// \tparam T
template<typename T>
struct equalTo
{
    explicit equalTo(const T &some) : _some(some) {};

    bool operator()(const T &other)
    {
        return _some == other;
    }

    T _some;
};


int elementDamagePercent(element eleAttack, element eleDefence)
{
    static const vector<vector<int>> percentTable{
            {100, 100, 100, 100, 100},   // NORMAL as ele_attack
            {100, 50,  50,  200, 100},   // DENDRO as ele_attack
            {100, 200, 50,  50,  100},   // PYRO as ele_attack
            {100, 50,  200, 50,  100},   // HYDRO as ele_attack
            {100, 50,  100, 200, 50}     // ELECTRO as ele_attack
    };
    auto idx1 = toUType(eleAttack);
    auto idx2 = toUType(eleDefence);
    return percentTable.at(idx1).at(idx2);
}

bool isPriorTo(element eleAttack, element eleDefence)
{
    return elementDamagePercent(eleAttack, eleDefence) > 100;
}

[[maybe_unused]] std::unique_ptr<Skill> makeSkill_DEPRECATED(const Skill::nameType &skillName)
{
    using powerT = int;
    using priorityT = int;
    using skillInfo = std::tuple<element, powerT, priorityT>;
    static const std::map<Skill::nameType, skillInfo> standardSkills{
            {"Tackle", {element::normal, 20, 0}},
            {"Leaf",   {element::dendro, 20, 0}},
            {"Flame",  {element::pyro,   20, 0}},
            {"Stream", {element::hydro,  20, 0}},
    };
    auto p = standardSkills.find(skillName);
    if (p == standardSkills.cend()) {
        // This should not happen
        std::clog << "Trying to initialize a skillCollection named " << skillName
                  << " , which is not an defined standard skillCollection.\n";
        return nullptr;
    }
    const auto &pTuple = p->second;
    auto element = get<0>(pTuple);
    auto power = get<1>(pTuple);
    auto priority = get<2>(pTuple);

    return std::make_unique<AttackSkill>(skillName, element, power, priority);
}

std::unique_ptr<Pet> makePet(const Pet::nameType &petName)
{
    using hpT = int;
    using atkT = int;
    using defT = int;
    using spdT = int;
    using skillTs = vector<skill_T>;
    using petInfo = tuple<element, hpT, atkT, defT, spdT, skillTs>;
    static const std::map<Pet::nameType, petInfo> standardPets{
            {"W", {element::dendro,  110, 10, 10, 10, {skill_T::tackle, skill_T::leaf,   skill_T::synthesis}}},
            {"L", {element::pyro,    100, 12, 10, 11, {skill_T::claw,   skill_T::flame,  skill_T::sunny}}},
            {"G", {element::hydro,   100, 10, 14, 9,  {skill_T::tackle, skill_T::stream, skill_T::rainy}}},
            {"Q", {element::electro, 90,  10, 10, 12, {skill_T::quick,  skill_T::volt,   skill_T::thunder}}},
            {"D", {element::normal,  120, 9,  8,  7,  {skill_T::slap,   skill_T::sing,   skill_T::paint}}}
    };

    auto p = standardPets.find(petName);
    if (p == standardPets.cend()) {
        // This should not happen
        return nullptr;
    }
    const auto &info = p->second;
    auto element = get<0>(info);
    auto hp/*int*/ = get<1>(info);
    auto atk/*int*/ = get<2>(info);
    auto def/*int*/ = get<3>(info);
    auto spd/*int*/ = get<4>(info);
    vector<skill_T> skills(get<5>(info));

    return std::make_unique<Pet>(
            petName, element, hp, atk, def, spd, skills);
}

std::ostream &operator<<(std::ostream &os, const Skill &entity)
{
    return os << entity.name();
}

int readInt(const std::string &prompt, const set<int> &choices, std::istream &is, std::ostream &os)
{
    if (choices.empty()) {
        //   This should not happen.
        //   As far as this project is concerned,
        // -1 is not legal for any user input.
        return -1;
    }
    int ret = -1;
    do {
        os << prompt;
        is >> ret;
    } while (choices.find(ret) == choices.cend());
    return ret;
}

std::unique_ptr<Intention> makeSkillIntention(Player *player, Pet *actingPet, Skill *skillToUse)
{
    return std::make_unique<Intention>(
            player->type(),
            intention_T::skill,
            skillToUse->priority(),
            Intention::infoPack(actingPet, skillToUse, potion_T::null)
    );
}

std::unique_ptr<Intention> makeChangeIntention(Player *player, Pet *newPet)
{
    return std::make_unique<Intention>(
            player->type(),
            intention_T::change,
            PRIORITY_CHANGE,
            Intention::infoPack(newPet, nullptr, potion_T::null)
    );
}

std::unique_ptr<Intention> makePotionIntention(Player *player, Pet *targetPet, potion_T potionType)
{
    return std::make_unique<Intention>(
            player->type(),
            intention_T::potion,
            PRIORITY_POTION,
            Intention::infoPack(targetPet, nullptr, potionType)
    );
}

std::unique_ptr<Intention> makeEscapeIntention(Player *player)
{
    return std::make_unique<Intention>(
            player->type(),
            intention_T::escape,
            PRIORITY_ESCAPE,
            Intention::infoPack(nullptr, nullptr, potion_T::null)
    );
}

void printVerticalLine(std::ostream &os)
{
    os << "--------------------------------------------------"
       << std::endl;
}

void printBothPetHp(Session *session, std::ostream &os)
{
    const auto *user = session->getPlayer1();
    const auto *userPet = user->getActingPet();
    const auto *enemy = session->getPlayer2();
    const auto *enemyPet = enemy->getActingPet();

    os << user->nameMy << " " << userPet->getName() << ": HP " << userPet->getCurHp()
       << " || "
       << enemy->nameMy << " " << enemyPet->getName() << ": HP " << enemyPet->getCurHp()
       << std::endl;
}

int weatherPowerPercent(element eleAttack, weather_T weather)
{
    if (weather == weather_T::normal) {
        return 100;
    }
    if (eleAttack == element::pyro) {
        return weather == weather_T::sunny ? 150 : 50;
    }
    if (eleAttack == element::hydro) {
        return weather == weather_T::rainy ? 150 : 50;
    }
    return 100;
}

skillUPtr makeSkill(skill_T skillT)
{
    using std::make_unique;
    using namespace skillCollection;
    switch (skillT) {
        case skill_T::tackle:
            return make_unique<Tackle>();
        case skill_T::claw:
            return make_unique<Claw>();
        case skill_T::quick:
            return make_unique<Quick>();
        case skill_T::slap:
            return make_unique<Slap>();
        case skill_T::sing:
            return make_unique<Sing>();
        case skill_T::paint:
            return make_unique<Paint>();
        case skill_T::leaf:
            return make_unique<Leaf>();
        case skill_T::synthesis:
            return make_unique<Synthesis>();
        case skill_T::flame:
            return make_unique<Flame>();
        case skill_T::sunny:
            return make_unique<Sunny>();
        case skill_T::stream:
            return make_unique<Stream>();
        case skill_T::rainy:
            return make_unique<Rainy>();
        case skill_T::volt:
            return make_unique<Volt>();
        case skill_T::thunder:
            return make_unique<Thunder>();
    }
    return nullptr;
}

bool operator>(const Intention &lhs, const Intention &rhs)
{
    if (lhs.priority != rhs.priority) {
        return lhs.priority > rhs.priority;
    }
    // same priority
    if (lhs.type != rhs.type) {
        // this should not happen
        std::clog << "Two intentions of same priority but have different intention_T!\n";
        return false;
    }
    switch (lhs.type) {
        case intention_T::skill: {
            const auto *lPet = std::get<Intention::infoPet>(lhs.info);
            const auto *rPet = std::get<Intention::infoPet>(rhs.info);
            if (lPet->getSpdPts() != rPet->getSpdPts()) {
                return lPet->getSpdPts() > rPet->getSpdPts();
            }
        }
            // Same speed also finds it eventually here
        default: {
            return lhs.playerType > rhs.playerType;
        }
    }
}

bool operator<(const Intention &lhs, const Intention &rhs)
{
    return !operator>(lhs, rhs);
}

std::string makeEnumOptionsWithBracket(const vector<int> &nums, const vector<std::string> &options)
{
    std::stringstream ret{};
    ret << "(";
    for (int i = 0; i < nums.size(); i++) {
        if (i != 0) {
            ret << ", ";
        }
        ret << nums[i] << " for " << options[i];
    }
    ret << ")";
    return ret.str();
}

std::string makeListWithAnd(const vector<std::string> &items)
{
    std::stringstream ret{};
    for (int i = 0; i < items.size(); i++) {
        if (i != 0) {
            ret << (i == items.size() - 1 ? " and " : ", ");
        }
        ret << items[i];
    }
    return ret.str();
}

std::vector<int> makeRange(int start, int end, int step)
{
    if (end <= start)
        return {};
    std::vector<int> ret;
    ret.reserve((end - start) / step + 1);
    while (start < end) {
        ret.push_back(start);
        start += step;
    }
    return ret;
}

bool RandomWithProbability(double prob)
{
    std::random_device dev;
    static std::uniform_real_distribution<double> u(0, 1);
    static std::default_random_engine e(dev());
    return u(e) <= prob;
}




//
// Created by Misakavan on 2023/7/6.
//

#ifndef PROJECT2_UTILITIES_H
#define PROJECT2_UTILITIES_H

#include "Defines.h"
#include "Skill.h"
#include "Pet.h"
#include "Session.h"
#include "Intention.h"
#include <set>
#include <memory>
#include <string>
#include <iostream>
#include <random>

using std::endl;

int elementDamagePercent(element eleAttack, element eleDefence);

int weatherPowerPercent(element eleAttack, weather_T weather);

bool isPriorTo(element eleAttack, element eleDefence);


[[maybe_unused]] std::unique_ptr<Skill> makeSkill_DEPRECATED(const Skill::nameType &skillName);

skillUPtr makeSkill(skill_T skillT);

std::unique_ptr<Pet> makePet(const Pet::nameType &petName);

std::ostream &operator<<(std::ostream &os, const Skill &entity);

std::unique_ptr<Intention> makeSkillIntention(Player *player, Pet *actingPet, Skill *skillToUse);

std::unique_ptr<Intention> makeChangeIntention(Player *player, Pet *newPet);

std::unique_ptr<Intention> makePotionIntention(Player *player, Pet *targetPet, potion_T potionType);

std::unique_ptr<Intention> makeEscapeIntention(Player *player);

bool operator>(const Intention &lhs, const Intention &rhs);

bool operator<(const Intention &lhs, const Intention &rhs);

int readInt(const std::string &prompt,
            const std::set<int> &choices,
            std::istream &is = std::cin,
            std::ostream &os = std::cout);

///
/// \tparam ContainerT a collector of int type
/// \return
template<class ContainerT>
int selectFrom(
        const std::string &prompt,
        const ContainerT &choices,
        std::istream &is = std::cin,
        std::ostream &os = std::cout)
{
    if (choices.empty()) {
        //   This should not happen.
        //   As far as this project is concerned,
        // -1 is not legal for any user input.
        return -1;
    }
    typename ContainerT::value_type ret = -1;
    do {
        os << prompt;
        is >> ret;
    } while (std::none_of(choices.cbegin(),
                          choices.cend(),
                          [ret](int other) { return ret == other; })
            );
    return ret;
}

void printVerticalLine(std::ostream &os = std::cout);

void printBothPetHp(Session *session, std::ostream &os = std::cout);

/// "(1 for W, 2 for L, 3 for G, 4 for Q, 5 for D)"
std::string makeEnumOptionsWithBracket(const std::vector<int> &nums, const std::vector<std::string> &options);

/// "Q, D and L"
std::string makeListWithAnd(const std::vector<std::string> &items);

std::vector<int> makeRange(int start, int end, int step = 1);

template<class PetPtrT>
std::string asStr(const PetPtrT &petPtr)
{
    return petPtr->getName();
}

template<class PetPtrT>
std::string asStrHpPair(const PetPtrT &petPtr)
{
    return petPtr->getName() + "-HP" + std::to_string(petPtr->getCurHp());
}

template<class PetPtrT>
std::string asStrOwnerPair(const PetPtrT &petPtr)
{
    return petPtr->getOwner()->nameMy + " " + petPtr->getName();
}


/// Gets the underlying type of a scoped enum class.
/// \tparam enumT an scoped or unscoped enum, such as "enum class element { ... };".
template<typename enumT>
constexpr typename std::underlying_type_t<enumT>
toUType(enumT enumerator) noexcept
{
    return static_cast<std::underlying_type_t<enumT>>(enumerator);
}


template <typename T>
void shuffleVector(std::vector<T> &vec) {
    std::random_device dev;
    std::default_random_engine e(dev());
    std::shuffle(vec.begin(), vec.end(), e);
}

/**
 * returns true versus false as prob versus (1 - prob).
 * @param prob
 * @return
 */
bool RandomWithProbability(double prob);

template <typename T>
T randomWithRange(T low, T high) {  // [low, high)
    std::random_device dev;
    std::uniform_int_distribution<T> u(low, high-1);
    std::default_random_engine e(dev());
    return u(e);
}

#endif // PROJECT2_UTILITIES_H

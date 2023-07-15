//
// Created by Misakavan on 2023/7/6.
//

#ifndef PROJECT2_INTENTION_H
#define PROJECT2_INTENTION_H


#include <memory>
#include "Defines.h"

class Skill;

class Pet;

class Player;


enum intentionInfoField
{
    pet, skill, potion
};


class Intention
{
public:
    using infoPack = std::tuple<Pet *, Skill *, potion_T>;

    enum infoField {infoPet=0, infoSkill, infoPotion};

    Intention() = default;

    Intention(player_T playerType, intention_T intentionType, int priority, infoPack info);

    ~Intention() = default;

    player_T playerType = player_T::ai;

    intention_T type = intention_T::null;

    int priority = -1;

    infoPack info{nullptr, nullptr, potion_T::null};
};

#endif //PROJECT2_INTENTION_H

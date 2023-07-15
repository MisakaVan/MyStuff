//
// Created by Misakavan on 2023/7/6.
//

#ifndef PROJECT2_DEFINES_H
#define PROJECT2_DEFINES_H

//#define DEBUG

#include <map>
#include <string>
#include <vector>

using damageType = int;

enum class element : unsigned int
{
    normal = 0,
    dendro, // grass
    pyro,   // fire
    hydro,  // water
    electro,
    null = 404
};

enum class buff_T : unsigned int
{
    attackBoost = 0,
    painted, // minus 50% defence
    sleeping
};

const unsigned int MAX_BUFF_TYPE = 10;

enum class potion_T : unsigned int
{
    attackPotion = 0,
    revivePotion,
    null = 404
};

const unsigned int MAX_POTION_TYPE = 10;

const std::vector<std::string> PET_NAME_LIST = {"W", "L", "G", "Q", "D"};


enum class player_T : unsigned int
{
    user = 10, ai = 20
};

enum class intention_T : unsigned int
{
    skill,
    change,
    potion,
    escape,
    null = 404
};

const int PRIORITY_CHANGE = 6;
const int PRIORITY_POTION = 5;
const int PRIORITY_ESCAPE = 8;

enum class weather_T
{
    normal,
    sunny,
    rainy
};

enum class skill_T
{
    tackle,
    claw,
    quick,
    slap,
    sing,
    paint,
    leaf,
    synthesis,
    flame,
    sunny,
    stream,
    rainy,
    volt,
    thunder
};

enum class skillResult_T
{
    succ,
    failure,
    waste,
    sleeping,
    notOK // did not do anything
};

enum class actIntentionResult_T
{
    normal,
    escaped
};


#endif // PROJECT2_DEFINES_H

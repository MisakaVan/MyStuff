//
// Created by Misakavan on 2023/7/8.
//

#include "EnemyPlayer1.h"
#include "utilities.h"

EnemyPlayer1::EnemyPlayer1() : Player("Enemy", "Enemy's")
{

}

EnemyPlayer1::EnemyPlayer1(const Player::nameType &name, const Player::nameType &nameMy) : Player(name, nameMy) {}


EnemyPlayer1::~EnemyPlayer1()
= default;

void EnemyPlayer1::selectInitPet()
{
    std::vector<Pet::nameType> nameList{"D", "W", "L"};
    for (const auto &name: nameList) {
        appendPet(makePet(name));
    }
    setActingPet(0);
    auto &os = getSession()->getOs();
    os << name << " has " << makeListWithAnd(nameList) << endl;
}

void EnemyPlayer1::makeIntention()
{
    // todo!
    const auto &name = getActingPet()->getName();
    auto oppoPet = _opponent->getActingPet();
    Pet::skillIndex useThisSkill = -1;
    if (name == "D") {
        // {"D", {element::electro, 120, 9,  8,  7,  {skill_T::slap,   skill_T::sing,   skill_T::paint}}}
        bool flagOppoPetLowHP = oppoPet->getCurHp() * 3 < oppoPet->getMaxHp();
        bool flagOppoPetSleeping = oppoPet->hasBuff(buff_T::sleeping);
        bool flagOppoPetPainted = oppoPet->hasBuff(buff_T::painted);
        bool flagOppoPetBeSleepingOrPainted = flagOppoPetSleeping || flagOppoPetPainted;
        if (flagOppoPetLowHP && flagOppoPetBeSleepingOrPainted) {
            useThisSkill = 0; // slap
        } else if (!flagOppoPetSleeping) {
            useThisSkill = 1; // sing
        } else if (!flagOppoPetPainted) {
            useThisSkill = 2; // paint
        } else {
            useThisSkill = 0; // slap
        }

    } else if (name == "L") {
        // {"L", {element::hydro,   100, 11, 10, 11, {skill_T::claw,   skill_T::flame,  skill_T::sunny}}},
        bool flagOppoPetLowHP = oppoPet->getCurHp() * 5 < oppoPet->getMaxHp();
        bool flagOppoPetBeHydroOrPyro =
                oppoPet->getElement() == element::hydro || oppoPet->getElement() == element::pyro;
        bool flagWeatherBeSunny = _session->getWeather() == weather_T::sunny;

        if (flagOppoPetLowHP && flagOppoPetBeHydroOrPyro) {
            useThisSkill = 0; // claw
        } else {
            useThisSkill = flagWeatherBeSunny ? 1  // flame
                                              : 2; // sunny
        }

    } else { // W
        // {"W", {element::hydro,   110, 10, 10, 10, {skill_T::tackle, skill_T::leaf,   skill_T::synthesis}}},
        bool flagWLowHP = getActingPet()->getCurHp() * 2 < getActingPet()->getMaxHp();
        bool flagLBeOK = getPet((indexDict.L))->isOK();
        bool flagLHpHigherThanUserPet = getActingPet()->getCurHp() > oppoPet->getCurHp();
        bool flagOppoPetBeHydro = (oppoPet->getElement() == element::hydro);
        if (flagWLowHP && flagLBeOK) {
            // change to L
            setNextIntention(makeChangeIntention(
                    this,
                    getPet(indexDict.L)));
            return;
        } else if (!flagWLowHP || (flagLHpHigherThanUserPet && !flagLBeOK)) {
            useThisSkill = flagOppoPetBeHydro ? 1  // leaf
                                              : 0; // tackle
        } else {
            useThisSkill = 2; // synthesis
        }
    }

    setNextIntention(makeSkillIntention(
            this,
            getActingPet(),
            getActingPet()->getSkill(useThisSkill)));
}

/*
actIntentionResult_T EnemyPlayer1::actIntention()
{
    return Player::actIntention();
}
*/

//void EnemyPlayer1::checkOkAndReplaceActingPet()
//{
//    Player::checkOkAndReplaceActingPet();
//}

Player::petIndex EnemyPlayer1::getNextPetPreference()
{
    // todo: maybe should be improved
    petIndex ret = -1;
    const auto &name = getActingPet()->getName();
    if (name == "D") {
        ret = indexDict.W;
    } else if (name == "W") {
        ret = indexDict.L;
    } else {
        ret = indexDict.W;
    }
    if (!getPet(ret)->isOK()) {
        ret = -1;
    }
    return ret;
}


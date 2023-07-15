//
// Created by Misakavan on 2023/7/14.
//

#include "EnemyPlayer2.h"
#include "Pet.h"
#include "utilities.h"

EnemyPlayer2::EnemyPlayer2() : Player("Enemy", "Enemy's") {}

EnemyPlayer2::EnemyPlayer2(const Player::nameType &name, const Player::nameType &nameMy) : Player(name, nameMy) {}

EnemyPlayer2::~EnemyPlayer2()
= default;

void EnemyPlayer2::makeIntention()
{
    const auto &name = getActingPet()->getName();
    auto oppoPet = _opponent->getActingPet();
    auto weather = getSession()->getWeather();
    Pet *myG = getPet(indexDict.G);
    Pet *myW = getPet(indexDict.W);
    Pet *myQ = getPet(indexDict.Q);

    Pet::skillIndex useThisSkill = -1;

    const bool flagOppoPetBePyro = oppoPet->getElement() == element::pyro;
    const bool flagOppoPetBeDendro = oppoPet->getElement() == element::dendro;
    const bool flagOppoPetBeElectro = oppoPet->getElement() == element::electro;
    const bool flagOppoPetBeHydro = oppoPet->getElement() == element::hydro;
    const bool flagWeatherBeSunny = weather == weather_T::sunny;
    const bool flagWeatherBeRainy = weather == weather_T::rainy;

    if (!myG->isOK() && potionDict.revive > 0) {
        // revive my G!
        setNextIntention(
                makePotionIntention(this, myG, potion_T::revivePotion)
        );
        return;
    }

/*    {
        {"L", {element::pyro,    100, 11, 10, 11, {skill_T::claw,   skill_T::flame,  skill_T::sunny}}},
        {"D", {element::normal,  120, 9,  8,  7,  {skill_T::slap,   skill_T::sing,   skill_T::paint}}}
    }*/
    if (name == "W") {
        // {"W", {element::dendro,  110, 10, 10, 10, {skill_T::tackle, skill_T::leaf,   skill_T::synthesis}}},
        bool flagMyGisOk = myG->isOK();
        if (flagOppoPetBePyro) {
            if (flagMyGisOk) {
                setNextIntention(makeChangeIntention(this, myG));
                return;
            }
            bool flagWHighHp = myW->getCurHp() * 3 > myW->getMaxHp() * 2;
            useThisSkill = flagWHighHp ? 0 /* Tackle */
                                       : 2 /* Synthesis */;
        } else if (flagOppoPetBeDendro) {
            bool flagWHighHp = myW->getCurHp() * 2 > myW->getMaxHp();
            useThisSkill = flagWHighHp ? 0 /* Tackle */
                                       : 2 /* Synthesis */;
        } else {
            bool flagWHighHp = myW->getCurHp() * 2 > myW->getMaxHp();
            bool flagOppoLowHp = oppoPet->getCurHp() * 5 < oppoPet->getMaxHp();
            useThisSkill = (flagWHighHp || flagOppoLowHp) ? 1 /* Leaf */
                                                          : 2 /* Synthesis */;
        }

    } else if (name == "G") {
        // {"G", {element::hydro,   100, 10, 11, 9,  {skill_T::tackle, skill_T::stream, skill_T::rainy}}},
        if (flagWeatherBeSunny) {
            useThisSkill = 2; // rainy
        }
            // weather not sunny
        else if (flagOppoPetBeElectro || (flagWeatherBeRainy && flagOppoPetBeDendro)) {
            // trying to put on W
            if (myW->isOK()) {
                setNextIntention(
                        makeChangeIntention(this, myW)
                );
                return;
            }
            // W not ok
            // trying to put on Q
            if (flagWeatherBeRainy && myQ->isOK()) {
                setNextIntention(
                        makeChangeIntention(this, myQ)
                );
                return;
            }
            // both trying failed
            useThisSkill = 1; // stream
        } else if (flagOppoPetBeHydro) {
            useThisSkill = 0; // tackle
        } else {
            useThisSkill = flagWeatherBeRainy ? 1 /* stream */
                                              : 2 /* rainy */;
        }

    } else {
        // Q
        // {"Q", {element::electro, 90,  10, 10, 12, {skill_T::quick,  skill_T::volt,   skill_T::thunder}}},
        bool flagOppoPetLowHp = oppoPet->getCurHp() * 5 < oppoPet->getMaxHp();
        if (flagOppoPetLowHp) {
            useThisSkill = 0; // quick
        } else if (flagOppoPetBePyro) {
            // trying to put on G
            if (myG->isOK()) {
                setNextIntention(
                        makeChangeIntention(this, myG)
                );
                return;
            }
            useThisSkill = 1; // volt
        } else {
            // not Pyro
            if (flagWeatherBeRainy) {
                useThisSkill = 2; // thunder
            } else {
                // trying to put on W
                if (myW->isOK()) {
                    setNextIntention(
                            makeChangeIntention(this, myW)
                    );
                    return;
                }
                useThisSkill = (flagOppoPetBeElectro || flagOppoPetBeDendro) ? 0 /* quick */
                                                                             : 1 /* volt */;
            }
        }
    }

    setNextIntention(makeSkillIntention(
            this,
            getActingPet(),
            getActingPet()->getSkill(useThisSkill))
    );
}

void EnemyPlayer2::selectInitPet()
{
    std::vector<Pet::nameType> nameList{"W", "G", "Q"};
//    shuffleVector(nameList);
    for (const auto &name: nameList) {
        appendPet(makePet(name));
    }
#ifdef DEBUG
    setActingPet(0); // start with the fixed one
#else
    setActingPet(randomWithRange(0, 3));
#endif
    auto &os = getSession()->getOs();
    os << Player::name << " has " << makeListWithAnd(nameList) << endl;

/*
    indexDict.W = static_cast<int>(
            std::find(nameList.cbegin(), nameList.cend(), "W")
            - nameList.cbegin()
    );
    indexDict.G = static_cast<int>(
            std::find(nameList.cbegin(), nameList.cend(), "G")
            - nameList.cbegin()
    );
    indexDict.Q = static_cast<int>(
            std::find(nameList.cbegin(), nameList.cend(), "Q")
            - nameList.cbegin()
    );
    */
}
//
//void EnemyPlayer2::checkOkAndReplaceActingPet()
//{
//    Player::checkOkAndReplaceActingPet();
//}

Player::petIndex EnemyPlayer2::getNextPetPreference()
{
    // todo!
    petIndex ret = -1;

    auto *ptrG = getPet(indexDict.G);
    auto *ptrW = getPet(indexDict.W);
    auto *ptrQ = getPet(indexDict.Q);

    if (_actingIndex == indexDict.W) {
        // W is down
        ret = ptrG->isOK() ? indexDict.G : indexDict.Q;
    } else if (_actingIndex == indexDict.Q) {
        // Q is down
        ret = ptrG->isOK() ? indexDict.G : indexDict.W;
    } else {
        // G is down
        ret = ptrW->isOK() ? indexDict.W : indexDict.Q;
    }

    if (!getPet(ret)->isOK()) ret = -1;
    return ret;
}



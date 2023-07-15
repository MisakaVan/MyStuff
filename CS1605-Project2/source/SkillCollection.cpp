//
// Created by Misakavan on 2023/7/11.
//

#include "SkillCollection.h"
#include "Player.h"
#include "Session.h"


skillCollection::Tackle::Tackle()
        : AttackSkill("Tackle", element::normal, 20, 0) {}

skillCollection::Claw::Claw()
        : AttackSkill("Claw", element::normal, 15, 0) {}

skillCollection::Quick::Quick()
        : AttackSkill("Quick", element::normal, 15, 1) {}

skillCollection::Slap::Slap()
        : AttackSkill("Slap", element::normal, 15, 0) {}

double skillCollection::Slap::dependentDamageRatio(Session *session, Player *opponent)
{
    return (opponent->getActingPet()->hasBuff(buff_T::sleeping) ? 2 : 1)
           * AttackSkill::dependentDamageRatio(session, opponent);
}

skillCollection::Leaf::Leaf()
        : AttackSkill("Leaf", element::dendro, 20, 0) {}

skillCollection::Flame::Flame()
        : AttackSkill("Flame", element::pyro, 20, 0) {}

skillCollection::Stream::Stream()
        : AttackSkill("Stream", element::hydro, 20, 0) {}

skillCollection::Volt::Volt()
        : AttackSkill("Volt", element::electro, 25, 0) {}


skillCollection::Thunder::Thunder()
        : AttackSkill("Thunder", element::electro, 50, 0) {}

skillResult skillCollection::Thunder::castAct(Session *session, Player *opponent)
{
    auto currentWeather = session->getWeather();
    if (currentWeather != weather_T::rainy) {
        return makeResultAndPrintMsg(skillResult_T::failure, 0, session, opponent);
    }
    return AttackSkill::castAct(session, opponent);
}


skillCollection::Sing::Sing()
        : AssistSkill("Sing", element::normal, 0, 0)
{
    customVerb = "falls asleep";
}

skillResult skillCollection::Sing::castAct(Session *session, Player *opponent)
{
    auto oppoPet = opponent->getActingPet();
    if (oppoPet->hasBuff(buff_T::sleeping)) {
        return makeResultAndPrintMsg(skillResult_T::waste, 0, session, opponent);
    }
    oppoPet->setBuffLastingTime(buff_T::sleeping, 3);
    return makeResultAndPrintMsg(skillResult_T::succ, 0, session, opponent);
}


skillCollection::Paint::Paint()
        : AssistSkill("Paint", element::normal, 0, 0)
{
    customVerb = "is painted";
}

skillResult skillCollection::Paint::castAct(Session *session, Player *opponent)
{
    auto *oppoPet = opponent->getActingPet();
    auto timeStamp = oppoPet->getLatestActiveTimeStamp();
    auto curRound = session->getRoundCount();
    if (timeStamp >= curRound) {
        // oppoPet has acted this round, cannot paint
        return makeResultAndPrintMsg(skillResult_T::failure, 0, session, opponent);
    }
    oppoPet->setBuffLastingTime(buff_T::painted, -1);
    return makeResultAndPrintMsg(skillResult_T::succ, 0, session, opponent);
}


skillCollection::Synthesis::Synthesis()
        : AssistSkill("Synthesis", element::dendro, 0, 0) {}

skillResult skillCollection::Synthesis::castAct(Session *session, Player *opponent)
{
    int restorePercent = 30;
    auto weather = session->getWeather();
    if (weather == weather_T::rainy) {
        restorePercent = 40;
    } else if (weather == weather_T::sunny) {
        restorePercent = 20;
    }
    int restoredPart = static_cast<int>(lround(1.0 * restorePercent / 100 * _ownerPet->getMaxHp()));
    double HPTemp = _ownerPet->getCurHp()
                    + restoredPart;
    int HPTempInt = static_cast<int>(HPTemp);
    _ownerPet->_curHP = std::min(_ownerPet->getMaxHp(), HPTempInt);
    return makeResultAndPrintMsg(skillResult_T::succ, -restoredPart, session, opponent);
}

std::string skillCollection::Synthesis::makeSuccMessageEffectPart(damageType damage, Session *session, Player *opponent)
{
    return std::string("Recovery: ") + std::to_string(-damage);
}

//void skillCollection::Synthesis::sendMessage(skillResult_T resultType, damageType damage, Session *session,
//                                             Player *opponent)
//{
//    // "Enemy’s W uses Synthesis! Recovery: 33"
//    const auto &thisPlayerPronoun = _ownerPet->_ownerPlayer->nameMy;
//    const auto &oppoPlayerPronoun = opponent->nameMy;
//    auto &os = session->getOs();
//    os << thisPlayerPronoun << " " << _ownerPet->getName() << " uses " << this->name() << "! ";
//    damageType revivedAmount = -damage;
//    os << "Recovery: " << revivedAmount << endl;
//}


skillCollection::Sunny::Sunny()
        : AssistSkill("Sunny", element::pyro, 0, 1)
{
    secondSentenceStartsWithName = false;
    customVerb = "Weather becomes sunny";
}

skillResult skillCollection::Sunny::castAct(Session *session, Player *opponent)
{
    session->setWeather(weather_T::sunny, 5);
    return makeResultAndPrintMsg(skillResult_T::succ, 0, session, opponent);
}


skillCollection::Rainy::Rainy()
        : AssistSkill("Rainy", element::hydro, 0, 1)
{
    secondSentenceStartsWithName = false;
    customVerb = "Weather becomes rainy";
}

skillResult skillCollection::Rainy::castAct(Session *session, Player *opponent)
{
    session->setWeather(weather_T::rainy, 5);
    return makeResultAndPrintMsg(skillResult_T::succ, 0, session, opponent);
}

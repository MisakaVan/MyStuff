//
// Created by Misakavan on 2023/7/6.
//
#include <utility>
#include <iostream>
#include <cmath>
#include "Skill.h"
#include "Player.h"
#include "Session.h"
#include "utilities.h"

Skill::Skill() = default;

Skill::Skill(Skill::nameType name, element element, int power, int priority)
        : _name(std::move(name)),
          _element(element),
          _power(power),
          _priority(priority) {}

skillResult Skill::makeResultAndPrintMsg(skillResult_T type, damageType damage, Session *session, Player *opponent)
{
    sendMessage(type, damage, session, opponent);
    if (type == skillResult_T::waste
        || type == skillResult_T::succ) {
        _ownerPet->_latestTimeStamp_NotFailAction = session->getRoundCount();
    }
    return {type, damage};
}

Skill::castCheckResult_T Skill::castCheck(Session *session, Player *opponent)
{
    if (!_ownerPet->isOK())
        return castCheckResult_T::casterIsNotOK;
    if (_ownerPet->getBuffStatus(buff_T::sleeping) != 0)
        return castCheckResult_T::sleeping;

    return Skill::castCheckResult_T::ableToCast;
}

skillResult Skill::cast(Session *session, Player *opponent)
{
    auto checkRes = castCheck(session, opponent);
    if (checkRes == castCheckResult_T::sleeping) {
        return makeResultAndPrintMsg(skillResult_T::sleeping, 0, session, opponent);
    }
    if (checkRes == castCheckResult_T::casterIsNotOK) {
        return makeResultAndPrintMsg(skillResult_T::notOK, 0, session, opponent);
    }

    return castAct(session, opponent);
}

void Skill::sendMessage(skillResult_T resultType, damageType damage, Session *session, Player *opponent)
{
    // "Your Q uses Thunder! Failure"
    //
    // "Enemy’s D uses Slap! Damage: 54"
    // "Your W is sleeping"

    auto &os = session->getOs();
    const auto &thisPlayerPronoun = _ownerPet->_ownerPlayer->nameMy;

    if (resultType == skillResult_T::notOK) {
        // the pet did nothing, and there is no output
        return;
    }

    if (resultType == skillResult_T::sleeping) {
        os << thisPlayerPronoun << " " << _ownerPet->getName() << " is sleeping!\n";
        return;
    }
    os << thisPlayerPronoun << " " << _ownerPet->getName() << " uses " << this->name() << "! ";
    if (resultType != skillResult_T::succ) {
        // failure or waste
        os << (resultType == skillResult_T::failure ? "Failure" : "Waste") << endl;
        return;
    }
    // succ
    os << makeSuccMessageEffectPart(damage, session, opponent) << endl;
}

AttackSkill::AttackSkill(Skill::nameType name, element element, int power, int priority)
        : Skill(std::move(name), element, power, priority) {}

skillResult AttackSkill::castAct(Session *session, Player *opponent)
{
    auto *oppoPet = opponent->getActingPet();
    auto currentWeather = session->getWeather();
    double weatherPowerRatio = 1. * weatherPowerPercent(_element, currentWeather) / 100;
    double elementDamageRatio = 1. * elementDamagePercent(_element, oppoPet->getElement()) / 100;
    double dependentDamageRatio1 = dependentDamageRatio(session, opponent);
    double attackPts = _ownerPet->getBuffedAtkPts();
    double defendPts = oppoPet->getBuffedDefPts();
    auto powerPts = static_cast<double>(_power);
    double damageTemp = powerPts * weatherPowerRatio
                        * attackPts
                        / defendPts
                        * elementDamageRatio
                        * dependentDamageRatio1; // extra determination, such as Slapping
    int damage = static_cast<int>(lround(damageTemp));

    auto ret = makeResultAndPrintMsg(skillResult_T::succ, damage, session, opponent);
    oppoPet->gotDamage(damage);
    // Notice the extra job here
    // so that '*** wakes up' will appear below '*** uses $$$! Damage: %%%\n'. Shit! "
    return ret;
}

double AttackSkill::dependentDamageRatio(Session *session, Player *opponent)
{
    return 1;
}

void AttackSkill::sendMessage(skillResult_T resultType, damageType damage, Session *session, Player *opponent)
{
    // "Your Q uses Thunder! Failure"
    //
    // "Enemy’s D uses Slap! Damage: 54"
    // "Your W is sleeping"

//    const auto &thisPlayerPronoun = _ownerPet->_ownerPlayer->nameMy;
////    const auto &oppoPlayerPronoun = opponent->nameMy;
//    auto &os = session->getOs();
//    if (resultType == skillResult_T::sleeping) {
//        os << thisPlayerPronoun << " " << _ownerPet->getName() << "is sleeping\n";
//        return;
//    }
//
//    os << thisPlayerPronoun << " " << _ownerPet->getName() << " uses " << this->name() << "! ";
//    if (resultType == skillResult_T::succ) {
//        os << "Damage: " << damage;
//    } else if (resultType == skillResult_T::failure) {
//        os << "Failure";
//    } else {
//        os << "Waste";
//    }
//    os << endl;
    return Skill::sendMessage(resultType, damage, session, opponent);
}

std::string AttackSkill::makeSuccMessageEffectPart(damageType damage, Session *session, Player *opponent)
{
    return std::string("Damage: ") + std::to_string(damage);
}

AssistSkill::AssistSkill(const Skill::nameType &name, element element, int power, int priority)
        : Skill(name, element, power, priority) {}

void AssistSkill::sendMessage(skillResult_T resultType, damageType damage, Session *session, Player *opponent)
{
    // "Enemy’s D uses Sing! Your Q falls asleep"

//    const auto &thisPlayerPronoun = _ownerPet->_ownerPlayer->nameMy;
//    const auto &oppoPlayerPronoun = opponent->nameMy;
//    auto &os = session->getOs();
//    os << thisPlayerPronoun << " " << _ownerPet->getName() << " uses " << this->name() << "! ";
//    if (secondSentenceStartsWithName) {
//        os << oppoPlayerPronoun << " " << opponent->getActingPet()->getName() << " ";
//    }
//    os << customVerb << endl;

    return Skill::sendMessage(resultType, damage, session, opponent);
}

std::string AssistSkill::makeSuccMessageEffectPart(damageType damage, Session *session, Player *opponent)
{
    if (secondSentenceStartsWithName) {
        // "Your Q" + "falls asleep"
        return asStrOwnerPair(opponent->getActingPet()) + " " + customVerb;
    }
    // "Weather becomes sunny"
    return customVerb;
}





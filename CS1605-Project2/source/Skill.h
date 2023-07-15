//
// Created by Misakavan on 2023/7/6.
//

#ifndef PROJECT2_SKILL_H
#define PROJECT2_SKILL_H

#include <string>
#include <vector>
#include "Defines.h"

class Pet;

class Player;

class Session;

//using powerT = int;
//using priorityT = int;
//using skillInfo = std::tuple<element, powerT, priorityT>;

struct skillResult
{
    skillResult_T type = skillResult_T::succ;
    damageType damage = 0;
};

class Skill
{
public:
    using nameType = std::string;

    Skill();

    Skill(nameType name, element element, int power, int priority);

    virtual ~Skill() = default;

    const nameType &name() const { return _name; };

    int priority() const { return _priority; }

    void setOwner(Pet *pet) { _ownerPet = pet; }

    /// First runs castCheck,
    /// then choose to castAct or returns directly, depending on the checkResult.
    /// \param session
    /// \param opponent
    /// \return
    virtual skillResult cast(Session *session, Player *opponent);

protected:
    enum class castCheckResult_T
    {
        ableToCast,
        sleeping,
        casterIsNotOK
    };

    castCheckResult_T castCheck(Session *session, Player *opponent);

    virtual skillResult castAct(Session *session, Player *opponent) = 0;

    /// Auto records the latest not-failure action of the pet.
    /// Prints message to os.
    /// Do use this for each castAct!
    skillResult
    makeResultAndPrintMsg(skillResult_T type, damageType damage, Session *session, Player *opponent);

    virtual void sendMessage(skillResult_T resultType, damageType damage, Session *session, Player *opponent);

    /// "Your Q falls asleep", "Your Q is painted", "Damage: 54", "Recovery: 33"
    virtual std::string makeSuccMessageEffectPart(damageType damage, Session *session, Player *opponent) = 0;

    nameType _name;
    element _element = element::normal;
    int _power = 0;
    int _priority = -1;

    Pet *_ownerPet = nullptr;
};


class AttackSkill : public Skill
{
public:
    AttackSkill() = default;

    AttackSkill(nameType name, element element, int power, int priority);

    skillResult castAct(Session *session, Player *opponent) override;

    virtual double dependentDamageRatio(Session *session, Player *opponent);

protected:
    void sendMessage(skillResult_T resultType, damageType damage, Session *session, Player *opponent) override;

    std::string makeSuccMessageEffectPart(damageType damage, Session *session, Player *opponent) override;
};


class AssistSkill : public Skill
{
public:
    AssistSkill() = default;

    AssistSkill(const nameType &name, element element, int power, int priority);

protected:
    // e.g. "falls asleep", "is painted", "Weather becomes sunny"
    std::string customVerb{};

    bool secondSentenceStartsWithName = true;

    void sendMessage(skillResult_T resultType, damageType damage, Session *session, Player *opponent) override;

    std::string makeSuccMessageEffectPart(damageType damage, Session *session, Player *opponent) override;
};


#endif //PROJECT2_SKILL_H

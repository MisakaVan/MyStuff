//
// Created by Misakavan on 2023/7/6.
//

#ifndef PROJECT2_PET_H
#define PROJECT2_PET_H

#include "Defines.h"
#include "Skill.h"
#include "SkillCollection.h"
#include <string>
#include <utility>
#include <vector>
#include <memory>

class Skill;

class Player;

using skillUPtr = std::unique_ptr<Skill>;

class Pet
{
    friend class Skill;
    friend class AttackSkill;
    friend class AssistSkill;
    friend class skillCollection::Synthesis;

public:
    using nameType = std::string;
    using skillUPtr = std::unique_ptr<Skill>;
    using skillIndex = size_t;

    Pet(nameType name, element element, int maxHp, int atkPts, int defPts, int spdPts,
        const std::vector<skill_T> &skillList);


    const nameType &getName() const { return _name; }

    element getElement() const { return _element; }

    int getMaxHp() const { return _maxHP; }

    int getCurHp() const { return _curHP; }

    bool isOK() const { return getCurHp() > 0; }

    int getAtkPts() const { return _atkPts; }

    double getBuffedAtkPts() const { return getAtkPts(); }

    int getDefPts() const { return _defPts; }

    double getBuffedDefPts() const;

    int getSpdPts() const { return _spdPts; }

    Player *getOwner() const { return _ownerPlayer; }

    void setOwner(Player *owner) { _ownerPlayer = owner; }

    Skill *getSkill(skillIndex index) const { return _skillList.at(index).get(); }

    skillIndex getSkillListSize() const { return _skillList.size(); }

    unsigned int getLatestActiveTimeStamp() const { return _latestTimeStamp_NotFailAction; }

    void gotDamage(damageType damage);

    bool hasBuff(buff_T buffType) const;

    void tickBuffLastingTime();

    void setBuffLastingTime(buff_T buffType, int newTime);

    void removeBuff(buff_T buffType);

    /// Doesn't send message.
    void revive();

protected:
    int getBuffStatus(buff_T buff) const;

private:
    nameType _name;
    element _element;
    int _maxHP;
    int _curHP;
    int _atkPts;
    int _defPts;
    int _spdPts;

    std::vector<skillUPtr> _skillList;

    Player *_ownerPlayer = nullptr;

    std::vector<int> _buffLastTime;

    unsigned int _latestTimeStamp_NotFailAction = 0;

};


#endif //PROJECT2_PET_H

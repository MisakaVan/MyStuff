//
// Created by Misakavan on 2023/7/6.
//

#include "Pet.h"
#include "utilities.h"

Pet::Pet(Pet::nameType name,
         element element,
         int maxHp,
         int atkPts,
         int defPts,
         int spdPts,
         const std::vector<skill_T> &skillList)
        : _name(std::move(name)),
          _element(element),
          _maxHP(maxHp),
          _curHP(maxHp),
          _atkPts(atkPts),
          _defPts(defPts),
          _spdPts(spdPts),
          _skillList(),
          _buffLastTime(MAX_BUFF_TYPE, 0)
{
    for (const auto skillT: skillList) {
        auto p = makeSkill(skillT);
        p->setOwner(this);
        _skillList.push_back(std::move(p));
    }
}

int Pet::getBuffStatus(buff_T buff) const
{
    auto u = toUType(buff);
    return _buffLastTime.at(u);
}

bool Pet::hasBuff(buff_T buffType) const
{
    return getBuffStatus(buffType) != 0;
}

double Pet::getBuffedDefPts() const
{
    return 1.0
           * getDefPts()
           * (hasBuff(buff_T::painted) ? 0.5 : 1);
}

void Pet::tickBuffLastingTime()
{
    for (unsigned int idx = 0; idx < _buffLastTime.size(); idx++) {
        if (_buffLastTime[idx] > 0) {
            if (_buffLastTime[idx] == 1) {
                // buff eliminated
                removeBuff(static_cast<buff_T>(idx));
            } else {
                --_buffLastTime[idx];
            }
        }
    }
}

void Pet::setBuffLastingTime(buff_T buffType, int newTime)
{
    _buffLastTime.at(toUType(buffType)) = newTime;
}

void Pet::gotDamage(damageType damage)
{
    if (damage <= 0) {
        std::clog << "[log] Pet " << _name << " of " << _ownerPlayer->name
                  << " got damage of " << damage << " , which is not positive!\n";
        return;
    }
    _curHP -= damage;
    // clear sleeping
    removeBuff(buff_T::sleeping);
}

void Pet::removeBuff(buff_T buffType)
{
    if (!hasBuff(buffType))
        return;
    setBuffLastingTime(buffType, 0);

    if (!isOK()) return;
    auto &os = _ownerPlayer->getSession()->getOs();
    if (buffType == buff_T::sleeping) {
        os << _ownerPlayer->nameMy << " " << getName() << " wakes up\n";
    }
}

void Pet::revive()
{
    if (isOK()) return;
    _curHP = static_cast<int>(lround(.5 * getMaxHp()));
}

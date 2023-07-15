//
// Created by Misakavan on 2023/7/11.
//

#ifndef PROJECT2_SKILLCOLLECTION_H
#define PROJECT2_SKILLCOLLECTION_H

#include "Skill.h"

namespace skillCollection {
    class Tackle : public AttackSkill
    {
    public:
        Tackle();
    };

    class Claw : public AttackSkill
    {
    public:
        Claw();
    };

    class Quick : public AttackSkill
    {
    public:
        Quick();
    };

    class Slap : public AttackSkill
    {
    public:
        Slap();

        double dependentDamageRatio(Session *session, Player *opponent) override;
    };

    class Leaf : public AttackSkill
    {
    public:
        Leaf();
    };

    class Flame : public AttackSkill
    {
    public:
        Flame();
    };

    class Stream : public AttackSkill
    {
    public:
        Stream();
    };

    class Volt : public AttackSkill
    {
    public:
        Volt();
    };

    class Thunder : public AttackSkill
    {
    public:
        Thunder();

        skillResult castAct(Session *session, Player *opponent) override;
    };

    /* Assist Skills */

    class Sing : public AssistSkill
    {
    public:
        Sing();

        skillResult castAct(Session *session, Player *opponent) override;
    };

    class Paint : public AssistSkill
    {
    public:
        Paint();

        skillResult castAct(Session *session, Player *opponent) override;
    };

    class Synthesis : public AssistSkill
    {
    public:
        Synthesis();

        skillResult castAct(Session *session, Player *opponent) override;

    protected:
//        void sendMessage(skillResult_T resultType, damageType damage, Session *session, Player *opponent) override;
        std::string makeSuccMessageEffectPart(damageType damage, Session *session, Player *opponent) override;
    };

    class Sunny : public AssistSkill
    {
    public:
        Sunny();

        skillResult castAct(Session *session, Player *opponent) override;
    };

    class Rainy : public AssistSkill
    {
    public:
        Rainy();

        skillResult castAct(Session *session, Player *opponent) override;
    };
}


#endif //PROJECT2_SKILLCOLLECTION_H

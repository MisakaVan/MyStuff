//
// Created by Misakavan on 2023/7/8.
//

#ifndef PROJECT2_ENEMYPLAYER1_H
#define PROJECT2_ENEMYPLAYER1_H

#include "Player.h"

class EnemyPlayer1 : public Player
{
public:
    EnemyPlayer1();

    EnemyPlayer1(const nameType &name, const nameType &nameMy);

    ~EnemyPlayer1() override;

    void makeIntention() override;

//    actIntentionResult_T actIntention() override;

    void selectInitPet() override;

//    void checkOkAndReplaceActingPet() override;

    petIndex getNextPetPreference() override;

    struct index
    {
        int D = 0;
        int W = 1;
        int L = 2;
    };

    index indexDict;
};


#endif //PROJECT2_ENEMYPLAYER1_H

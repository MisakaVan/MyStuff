//
// Created by Misakavan on 2023/7/14.
//

#ifndef PROJECT2_ENEMYPLAYER2_H
#define PROJECT2_ENEMYPLAYER2_H

#include "Player.h"

class EnemyPlayer2 : public Player
{
public:
    EnemyPlayer2();

    EnemyPlayer2(const nameType &name, const nameType &nameMy);

    ~EnemyPlayer2() override;

    void makeIntention() override;

    void selectInitPet() override;

    petIndex getNextPetPreference() override;

    struct index
    {
        int W = 0;
        int G = 1;
        int Q = 2;
    };

    index indexDict;



};


#endif //PROJECT2_ENEMYPLAYER2_H

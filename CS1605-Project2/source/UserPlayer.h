//
// Created by Misakavan on 2023/7/8.
//

#ifndef PROJECT2_USERPLAYER_H
#define PROJECT2_USERPLAYER_H

#include "Player.h"

class UserPlayer: public Player
{

public:
    UserPlayer();

    UserPlayer(const nameType &name, const nameType &nameMy);

    ~UserPlayer() override;

    void makeIntention() override;

//    actIntentionResult_T actIntention() override;

    void selectInitPet() override;

//    void checkOkAndReplaceActingPet() override;

    petIndex getNextPetPreference() override;
};


#endif //PROJECT2_USERPLAYER_H

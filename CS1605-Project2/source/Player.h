//
// Created by Misakavan on 2023/7/6.
//

#ifndef PROJECT2_PLAYER_H
#define PROJECT2_PLAYER_H

#include <string>
#include <vector>
#include <memory>

#include "Defines.h"
#include "Pet.h"
#include "Intention.h"

class Pet;

class Session;

class Intention;

class Player
{
public:
    using nameType = std::string;
    using petIndex = int;
    using petUPtr = std::unique_ptr<Pet>;
    using intentionUPtr = std::unique_ptr<Intention>;
    using playerT = ::player_T;

    Player();

    Player(nameType name, nameType nameMy);

    virtual ~Player();

    nameType name{};

    nameType nameMy{};

    void appendPet(petUPtr pet);

    Pet *getPet(petIndex index) const;

    Pet *getActingPet() const;

    void setActingPet(petIndex index);

    /// "You start with W", "Enemy send L"
    /// \param verb such as "start with", "send"
    void setActingPet(petIndex index, const std::string &verb);

    Pet *operator[](petIndex idx) const;

    petIndex size() const;

    Session *getSession() const;

    void setSession(Session *session);

    Player *getOpponent() const;

    void setOpponent(Player *opponent);

    Intention *getIntention() const;

    player_T type() const;

    bool hasOkPet() const;

    virtual void makeIntention() = 0;

    /* virtual */actIntentionResult_T actIntention();

    virtual void selectInitPet() = 0;

    void checkOkAndReplaceActingPet();

protected:
    /// Used in %checkOkAndReplaceActingPet()%. No "back" option.
    /// Calling this means the acting pet just went not OK, and should get the next choice of acting pet,
    /// which depends on user input or ai's logic.
    virtual petIndex getNextPetPreference() = 0;

protected:
    playerT typeID = playerT::ai;

    Session *_session = nullptr;
    Player *_opponent = nullptr;

    std::vector<petUPtr> _petList{};
    petIndex _actingIndex = 0;

    intentionUPtr _intention = nullptr;

    void setNextIntention(intentionUPtr newIntention) { _intention = std::move(newIntention); }

    struct potionCount
    {
        int revive = 1;
    };

    potionCount potionDict;
};


#endif //PROJECT2_PLAYER_H

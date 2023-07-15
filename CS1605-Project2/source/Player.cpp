//
// Created by Misakavan on 2023/7/6.
//

#include "Player.h"
#include "Intention.h"
#include "Session.h"

Player::Player() : Player("You", "Your")
{

}

Player::Player(Player::nameType name, Player::nameType nameMy) : name(std::move(name)), nameMy(std::move(nameMy))
{

}

Player::~Player()
{

}

void Player::appendPet(Player::petUPtr pet)
{
    pet->setOwner(this);
    _petList.push_back(std::move(pet));
}

Pet *Player::getPet(Player::petIndex index) const
{
    if (index < 0 || index >= _petList.size()) {
        return nullptr;
    }
    return _petList.at(index).get();
}

Pet *Player::getActingPet() const
{
    return getPet(_actingIndex);
}

void Player::setActingPet(Player::petIndex index)
{
    _actingIndex = index;
}

void Player::setActingPet(Player::petIndex index, const std::string &verb)
{
    setActingPet(index);
    if (getActingPet()) {
        const auto &petName = getActingPet()->getName();
        getSession()->getOs() << Player::name << " " << verb << " " << petName << "\n";
    }
}

Pet *Player::operator[](Player::petIndex idx) const
{
    return getPet(idx);
}

Player::petIndex Player::size() const
{
    return static_cast<petIndex>(_petList.size());
}

Session *Player::getSession() const
{
    return _session;
}

void Player::setSession(Session *session)
{
    _session = session;
}

Player *Player::getOpponent() const
{
    return _opponent;
}

void Player::setOpponent(Player *opponent)
{
    _opponent = opponent;
}

Intention *Player::getIntention() const
{
    return _intention.get();
}

bool Player::hasOkPet() const
{
    return std::any_of(
            _petList.cbegin(),
            _petList.cend(),
            [](const petUPtr &pet) { return pet->isOK(); });
}

player_T Player::type() const
{
    return typeID;
}

actIntentionResult_T Player::actIntention()
{
    using field = Intention::infoField;

    auto &os = getSession()->getOs();
    auto res = actIntentionResult_T::normal;
    auto intention = std::move(_intention);

    if (!intention) return actIntentionResult_T::normal;

    switch (intention->type) {
        case intention_T::skill: {
            auto *skill = std::get<field::infoSkill>(intention->info);
            skill->cast(getSession(), getOpponent());
        }
            break;
        case intention_T::change: {
            auto *nextPet = std::get<field::infoPet>(intention->info);
            int nextIndex = -1;
            for (int petIndex = 0; petIndex < _petList.size(); ++petIndex) {
                if (getPet(petIndex) == nextPet) {
                    nextIndex = petIndex;
                    break;
                }
            }
            setActingPet(nextIndex, "send");
        }
            break;
        case intention_T::potion: {
            auto *targetPet = std::get<field::infoPet>(intention->info);
            auto potionType = std::get<field::infoPotion>(intention->info);
            if (potionType == potion_T::revivePotion) {
                // Now revive the target Pet
                targetPet->revive();
                os << name << " uses revival potion on " << targetPet->getName() << endl;
                potionDict.revive--;
            }

        }
            break;
        case intention_T::escape:
            os << name << " Escape" << endl;
            res = actIntentionResult_T::escaped;
            break;
        case intention_T::null:
            break;
    }

    return res;
}

void Player::checkOkAndReplaceActingPet()
{
    if (getActingPet()->isOK()) {
        return;
    }
    auto &os = getSession()->getOs();
    // "Your G is beaten"
    os << asStrOwnerPair(getActingPet()) << " is beaten\n";
    if (!hasOkPet()) {
        setActingPet(-1);
        return;
    }
    // "Select your next pet (2 for L-HP**, 3 for G-HP**): *"
    int chosenIndex = getNextPetPreference();

    if (chosenIndex == -1) {
        // actually this player's pets are all out!
        return;
    }
    setNextIntention(makeChangeIntention(this, getPet(chosenIndex)));
    actIntention();
}





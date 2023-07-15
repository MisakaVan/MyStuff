//
// Created by Misakavan on 2023/7/6.
//

#include "Session.h"

Session::Session(Session::playerUPtr &&player1,
                 Session::playerUPtr &&player2) :
        _player1(std::move(player1)),
        _player2(std::move(player2))
{
    _player1->setSession(this);
    _player2->setSession(this);
    _player1->setOpponent(_player2.get());
    _player2->setOpponent(_player1.get());
}

int Session::run()
{
    initBattle();
    _currentRound = 0;
    bool exit = false;
    while (!exit) {
        /*  Phase 0  */
        _currentRound++;
        printVerticalLine();
        tickWeatherTime();
        _player1->getActingPet()->tickBuffLastingTime();
        _player2->getActingPet()->tickBuffLastingTime();

        os << "Round " << _currentRound << endl;

        /*  Phase 1  */
        _player1->makeIntention();
        _player2->makeIntention();

        /*  Phase 2  */
        auto result = bothPlayerAct();
        if (result == actIntentionResult_T::escaped)
            break;

        /*  Phase 3  */
        _player1->checkOkAndReplaceActingPet();
        _player2->checkOkAndReplaceActingPet();

        /*  Phase 4  */
        auto status = evaluateStatus();
        if (status == sessionStatus::ongoing) {
            printBothPetHp(this);
            continue;
        }
        // Exiting!
        exit = true;
        std::string msg =
                status == sessionStatus::draw
                ? "Draw"
                : _player1->name + " " + (status == sessionStatus::p1win ? "Win" : "Lose");
        os << msg << endl;
    }
    return 0;
}

void Session::initBattle()
{
    os << "Welcome to Battle of Pets 2!" << endl;
    _player2->selectInitPet();
    _player1->selectInitPet();
    const char *startPhrase1 = _player1->type() == player_T::user ? " start with " : " starts with ";
    const char *startPhrase2 = _player2->type() == player_T::user ? " start with " : " starts with ";
    os << _player1->name << startPhrase1 << _player1->getActingPet()->getName() << endl;
    os << _player2->name << startPhrase2 << _player2->getActingPet()->getName() << endl;
    os << "Battle starts!" << endl;
    printBothPetHp(this);
}

sessionStatus Session::evaluateStatus()
{
    if (_currentRound == MAX_ROUND_COUNT) {
        return sessionStatus::draw;
    }
    bool flag1 = _player1->hasOkPet();
    bool flag2 = _player2->hasOkPet();
    if (flag1) {
        return flag2
               ? sessionStatus::ongoing
               : sessionStatus::p1win;
    } else {
        return flag2
               ? sessionStatus::p2win
               : sessionStatus::draw;
    }
}

actIntentionResult_T Session::bothPlayerAct()
{
    const auto &intention1 = *_player1->getIntention();
    const auto &intention2 = *_player2->getIntention();
    auto *firstActor = (intention1 > intention2 ? _player1 : _player2).get();
    auto *secondActor = firstActor->getOpponent();

    auto result1 = firstActor->actIntention();
    if (result1 == actIntentionResult_T::escaped) {
        return actIntentionResult_T::escaped;
    }
    auto result2 = secondActor->actIntention();
    return result2;
}

void Session::setWeather(weather_T type, int time)
{
    _weather = type;
    _weatherTimeToLast = time;
}

void Session::tickWeatherTime()
{
    if (_weatherTimeToLast > 0) {
        _weatherTimeToLast--;
        if (_weatherTimeToLast == 0) {
            _weather = weather_T::normal;
            os << "Weather becomes normal" << endl;
        }
    }
}






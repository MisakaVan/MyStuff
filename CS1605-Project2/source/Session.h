//
// Created by Misakavan on 2023/7/6.
//

#ifndef PROJECT2_SESSION_H
#define PROJECT2_SESSION_H

#include <memory>
#include <iostream>
#include "Defines.h"
#include "Player.h"
#include "utilities.h"

enum class sessionStatus
{
    ongoing, p1win, p2win, draw
};

class Player;

class Session
{
    friend class Player;
    friend class UserPlayer;
    friend class EnemyPlayer1;
    friend class skillCollection::Sunny;
    friend class skillCollection::Rainy;

public:
    using playerUPtr = std::unique_ptr<Player>;
    using statusT = ::sessionStatus;

    Session(playerUPtr &&player1, playerUPtr &&player2);

    virtual ~Session() = default;

    int run();

    Player *getPlayer1() const { return _player1.get(); }

    Player *getPlayer2() const { return _player2.get(); }

    weather_T getWeather() const { return _weather; }

    unsigned int getRoundCount() const { return _currentRound; }

    auto getIs() -> std::istream & { return is; }

    auto getOs() -> std::ostream & { return os; }

    void setWeather(weather_T type, int time);

private:
    void initBattle();

    actIntentionResult_T bothPlayerAct();

    void tickWeatherTime();

    sessionStatus evaluateStatus();

private:
    std::istream &is = std::cin;
    std::ostream &os = std::cout;

    const unsigned int MAX_ROUND_COUNT = 100;

    unsigned int _currentRound = 0;

    weather_T _weather = weather_T::normal;

    int _weatherTimeToLast = 0;

    playerUPtr _player1 = nullptr;
    playerUPtr _player2 = nullptr;

};


#endif //PROJECT2_SESSION_H

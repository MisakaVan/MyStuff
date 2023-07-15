#include <iostream>
#include <memory>
#include "Session.h"
#include "UserPlayer.h"
#include "EnemyPlayer1.h"
#include "EnemyPlayer2.h"


#ifndef ENEMY_VERSION
#define ENEMY_VERSION 1
#endif

#if ENEMY_VERSION == 1
#define EnemyPlayer EnemyPlayer1
#elif ENEMY_VERSION == 2
#define EnemyPlayer EnemyPlayer2
#endif

int main()
{
    using namespace std;
    Session::playerUPtr user = make_unique<UserPlayer>();
//    Session::playerUPtr user = make_unique<EnemyPlayer1>("Sunny Team", "Sunny Team's");
//    Session::playerUPtr enemy = make_unique<UserPlayer>("TEST", "TEST's");
    Session::playerUPtr enemy = make_unique<EnemyPlayer>();
    auto session = make_unique<Session>(
            std::move(user), std::move(enemy)
    );
    return session->run();
}


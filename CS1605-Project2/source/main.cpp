#include "EnemyPlayer1.h"
#include "EnemyPlayer2.h"
#include "Session.h"
#include "UserPlayer.h"
#include <iostream>
#include <memory>

// #ifndef ENEMY_VERSION
// #define ENEMY_VERSION 1
// #endif

// #if ENEMY_VERSION == 1
// #define EnemyPlayer EnemyPlayer1
// #elif ENEMY_VERSION == 2
// #define EnemyPlayer EnemyPlayer2
// #endif

int main()
{
    using namespace std;
    auto msg = "Main menu:\n1: AI with Sunny Team;\n2: AI with Rainy Team;\n3: Watch both AI fight against each other ";
    auto selectedVersion = readInt(msg, {1, 2, 3});
    Session::playerUPtr user;
    Session::playerUPtr enemy;
    switch (selectedVersion)
    {
    case 1:
        user = make_unique<UserPlayer>();
        enemy = make_unique<EnemyPlayer1>();
        break;
    case 2:
        user = make_unique<UserPlayer>();
        enemy = make_unique<EnemyPlayer2>();
        break;
    case 3:
        user = make_unique<EnemyPlayer1>();
        enemy = make_unique<EnemyPlayer2>();
        break;
    default:
        break;
    }
    //    Session::playerUPtr user = make_unique<EnemyPlayer1>("Sunny Team", "Sunny Team's");
    //    Session::playerUPtr enemy = make_unique<UserPlayer>("TEST", "TEST's");
    auto session = make_unique<Session>(
        std::move(user), std::move(enemy));
    return session->run();
}

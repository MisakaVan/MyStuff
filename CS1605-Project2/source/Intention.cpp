//
// Created by Misakavan on 2023/7/6.
//

#include "Intention.h"

#include <utility>

Intention::Intention(player_T playerType,
                     intention_T intentionType,
                     int priority,
                     Intention::infoPack info):
                     playerType(playerType),
                     type(intentionType),
                     priority(priority),
                     info(std::move(info))
{

}

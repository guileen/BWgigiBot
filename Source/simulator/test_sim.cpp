#include <iostream>
#include "player.h"
#include "webdata.h"

using namespace BWSim;

int testAI(Game*, int);
int main() {
  BWMap bwmap = {4, {8,8,8,8,6,6,6,6,6,6,0}, {1, 1, 1, 1, 1, 1, 1, 0}, {{0x0104,10}, {0x0205,10}, {0x0306,10}, {0x0407,10}, {0x0405,10}, {0x0506,10}, {0x0607,10}, {0x0407,10}, {0x0406,15}, {0x0507,15}}};
  Game game({Races::Zerg, Races::Protoss}, &bwmap);
  for(int i=0;i<game.getPlayerCount();i++) {
    game.getPlayer(i)->printLog(std::cout);
  }
  for(int i=0;i<10;i++) {
    testAI(&game, 0);
    game.update(20);
    std::cout << "Frame:" << game.getFrames()
      << " 0M:" << game.getPlayer(0)->getMinerals()
      << " 0G:" << game.getPlayer(0)->getGas()
      << " 1M:" << game.getPlayer(1)->getMinerals()
      << " 1G:" << game.getPlayer(1)->getGas()
      << "\n";
  }
}

int testAI(Game* game, int myIndex) {
  //std::cout << "9, 7:" << (getSCVMineralsPer100F(9, 7) * MIN_N_100F) << "\n";
  //std::cout << "9, 17:" << (getSCVMineralsPer100F(9, 17) * MIN_N_100F) << "\n";
  //std::cout << "9, 27:" << (getSCVMineralsPer100F(9, 27) * MIN_N_100F) << "\n";
  //std::cout << "9, 37:" << (getSCVMineralsPer100F(9, 37) * MIN_N_100F) << "\n";
  auto me = game->getPlayer(myIndex);
  // find idle worker
  for(auto u: me->getUnits()) {
    if(u->type->isWorker && u->state == UnitStateIdle) {
      if(me->getMineralWorkers() < me->getMineralFields() ) {
        u->mineMinerals();
      } else if(me->getGasWorkers() < me->getGasRefineries() * 4) {
        u->mineGas();
      } else {
        u->mineMinerals();
      }
    }
    // Alpha-beta trim availableResActions
  }
  return 1;
}


#include <iostream>
#include "player.h"

using namespace BWSim;

int main() {
  Game* game = new Game({Races::Zerg, Races::Protoss});
  for(Player* p: game->getPlayers()) {
    p->printLog(std::cout);
  }
  /*
  for(;;) {
    game->update();
  }
  */
}

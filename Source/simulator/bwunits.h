#pragma once
#include "bwdata.h"
#include "action.h"
#include <deque>
#include <set>
#include <vector>

using namespace std;
namespace BWSim {

  class Game;
  class Player;
  class Unit {
    public:
      int id;
      Player* player;
      UnitType* type;

      bool isPowered=true;
      // basic
      int hitPoints;

      // build or morph
      UnitType* buildType;
      int buildRemainTime;
      bool cancelDestroy;
      Unit* buildWorker; // for terran
      bool isBuilding;

      // train
      std::deque<UnitType*> trainingQueue;
      int trainingRemainTime;
      bool isTraining;

      // research
      TechType* researchType;
      int researchRemainTime;

      // upgrade
      UpgradeType* upgradeType;
      int upgradeRemainTime;

      // larvas
      std::set<Unit*> larvas;
      int larvasHatchRemainTime;
      Unit* hatchery;
      void doneBuild(UnitType* type);
    public:
      // ...
      void update();
      // different unit have different build action.
      // lurker
      void gather();
      void returnCargo();
      void morph(const UnitType* type);
      void build(const UnitType* type);
      void train(const UnitType* type);
      void research(const TechType* type);
      void upgrade(const UpgradeType* type);
      void castAction(Action* action);
      void canCastAction(Action* action);
  };

}

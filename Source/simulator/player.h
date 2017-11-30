#pragma once

#include "bwunits.h"

using namespace std;
namespace BWSim {

  class Player {
    private:
      Game* game;
      Races::Enum race;
      int maxUnitId;
      set<Unit*> units;
      // dynamic update
      int unitCounts[UnitTypeMax];
      int enabledTechs[TechTypeMax];
      int upgrades[UpgradeTypeMax];
      // TODO simplify to pure data struct, array.
      // struct PlayerData{
      //    int unitCount;
      //    UnitData units[];
      //    int enabledTechs[]
      //    int upgrades[]
      // }
      // PlayerData data;
      set<int> availableCreateBuildings;
      set<int> availableResearchTechs;
      set<int> availableUpgrades;
      set<int> availableCreateMen;
      // command center
      set<Unit*> commandCenters;
      map<Unit*, int> mineralFields;
      map<Unit*, int> gasFields;
      map<Unit*, set<Unit*>> gasRefineries;
      map<Unit*, set<Unit*>> mineralWorkers;
      map<Unit*, set<Unit*>> gasWorkers;
      double gasPerSecond;
      double mineralsPerSecond;

      void updateAvailabelActions();
    public:
      int usedSupplies;
      int totalSupplies;
      int maxSupplies=200;
      int minerals=400;
      int gas=0;

      Player(Game* game, Races::Enum race);
      inline bool checkBuildOrTrain(const UnitType* type);
      inline bool checkResearch(const TechType* type);
      inline bool checkUpgrade(const UpgradeType* type, int level=1);
      inline void completeUnit(Unit* unit);
      Unit* createUnit(const UnitType* type);
      void destroyUnit(Unit* unit);
      const Unit* selectIdelWorker();
      const Unit* selectActionUnit(int actionType, int actionId);
      bool anyUnitCanCastAction(int actionType, int actionId);
      bool unitCanCastAction(Unit* unit, int actionType, int actionId);
      bool unitCastAction(Unit* unit, int actionType, int actionId);
      vector<Action> allAvailableActions();
      void doneReasearch(const TechType* type) {
        enabledTechs[type->id] = true;
      }
      void doneUpgrade(const UpgradeType* type) {
        upgrades[type->id]++;
      }
      void printLog(ostream& out);
      void update() {
        for(auto u: units) {
          u->update();
        }
      }
  };

  class Game {
    private:
      vector<Player*> players;
      // map
    public:
      Game(vector<Races::Enum> races) {
        for(Races::Enum r : races) {
          players.push_back(new Player(this, r));
        }
      }
      const vector<Player*> getPlayers() {return players;}
      void update() {
        for(auto p:players) {
          // check eliminated
          p->update();
        }
        // check win
      }
  };
}

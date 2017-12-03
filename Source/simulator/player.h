#pragma once

#include "bwmap.h"
#include "bwunits.h"

using namespace std;
namespace BWSim {

  class Player {
    private:
      Game* game;
      Races::Enum race;
      int startRegion;
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
      // new Player(PlayerData data);
      // command center
      set<int> commandCenterRegions;
      int mineralFields=0;
      int gasFields=0;
      int gasRefineries=0;
      int mineralWorkers=0;
      int gasWorkers=0;
      double gasPer100F=0;
      double mineralsPer100F=0;
      // --
      double minerals=50;
      double gas=0;

      int frames=0;
      void updateAvailabelActions();
    public:
      int usedSupplies;
      int totalSupplies;
      set<int> availableResActions;

      Player(Game* game, Races::Enum race, int region);
      inline int getMineralFields() {return mineralFields;}
      inline int getGasFields() {return gasFields;}
      inline int getGasRefineries() {return gasRefineries;}
      inline int getMineralWorkers() {return mineralWorkers;}
      inline int getGasWorkers() {return gasWorkers;}
      inline double getGasPer100F() {return gasPer100F;}
      inline double getMineralsPer100F() {return mineralsPer100F;}
      inline int getUsedSupplies() {return usedSupplies;}
      inline int getTotalSupplies() {return totalSupplies;}
      inline double getMinerals() {return minerals;}
      inline double getGas() {return gas;}
      void incrMineralWorkers(int=1);
      void incrGasWorkers(int=1);

      inline bool checkBuildOrTrain(const UnitType* type);
      inline bool checkResearch(const TechType* type);
      inline bool checkUpgrade(const UpgradeType* type, int level=1);
      inline void completeUnit(Unit* unit, int region=-1);
      void updateWorkersCount();
      Unit* createUnit(const UnitType* type, int region=-1);
      void destroyUnit(Unit* unit);
      const Unit* selectIdelWorker();
      const Unit* selectActionUnit(int actionType, int actionId);
      inline const set<Unit*> getUnits() {
        return units;
      }
      bool anyUnitCanCastAction(int actionType, int actionId);
      bool unitCanCastAction(Unit* unit, int actionType, int actionId);
      bool unitCastAction(Unit* unit, int actionType, int actionId);
      void doneReasearch(const TechType* type) {
        enabledTechs[type->id] = true;
      }
      void doneUpgrade(const UpgradeType* type) {
        upgrades[type->id]++;
      }
      void printLog(ostream& out);
      void update(int n) {
        for(auto u: units) {
          u->update(n);
        }
        this->minerals += this->mineralsPer100F * n / 100.0;
        this->gas += this->gasPer100F * n / 100.0;
        frames+=n;
      }
  };

  class Game {
    private:
      vector<Player*> players;
      BWMap* map;
      int frames;
    public:
      Game(vector<Races::Enum> races, BWMap* map) {
        this->map = map;
        int i=0;
        for(Races::Enum r : races) {
          players.push_back(new Player(this, r, i++));
        }
      }
      Player* getPlayer(int i) {return players[i];}
      int getPlayerCount() {return players.size();}
      inline const BWMap* getMap() {
        return map;
      }
      void update(int n) {
        frames+=n;
        for(auto p:players) {
          // check eliminated
          p->update(n);
        }
        // check win
      }
      inline int getFrames() {return frames;}
  };
}

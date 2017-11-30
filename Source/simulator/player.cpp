#include <iostream>
#include <ostream>
#include "player.h"
namespace BWSim {

  Player::Player(Game* game, Races::Enum race) {
    this->game = game;
    this->race = race;
    // init race ...
    if(race == Races::Zerg) {
      this->createUnit(&Unit_Zerg_Hatchery);
      this->createUnit(&Unit_Zerg_Drone);
      this->createUnit(&Unit_Zerg_Drone);
      this->createUnit(&Unit_Zerg_Drone);
      this->createUnit(&Unit_Zerg_Drone);
    } else if(race == Races::Terran) {
      this->createUnit(&Unit_Terran_Command_Center);
      this->createUnit(&Unit_Terran_SCV);
      this->createUnit(&Unit_Terran_SCV);
      this->createUnit(&Unit_Terran_SCV);
      this->createUnit(&Unit_Terran_SCV);
    } else if(race == Races::Protoss) {
      this->createUnit(&Unit_Protoss_Nexus);
      this->createUnit(&Unit_Protoss_Probe);
      this->createUnit(&Unit_Protoss_Probe);
      this->createUnit(&Unit_Protoss_Probe);
      this->createUnit(&Unit_Protoss_Probe);
    }
  }

  Unit* Player::createUnit(const UnitType* type) {
    Unit* unit = new Unit();
    unit->type = (UnitType*)type;
    this->units.insert(unit);
    usedSupplies += type->supplyRequired;
    completeUnit(unit);
    return unit;
  }

  void Player::completeUnit(Unit* unit) {
    if(unit->type->requiresPsi) {
      // check psi
    }
    unit->isPowered = true;
    unit->hitPoints = unit->type->maxHitPoints;
    totalSupplies += unit->type->supplyProvided;
    if(unit->type->producesLarva) {
      int n = this->units.size() == 1 ? 3 : 1;
      for(int i=0;i<n;i++) unit->larvas.insert(this->createUnit(&Unit_Zerg_Larva));
    }
    unitCounts[unit->type->id]++;
    updateAvailabelActions();
  }

  inline bool Player::checkBuildOrTrain(const UnitType* type) {
    if(type->isHero || type->isSpecialBuilding || type->id==Unit_Zerg_Egg.id || type->id == Unit_Zerg_Larva.id) return false;
    for(int t: type->requiredUnits) {
      // utype = (UnitType*)&AllUnitTypes[t];
      if(t!= Unit_None.id && !unitCounts[t]) {
        //std::cout << type->name << " require " << t << ":" << AllUnitTypes[t].name << " count:" << unitCounts[t] << "\n";
        return false;
      }
    }
    if(type->requiresPsi && !unitCounts[Unit_Protoss_Pylon.id]) {
      //std::cout << type->name << " requirePsi pylon count:" << unitCounts[Unit_Protoss_Pylon.id] << "\n";
      return false;
    }
    if(type->requiredTech != Tech_None.id && !enabledTechs[type->requiredTech]) {
      //std::cout << type->name << " require Tech " << AllTechTypes[type->requiredTech].name << " enabled:" << enabledTechs[type->requiredTech] << "\n";
      return false;
    }
    if(type->supplyRequired > (totalSupplies-usedSupplies)) {
      //std::cout << type->name << " require supplies " << type->supplyRequired << " used:" << usedSupplies << " total:" << totalSupplies << "\n";
      return false;
    }
    if(type->whatBuilds == Unit_None.id) return false;
    if(unitCounts[type->whatBuilds] < type->whatBuildsNeed) {
      //std::cout << type->name << " whatBuilds: " << AllUnitTypes[type->whatBuilds].name << " need:" << type->whatBuildsNeed << " but:" << unitCounts[type->whatBuilds] << "\n";
      return false; 
    }
    return true;
  }

  inline bool Player::checkResearch(const TechType* type) {
    if(enabledTechs[type->id]) return false;
    if(type->requiredUnit && !unitCounts[type->requiredUnit]) return false;
    if(type->whatResearches && !unitCounts[type->whatResearches]) return false; 
    return true;
  }

  inline bool Player::checkUpgrade(const UpgradeType* type, int level) {
    if(level > type->maxRepeats || level <= upgrades[type->id]) return false;
    int req = type->whatsRequired[level-1];
    if(req && !unitCounts[req]) return false;
    if(type->whatUpgrades && !unitCounts[type->whatUpgrades]) return false; 
    return true;
  }

  void Player::updateAvailabelActions() {
    // update availableResearchTechs
    // update availableUpgrads;
    // update availableCreate Units
    UnitType* utype;
    //UnitType* utype;
    availableCreateBuildings.empty();
    for(int i=0;i<UnitTypeMax-2;i++) {
      utype = (UnitType*)&AllUnitTypes[i];
      if(checkBuildOrTrain(utype)) {
        if(utype->isBuilding) {
          availableCreateBuildings.insert(i);
        } else {
          availableCreateMen.insert(i);
        }
      }
    }
    TechType* ttype;
    for(int i=0;i<TechTypeMax-2;i++) {
      ttype = (TechType*)&AllTechTypes[i];
      if(checkResearch(ttype)) {
        availableResearchTechs.insert(i);
      }
    }
    UpgradeType* uptype;
    for(int i=0;i<UpgradeTypeMax-2;i++) {
      uptype = (UpgradeType*)&AllUpgradeTypes[i];
      if(checkUpgrade(uptype)) {
        availableUpgrades.insert(i);
      }
    }

  }

  void Player::printLog(ostream& out) {
    out << "UnitCounts:\n";
    for(int i=0;i<UnitTypeMax;i++) {
      if(unitCounts[i]>0) {
        out << "  " << i << ":" << AllUnitTypes[i].name << ":" << unitCounts[i] << "\n";
      }
    }
    out << "AvailableBuildings:\n";
    for(int id: availableCreateBuildings) {
      out << "  " << id << ":" << AllUnitTypes[id].name << " from " << AllUnitTypes[AllUnitTypes[id].whatBuilds].name << "\n";
    }
    out << "AvailableResearchs:\n";
    for(int id: availableResearchTechs) {
      out << "  " << id << ":" << AllTechTypes[id].name << " from " << AllUnitTypes[AllTechTypes[id].whatResearches].name << "\n";
    }
    out << "AvailableUpgrades:\n";
    for(int id: availableUpgrades) {
      out << "  " << id << ":" << AllTechTypes[id].name << " from " << AllUnitTypes[AllUpgradeTypes[id].whatUpgrades].name << "\n";
    }
    out << "AvailableTrain:\n";
    for(int id: availableCreateMen) {
      out << "  " << id << ":" << AllUnitTypes[id].name << " from " << AllUnitTypes[AllUnitTypes[id].whatBuilds].name << "\n";
    }
  }

  //vector<Action> Player::allAvailableActions() {

  //}

}

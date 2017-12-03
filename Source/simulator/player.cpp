#include <iostream>
#include <ostream>
#include "player.h"
#include "webdata.h"
namespace BWSim {

  Player::Player(Game* game, Races::Enum race, int region) {
    this->game = game;
    this->race = race;
    this->startRegion = region;
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

  Unit* Player::createUnit(const UnitType* type, int region) {
    if(region == -1) region = startRegion;
    Unit* unit = new Unit();
    unit->type = (UnitType*)type;
    unit->player = this;
    this->units.insert(unit);
    usedSupplies += type->supplyRequired;
    completeUnit(unit, region);
    return unit;
  }

  void Player::completeUnit(Unit* unit, int region) {
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
    if(unit->type->id == Unit_Terran_Command_Center.id ||
        unit->type->id == Unit_Protoss_Nexus.id ||
        unit->type->id == Unit_Zerg_Hatchery.id ||
        unit->type->id == Unit_Zerg_Lair.id ||
        unit->type->id == Unit_Zerg_Hive.id
        ) {
      // command center
      this->commandCenterRegions.insert(region);
      this->gasFields += this->game->getMap()->regionGasFields[region];
      this->mineralFields += this->game->getMap()->regionMineralFields[region];
    }
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
    availableResActions.empty();
    for(int i=0;i<UnitTypeMax-2;i++) {
      utype = (UnitType*)&AllUnitTypes[i];
      if(checkBuildOrTrain(utype)) {
        availableResActions.insert(makeResActionId(BWTypeUnit, i));
      }
    }
    TechType* ttype;
    for(int i=0;i<TechTypeMax-2;i++) {
      ttype = (TechType*)&AllTechTypes[i];
      if(checkResearch(ttype)) {
        availableResActions.insert(makeResActionId(BWTypeTech, i));
      }
    }
    UpgradeType* uptype;
    for(int i=0;i<UpgradeTypeMax-2;i++) {
      uptype = (UpgradeType*)&AllUpgradeTypes[i];
      if(checkUpgrade(uptype)) {
        availableResActions.insert(makeResActionId(BWTypeUpgrade, i));
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
    out << "allAvailableActions:\n";
    for(int resActionId: availableResActions) {
      BWType bwType = getBWTypeOfResActionId(resActionId);
      int id = getTypeIdOfResActionId(resActionId);
      switch(bwType) {
        case BWTypeUnit:
          out << "  Unit" << id << ":" << AllUnitTypes[id].name << " from " << AllUnitTypes[AllUnitTypes[id].whatBuilds].name << "\n";
          break;
        case BWTypeTech:
          out << "  Tech" << id << ":" << AllTechTypes[id].name << " from " << AllUnitTypes[AllTechTypes[id].whatResearches].name << "\n";
          break;
        case BWTypeWeapon:
          out << "Weapon";
          break;
        case BWTypeUpgrade:
          out << "  Upgrade" << id << ":" << AllTechTypes[id].name << " from " << AllUnitTypes[AllUpgradeTypes[id].whatUpgrades].name << "\n";
          break;
        default:
          break;
      }
    }
  }

  void Player::incrMineralWorkers(int i) {
    mineralWorkers+=i;
    // TODO: M/s should be statistic in real game.
    // update M/s
    mineralsPer100F = getSCVMineralsPer100F(mineralFields, mineralWorkers);
  }

  void Player::incrGasWorkers(int i) {
    gasWorkers+=i;
    // TODO: G/s should be statistic in real game.
    // update G/s
    gasPer100F = getAnyGasPer100F(gasFields, gasWorkers);
  }

}

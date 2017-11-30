#include "action.h"

namespace BWSim {

  void getActionTypeFor(BWType type, int typeId, Action* action) {
    action->casterNeed = 1;
    if(type == BWTypeUpgrade) {
      action->actionType = ActionUpgrade;
      action->casterUnitType = (UnitType*) &AllUnitTypes[AllUpgradeTypes[typeId].whatUpgrades];
      action->actionId = typeId;
      return;
    }
    if(type == BWTypeTech) {
      action->actionType = ActionResearch;
      action->casterUnitType = (UnitType*) &AllUnitTypes[AllTechTypes[typeId].whatResearches];
      action->actionId = typeId;
      return;
    }
    if(type != BWTypeUnit) {
      // Unknown.
      printf("Unknown Action for type %d\n", type);
      return;
    }
    // BWTypeUnit
    const UnitType* utype = &AllUnitTypes[typeId];
    UnitType* builder = (UnitType*) &AllUnitTypes[utype->whatBuilds];
    if(utype->isBuilding) {
      if(utype->isAddon) {
        action->casterUnitType = builder;
        action->actionType = ActionBuildAddon;
        action->actionId = typeId;
        return;
      }
      if(utype->getRace == Races::Zerg && builder->isBuilding) {
        action->casterUnitType = builder;
        action->actionType = ActionMorph;
        action->actionId = typeId;
        return;
      }
      if(builder->isWorker) {
        action->casterUnitType = builder;
        action->actionType = ActionBuild;
        action->actionId = typeId;
      }
      // Unknow
      printf("Error: %d of %d", type, typeId);
      return;
    }
    printf("builder of %s is %s", utype->name, builder->name);
    if(builder->isBuilding) {
      action->casterUnitType = builder;
      action->actionType = ActionBuild;
      action->actionId = typeId;
      return;
    }
    if(utype->id == Unit_Terran_Siege_Tank_Siege_Mode.id) {
      action->casterUnitType = (UnitType*)&Unit_Terran_Siege_Tank_Tank_Mode;
      action->actionId = typeId;
      // TODO better type.
      action->actionType = ActionSiege;
      return;
    }
    if(builder->id == Unit_Protoss_Reaver.id || builder->id == Unit_Protoss_Carrier.id) {
      action->casterUnitType = builder;
      action->actionId = typeId;
      action->actionType = ActionTrain;
      return;
    }
    if(utype->id == Unit_Zerg_Infested_Command_Center.id) {
      action->casterUnitType = (UnitType*)&Unit_Zerg_Queen;
      action->actionId = typeId;
      action->actionType = ActionSpell;
      return;
    }
    action->casterUnitType = builder;
    action->casterNeed = utype->whatBuildsNeed;
    action->actionId = typeId;
    action->actionType = ActionMorph;
    return;;
  };
}

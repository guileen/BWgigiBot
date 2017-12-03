#pragma once
#include "bwdata.h"
using namespace std;
namespace BWSim {

  enum ActionType {
    ActionBuild=0,
    ActionResearch,
    ActionUpgrade,
    ActionTrain,
    ActionMorph,
    ActionBuildAddon,
    ActionSpell,
    ActionSiege,
    ActionMineMinerals,
    ActionMineGas,
    ActionMax
  };

  enum BWType {
    BWTypeNone=0,
    BWTypeUnit,
    BWTypeTech,
    BWTypeUpgrade,
    BWTypeWeapon,
    BWTypeMax
  };

  inline int makeResActionId(BWType bwType, int typeId) {
    return bwType << 16 | typeId;
  }

  inline BWType getBWTypeOfResActionId(int resActionId) {
    return BWType(resActionId >> 16);
  }

  inline int getTypeIdOfResActionId(int resActionId) {
    return resActionId & 0xffff;
  }

  typedef struct Action {
    UnitType* casterUnitType;
    int casterNeed;
    ActionType actionType;
    int actionId;
  } Action;

  void getActionTypeFor(BWType type, int typeId, Action* action);

  inline BWType getActionTargetType(ActionType type) {
    switch(type) {
      case ActionBuild:
      case ActionBuildAddon:
      case ActionTrain:
      case ActionMorph:
      case ActionSiege:
      case ActionMineGas:
      case ActionMineMinerals:
        return BWTypeUnit;
      case ActionResearch:
      case ActionSpell:
        return BWTypeTech;
      case ActionUpgrade:
        return BWTypeUpgrade;
      case ActionMax:
        return BWTypeNone;
    }
  }

}

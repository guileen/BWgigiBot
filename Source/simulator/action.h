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
    ActionMax
  };

  enum BWType {
    BWTypeUnit=0,
    BWTypeTech,
    BWTypeUpgrade,
    BWTypeWeapon,
    BWTypeMax
  };

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
        return BWTypeUnit;
      case ActionResearch:
      case ActionSpell:
        return BWTypeTech;
      case ActionUpgrade:
        return BWTypeUpgrade;
      case ActionMax:
        return BWTypeMax;
    }
  }

}

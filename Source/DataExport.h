#include <stdlib.h>
#include <string>
#include "BWAPI.h"
#include <iostream>
#include <fstream>

using namespace std;

void exportUpgrades(ofstream&hout, ofstream& out) {
  // export Upgrades
  hout << "typedef struct {\n\
  int id;\n\
  char name[50];\n\
  int gasPrice[10];\n\
  int gasPriceFactor;\n\
  int getRace;\n\
  int maxRepeats;\n\
  int mineralPrice[10];\n\
  int mineralPriceFactor;\n\
  int upgradeTime[10];\n\
  int upgradeTimeFactor;\n\
  int whatsRequired[10];\n\
  int whatUpgrades;\n\
  set<int> whatUses;\n\
  ";
  hout << "} UpgradeType;\n";
  int enumMax = BWAPI::UpgradeTypes::Enum::MAX;
  for(int i=0;i<enumMax;i++) {
    auto e = BWAPI::UpgradeTypes::Enum::Enum(i);
    auto t = BWAPI::UpgradeType(e);
    if(t.getName() == "") {
      continue;
    }
    hout << "extern const UpgradeType& Upgrade_" << t.getName() << ";\n";
    out << "const UpgradeType& Upgrade_" << t.getName() << " = {"
      << t.getID() << ",\""
      << t.getName() << "\",{";
    for(int l=1;l<=t.maxRepeats();l++) {
      out << t.gasPrice(l) << ",";
    }
    out << "},"
      << t.gasPriceFactor() << ","
      << t.getRace().getID() << ","
      << t.maxRepeats() << ",{";
    for(int l=1;l<=t.maxRepeats();l++) {
      out << t.mineralPrice(l) << ",";
    }
    out << "},"
      << t.mineralPriceFactor() << ",{";
    for (int l=1;l<=t.maxRepeats();l++) {
      out << t.upgradeTime(l) << ",";
    }
    out << "},"
      << t.upgradeTimeFactor() << ",{";
    for (int l=1;l<=t.maxRepeats();l++) {
        out << t.whatsRequired(l).getID() << ",";
    }
    out << "},"
      << t.whatUpgrades().getID() << ",{";
    for (auto x:t.whatUses()) {
      out << x.getID() << ",";
    }
    out << "}};\n";
  }
  hout<< "#define UpgradeTypeMax " << enumMax << "\n";
  hout<< "extern const UpgradeType AllUpgradeTypes[" << enumMax << "];\n";
  out<< "const UpgradeType AllUpgradeTypes[" << enumMax << "] = {";
  for(int i=0;i<enumMax;i++) {
    auto e = BWAPI::UpgradeTypes::Enum::Enum(i);
    auto t = BWAPI::UpgradeType(e);
    if (t.getName() == "") {
      out << "Upgrade_Unknown,";
    } else {
      out << "Upgrade_" << t.getName() << ",";
    }
  }
  out << "};\n";
}

void exportTechTypes(ofstream& hout, ofstream& out) {
  // export Techs
  hout << "typedef struct {\n\
  int id;\n\
  char name[50];\n\
  int energyCost;\n\
  int gasPrice;\n\
  int getOrder;\n\
  int getRace;\n\
  int getWeapon;\n\
  int mineralPrice;\n\
  int requiredUnit;\n\
  int researchTime;\n\
  bool targetsPosition;\n\
  bool targetsUnit;\n\
  int whatResearches;\n\
  set<int> whatUses;\n";
  hout << "} TechType;\n";
  int enumMax = BWAPI::TechTypes::Enum::MAX;
  for(int i=0;i<enumMax;i++) {
    auto e = BWAPI::TechTypes::Enum::Enum(i);
    auto t = BWAPI::TechType(e);
    if(t.getName() == "") {
      continue;
    }
    hout << "extern const TechType& Tech_" << t.getName() << ";\n";
    out << "const TechType& Tech_" << t.getName() << " = {"
      << t.getID() << ",\""
      << t.getName() << "\","
      << t.energyCost() << ","
      << t.gasPrice() << ","
      << t.getOrder().getID() << ","
      << t.getRace().getID() << ","
      << t.getWeapon().getID() << ","
      << t.mineralPrice() << ","
      << t.requiredUnit().getID() << ","
      << t.researchTime() << ","
      << t.targetsPosition() << ","
      << t.targetsUnit() << ","
      << t.whatResearches().getID() << ",{";
    for(auto x:t.whatUses()) {
      out << x.getID() << ",";
    }
    out << "}";
    out << "};\n";
  }
  hout<< "#define TechTypeMax " << enumMax << "\n";
  hout<< "extern const TechType AllTechTypes[" << enumMax << "];\n";
  out<< "const TechType AllTechTypes[" << enumMax << "] = {";
  for(int i=0;i<enumMax;i++) {
    auto e = BWAPI::TechTypes::Enum::Enum(i);
    auto t = BWAPI::TechType(e);
    if(t.getName() == "") {
      out << "Tech_Unknown,";
    } else {
      out << "Tech_" << t.getName() << ",";
    }
  }
  out << "};\n";
}

void exportUnitTypes(ofstream& hout, ofstream& out) {
  hout << "typedef struct{\n\
    int id;\n\
    char name[50];\n\
    set<int> abilities;\n\
    int acceleration;\n\
    int airWeapon;\n\
    int armor;\n\
    int armorUpgrade;\n\
    int buildScore;\n\
    set<int> buildsWhat;\n\
    int buildTime;\n\
    bool canAttack;\n\
    bool canBuildAddon;\n\
    bool canMove;\n\
    bool canProduce;\n\
    int cloakingTech;\n\
    int destroyScore;\n\
    int dimensionDown;\n\
    int dimensionLeft;\n\
    int dimensionRight;\n\
    int dimensionUp;\n\
    int gasPrice;\n\
    int getRace;\n\
    int groundWeapon;\n\
    int haltDistance;\n\
    bool hasPermanentCloak;\n\
    int height;\n\
    bool isAddon;\n\
    bool isBeacon;\n\
    bool isBuilding;\n\
    bool isBurrowable;\n\
    bool isCloakable;\n\
    bool isCritter;\n\
    bool isDetector;\n\
    bool isFlagBeacon;\n\
    bool isFlyer;\n\
    bool isFlyingBuilding;\n\
    bool isHero;\n\
    bool isInvincible;\n\
    bool isMechanical;\n\
    bool isMineralField;\n\
    bool isNeutral;\n\
    bool isOrganic;\n\
    bool isPowerup;\n\
    bool isRefinery;\n\
    bool isResourceContainer;\n\
    bool isResourceDepot;\n\
    bool isRobotic;\n\
    bool isSpecialBuilding;\n\
    bool isSpell;\n\
    bool isSpellcaster;\n\
    bool isTwoUnitsInOneEgg;\n\
    bool isWorker;\n\
    int maxAirHits;\n\
    int maxEnergy;\n\
    int maxGroundHits;\n\
    int maxHitPoints;\n\
    int maxShields;\n\
    int mineralPrice;\n\
    bool producesCreep;\n\
    bool producesLarva;\n\
    bool regeneratesHP;\n\
    int requiredTech;\n\
    set<int> requiredUnits;\n\
    bool requiresCreep;\n\
    bool requiresPsi;\n\
    set<int> researchesWhat;\n\
    int seekRange;\n\
    int sightRange;\n\
    int size;\n\
    int spaceProvided;\n\
    int spaceRequired;\n\
    int supplyProvided;\n\
    int supplyRequired;\n\
    int tileHeight;\n\
    int tileWidth;\n\
    double topSpeed;\n\
    int turnRadius;\n\
    set<int> upgrades;\n\
    set<int> upgradesWhat;\n\
    int whatBuilds;\n\
    int whatBuildsNeed;\n\
    int width;\n";
  hout << "} UnitType;\n";
  int enumMax = BWAPI::UnitTypes::Enum::MAX;
  for (int i=0;i<enumMax;i++) {
    auto e = BWAPI::UnitTypes::Enum::Enum(i);
    auto t = BWAPI::UnitType(e);
    hout << "extern const UnitType Unit_" << t.getName() << ";\n";
    out << "const UnitType Unit_" << t.getName() << " = {"
      << t.getID() << ",\""
      << t.getName() << "\","
      << "{";
    for (auto x: t.abilities()) {
      out << x.getID() << ",";
    }
    out<< "}," << t.acceleration() << ","
      << t.airWeapon().getID() << ","
      << t.armor() << ","
      << t.armorUpgrade().getID() << ","
      << t.buildScore() << ",{";
    for(auto x: t.buildsWhat()) {
      out<< x.getID() << ",";
    }
    out <<"}," << t.buildTime() << ","
      << t.canAttack() << ","
      << t.canBuildAddon() << ","
      << t.canMove() << ","
      << t.canProduce() << ","
      << t.cloakingTech().getID() << ",";
    out << t.destroyScore() << ","
      << t.dimensionDown() << ","
      << t.dimensionLeft() << ","
      << t.dimensionRight() << ","
      << t.dimensionUp() << ",";
    out << t.gasPrice() << ","
      << t.getRace().getID() << ","
      << t.groundWeapon().getID() << ",";

    out << t.haltDistance() << ","
      << t.hasPermanentCloak() << ","
      << t.height() << ",";
    out << t.isAddon() << ","
      << t.isBeacon() << ","
      << t.isBuilding() << ","
      << t.isBurrowable() << ","
      << t.isCloakable() << ","
      << t.isCritter() << ","
      << t.isDetector() << ","
      << t.isFlagBeacon() << ","
      << t.isFlyer() << ","
      << t.isFlyingBuilding() << ","
      << t.isHero() << ","
      << t.isInvincible() << ","
      << t.isMechanical() << ","
      << t.isMineralField() << ","
      << t.isNeutral() << ","
      << t.isOrganic() << ","
      << t.isPowerup() << ","
      << t.isRefinery() << ","
      << t.isResourceContainer() << ","
      << t.isResourceDepot() << ","
      << t.isRobotic() << ","
      << t.isSpecialBuilding() << ","
      << t.isSpell() << ","
      << t.isSpellcaster() << ","
      << t.isTwoUnitsInOneEgg() << ","
      << t.isWorker() << ",";
    out << t.maxAirHits() << ","
      << t.maxEnergy() << ","
      << t.maxGroundHits() << ","
      << t.maxHitPoints() << ","
      << t.maxShields() << ","
      << t.mineralPrice() << ",";
    out << t.producesCreep() << ","
      << t.producesLarva() << ","
      << t.regeneratesHP() << ","
      << t.requiredTech().getID() << ",{";
    for(auto x:t.requiredUnits()) {
      out << x.first.getID() << ",";
    }
    out << "},"
      << t.requiresCreep() << ","
      << t.requiresPsi() << ",{";
    for(auto x:t.researchesWhat()) {
      out << x.getID() << ",";
    }
    out<< "},"
      << t.seekRange() << ","
      << t.sightRange() << ","
      << t.size().getID() << ","
      << t.spaceProvided() << ","
      << t.spaceRequired() << ","
      << t.supplyProvided() << ","
      << t.supplyRequired() << ",";
    out << t.tileHeight() << ","
      << t.tileWidth() << ","
      << t.topSpeed() << ","
      << t.turnRadius() << ",{";

    for(auto x: t.upgrades()) {
      out << x.getID() << ",";
    }
    out << "},{";
    for(auto x: t.upgradesWhat()) {
      out << x.getID() << ",";
    }
    out << "},"
      << t.whatBuilds().first.getID() << ","
      << t.whatBuilds().second << ","
      << t.width() << "};\n";
  }
  hout<< "#define UnitTypeMax " << enumMax << "\n";
  hout<< "extern const UnitType AllUnitTypes[" << enumMax << "];\n";
  out<< "const UnitType AllUnitTypes[" << enumMax << "] = {";
  for (int i=0;i<enumMax;i++) {
    auto e = BWAPI::UnitTypes::Enum::Enum(i);
    auto t = BWAPI::UnitType(e);
    out << "Unit_" << t.getName() << ",";
  }
  out << "};\n";
}

void exportWeaponTypes(ofstream& hout, ofstream& out) {
  // export Techs
  hout << "typedef struct {\n\
  int id;\n\
  char name[50];\n\
  int damageAmount;\n\
  int damageBonus;\n\
  int damageCooldown;\n\
  int damageFactor;\n\
  int damageType;\n\
  int explosionType;\n\
  int getTech;\n\
  int innerSplashRadius;\n\
  int maxRange;\n\
  int medianSplashRadius;\n\
  int minRange;\n\
  int outerSplashRadius;\n\
  bool targetsAir;\n\
  bool targetsGround;\n\
  bool targetsMechanical;\n\
  bool targetsNonBuilding;\n\
  bool targetsNonRobotic;\n\
  bool targetsOrganic;\n\
  bool targetsOrgOrMech;\n\
  bool targetsOwn;\n\
  bool targetsTerrain;\n\
  int upgradeType;\n\
  int whatUses;\n";
  hout << "} WeaponType;\n";
  int enumMax = BWAPI::WeaponTypes::Enum::MAX;
  for(int i=0;i<enumMax;i++) {
    auto e = BWAPI::WeaponTypes::Enum::Enum(i);
    auto t = BWAPI::WeaponType(e);
    if(t.getName() == "") {
      continue;
    }
    hout << "extern const WeaponType& Weapon_" << t.getName() << ";\n";
    out << "const WeaponType& Weapon_" << t.getName() << " = {"
      << t.getID() << ",\""
      << t.getName() << "\","
      << t.damageAmount() << ","
      << t.damageBonus() << ","
      << t.damageCooldown() << ","
      << t.damageFactor() << ","
      << t.damageType().getID() << ","
      << t.explosionType().getID() << ","
      << t.getTech().getID() << ","
      << t.innerSplashRadius() << ","
      << t.maxRange() << ","
      << t.medianSplashRadius() << ","
      << t.minRange() << ","
      << t.outerSplashRadius() << ","
      << t.targetsAir() << ","
      << t.targetsGround() << ","
      << t.targetsMechanical() << ","
      << t.targetsNonBuilding() << ","
      << t.targetsNonRobotic() << ","
      << t.targetsOrganic() << ","
      << t.targetsOrgOrMech() << ","
      << t.targetsOwn() << ","
      << t.targetsTerrain() << ","
      << t.upgradeType().getID() << ","
      << t.whatUses().getID() << ",";
    out << "};\n";
  }
  hout<< "#define WeaponTypeMax " << enumMax << "\n";
  hout<< "extern const WeaponType AllWeaponTypes[" << enumMax << "];\n";
  out<< "const WeaponType AllWeaponTypes[" << enumMax << "] = {";
  for(int i=0;i<enumMax;i++) {
    auto e = BWAPI::WeaponTypes::Enum::Enum(i);
    auto t = BWAPI::WeaponType(e);
    if(t.getName() == "") {
      out << "Weapon_Unknown,";
    } else {
      out << "Weapon_" << t.getName() << ",";
    }
  }
  out << "};\n";
}

void exportDamageTypesEnum(ofstream& hout) {
  hout << "namespace DamageTypes { enum Enum { Independent, Explosive, Concussive, Normal, Ignore_Armor, None, Unknown, MAX};}\n";
}

void exportUnitCommandTypesEnum(ofstream& hout) {
  hout << "namespace UnitCommandTypes { enum Enum { Attack_Move = 0, Attack_Unit, Build, Build_Addon, Train, Morph, Research, Upgrade, Set_Rally_Position, Set_Rally_Unit, Move, Patrol, Hold_Position, Stop, Follow, Gather, Return_Cargo, Repair, Burrow, Unburrow, Cloak, Decloak, Siege, Unsiege, Lift, Land, Load, Unload, Unload_All, Unload_All_Position, Right_Click_Position, Right_Click_Unit, Halt_Construction, Cancel_Construction, Cancel_Addon, Cancel_Train, Cancel_Train_Slot, Cancel_Morph, Cancel_Research, Cancel_Upgrade, Use_Tech, Use_Tech_Position, Use_Tech_Unit, Place_COP, None, Unknown, MAX };}\n";
}

void exportUnitSizeTypesEnum(ofstream& hout) {
  hout << "namespace UnitSizeTypes { enum Enum { Independent = 0, Small, Medium, Large, None, Unknown, MAX };}\n";
}

void exportRacesEnum(ofstream& hout) {
  hout << "namespace Races { enum Enum { Zerg = 0, Terran, Protoss, Other, Unused, Select, Random, None, Unknown, MAX };}\n";
}

int exportDataAndExit(string hpath, string cpppath) {
  ofstream out, hout;
  out.open(cpppath);
  hout.open(hpath);
  hout <<
    "#pragma once\n"
    "#include <set>\n"
    "#include <map>\n"
    "#include <array>\n"
    "using namespace std;\n"
    "namespace BWSim {\n";
  out <<
    "#include \"bwdata.h\"\n"
    "using namespace std;\n"
    "namespace BWSim {\n";
  exportUnitTypes(hout, out);
  exportTechTypes(hout, out);
  exportUpgrades(hout, out);
  exportWeaponTypes(hout, out);
  exportDamageTypesEnum(hout);
  exportRacesEnum(hout);
  exportUnitCommandTypesEnum(hout);
  exportUnitSizeTypesEnum(hout);
  hout << "}\n";
  out << "}\n";
  hout.close();
  out.close();
  exit(EXIT_FAILURE);
  return 0;
}


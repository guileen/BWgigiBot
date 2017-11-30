#include "bwunits.h"
#include "player.h"
#include <iostream>
namespace BWSim {

  void Unit::doneBuild(UnitType* type) {
    this->type = type;
    this->isPowered = true;
    this->hitPoints = type->maxHitPoints;
  }

  void Unit::update() {
    std::cout << this->type->name << isPowered << buildRemainTime << trainingRemainTime << researchRemainTime << "\n";
    if(!isPowered) {
      return;
    }
    // update build
    if(buildType) {
      buildRemainTime--;
      if(buildRemainTime<=0) {
        // done build
        this->doneBuild(buildType);
        buildType = NULL;
        buildRemainTime = 0;
        // return worker
        if(this->buildWorker) {
          buildWorker->isBuilding = false;
        }
      }
    }

    // update research
    if(researchType) {
      researchRemainTime --;
      if(researchRemainTime<=0) {
        // research done.
        player->doneReasearch(researchType);
        researchType = NULL;
      }
    }

    // update upgrade
    if(upgradeType) {
      upgradeRemainTime --;
      if(upgradeRemainTime<=0) {
        // upgrade done.
        player->doneUpgrade(upgradeType);
        researchType = NULL;
      }
    }

    // update train
    if(trainingRemainTime) {
      if(!isTraining) {
        auto trainType = trainingQueue.front();
        if(player->usedSupplies + trainType->supplyRequired < player->totalSupplies) {
          isTraining = true;
        }
      }
      if(isTraining) {
        trainingRemainTime --;
        if(trainingRemainTime<=0) {
          auto trainType = trainingQueue.front();
          player->createUnit(trainType);
          trainingQueue.pop_front();
          // trainNext
          trainType = trainingQueue.front();
          if(trainType) {
            trainingRemainTime = trainType->buildTime;
            isTraining = false;
          } else {
            trainingRemainTime = 0;
          }
        }
      }
    }

    // update larvas
    if(this->type->producesLarva && larvas.size() < 3) {
      larvasHatchRemainTime --;
      if(larvasHatchRemainTime <= 0) {
        Unit* u = player->createUnit(&Unit_Zerg_Larva);
        larvas.insert(u);
        u->hatchery = this;
        // TODO fix larvas hatch time
        larvasHatchRemainTime = 20;
      }
    }
  }

  void Unit::morph(const UnitType* type) {
    // check can morph to
    if(this->hatchery) {
      this->hatchery = NULL;
      this->hatchery->larvas.erase(this);
    }
    buildType = (UnitType*)type;
    buildRemainTime = type->buildTime;
  }

}

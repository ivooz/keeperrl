#include "stdafx.h"
#include "util.h"
#include "creature.h"
#include "collective_config.h"

AttractionInfo::AttractionInfo(MinionAttraction a, double cl, double min, bool mand)
  : attraction(a), amountClaimed(cl), minAmount(min), mandatory(mand) {}

template <class Archive>
void ImmigrantInfo::serialize(Archive& ar, const unsigned int version) {
  ar& SVAR(id)
    & SVAR(frequency)
    & SVAR(attractions)
    & SVAR(traits)
    & SVAR(spawnAtDorm)
    & SVAR(salary)
    & SVAR(techId)
    & SVAR(limit)
    & SVAR(groupSize)
    & SVAR(autoTeam);
}

SERIALIZABLE(ImmigrantInfo);

template <class Archive>
void AttractionInfo::serialize(Archive& ar, const unsigned int version) {
  ar& SVAR(attraction)
    & SVAR(amountClaimed)
    & SVAR(minAmount)
    & SVAR(mandatory);
}

SERIALIZABLE(AttractionInfo);
SERIALIZATION_CONSTRUCTOR_IMPL(AttractionInfo);

template <class Archive>
void PopulationIncrease::serialize(Archive& ar, const unsigned int version) {
  ar& SVAR(type)
    & SVAR(increase)
    & SVAR(minSize)
    & SVAR(oneTime);
}

SERIALIZABLE(PopulationIncrease);

CollectiveConfig::CollectiveConfig(double freq, int payoutT, double payoutM, vector<ImmigrantInfo> im,
    CollectiveType t, int maxPop, vector<PopulationIncrease> popInc)
    : immigrantFrequency(freq), payoutTime(payoutT), payoutMultiplier(payoutM),
    maxPopulation(maxPop), populationIncreases(popInc), immigrantInfo(im), type(t) {}

CollectiveConfig CollectiveConfig::keeper(double freq, int payout, double payoutMult, int maxPopulation,
    vector<PopulationIncrease> increases, vector<ImmigrantInfo> im) {
  return CollectiveConfig(freq, payout, payoutMult, im, KEEPER, maxPopulation, increases);
}

CollectiveConfig CollectiveConfig::withImmigrants(double frequency, int maxPopulation, vector<ImmigrantInfo> im) {
  return CollectiveConfig(frequency, 0, 0, im, VILLAGE, maxPopulation, {});
}

CollectiveConfig CollectiveConfig::noImmigrants() {
  return CollectiveConfig(0, 0, 0, {}, VILLAGE, 10000, {});
}

bool CollectiveConfig::getManageEquipment() const {
  return type == KEEPER;
}

bool CollectiveConfig::getWorkerFollowLeader() const {
  return type == KEEPER;
}

bool CollectiveConfig::sleepOnlyAtNight() const {
  return type != KEEPER;
}

double CollectiveConfig::getImmigrantFrequency() const {
  return immigrantFrequency;
}

int CollectiveConfig::getPayoutTime() const {
  return payoutTime;
}
 
double CollectiveConfig::getPayoutMultiplier() const {
  return payoutMultiplier;
}

bool CollectiveConfig::getStripSpawns() const {
  return type == KEEPER;
}

bool CollectiveConfig::getFetchItems() const {
  return type == KEEPER;
}

bool CollectiveConfig::getEnemyPositions() const {
  return type == KEEPER;
}

bool CollectiveConfig::getWarnings() const {
  return type == KEEPER;
}

bool CollectiveConfig::getConstructions() const {
  return type == KEEPER;
}

int CollectiveConfig::getMaxPopulation() const {
  return maxPopulation;
}

const vector<ImmigrantInfo>& CollectiveConfig::getImmigrantInfo() const {
  return immigrantInfo;
}

const vector<PopulationIncrease>& CollectiveConfig::getPopulationIncreases() const {
  return populationIncreases;
}


template <class Archive>
void CollectiveConfig::serialize(Archive& ar, const unsigned int version) {
  ar& SVAR(immigrantFrequency)
    & SVAR(payoutTime)
    & SVAR(payoutMultiplier)
    & SVAR(maxPopulation)
    & SVAR(populationIncreases)
    & SVAR(immigrantInfo)
    & SVAR(type);
}

SERIALIZABLE(CollectiveConfig);
SERIALIZATION_CONSTRUCTOR_IMPL(CollectiveConfig);


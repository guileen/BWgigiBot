#define LAVAR_GENERATE_FRAMES 342
const double MAX_GAS_PER_100F = 21.6;
const double MAX_MINFIELD_PER_100F = 10.0;
const double MINE_OVER_RATIO = 0.6;
const double SCV_MINE_PER_100F = 4.53;
const double Drone_MINE_PER_100F = 4.67;
const double Probe_MINE_PER_100F = 4.74;
const double ANY_GAS_PER_100F = 7.2;
const double MIN_N_100F = 14.36;

//const double GAS_PER_MIN 103.0
//const double SCV_PER_MIN 65.0
//const double SCV_PER_MIN_OVER 39.0
//const double Drone_PER_MIN 67.1
//const double Probe_PER_MIN 68.1
inline double min(double a, double b) {
  return a < b ? a : b;
}

inline double getAnyGasPer100F(int refineries, int workers) {
  return min(MAX_GAS_PER_100F * refineries, workers*ANY_GAS_PER_100F);
}

inline double getSCVMineralsPer100F(int mineralFields, int workers) {
  return min(MAX_MINFIELD_PER_100F*mineralFields,
      min(workers, mineralFields)*SCV_MINE_PER_100F +
      (workers>mineralFields ?
       (workers-mineralFields) * MINE_OVER_RATIO * SCV_MINE_PER_100F:
       0));
}

inline double getProbeMineralsPer100F(int mineralFields, int workers) {
  return min(MAX_MINFIELD_PER_100F*mineralFields,
      min(workers, mineralFields)*Probe_MINE_PER_100F +
      (workers>mineralFields ?
       (workers-mineralFields) * MINE_OVER_RATIO * Probe_MINE_PER_100F:
       0));
}

inline double getDroneMineralsPer100F(int mineralFields, int workers) {
  return min(MAX_MINFIELD_PER_100F*mineralFields,
      min(workers, mineralFields)*Drone_MINE_PER_100F +
      (workers>mineralFields ?
       (workers-mineralFields) * MINE_OVER_RATIO * Drone_MINE_PER_100F:
       0));
}

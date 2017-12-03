#include <map>

namespace BWSim {
  typedef struct BWMap{
      // map base regions, static analysis
      int startRegions;
      // regionPosition  X Y
      // region -> mineralFieldsCount.
      int regionMineralFields[20];
      // region -> gasFieldsCount.
      int regionGasFields[20];
      // (region << 8 | region) -> distance
      std::map<int, int> regionDistances;
  } BWMap;
}

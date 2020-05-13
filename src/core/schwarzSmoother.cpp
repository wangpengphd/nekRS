#include "schwarzSmoother.hpp"
#include "nekInterfaceAdapter.hpp"
#include <vector>
void reconfigurePressureSolver(elliptic_t* pSolver){
  open_logfile();
  auto Nelements = pSolver->mesh->Nelements;
  auto mglevels = pSolver->precon->parAlmond->levels;
  for(int level = 0; level < pSolver->nLevels; ++level){
    MGLevel* curr_level = dynamic_cast<MGLevel*>(mglevels[level]);
    if(curr_level){
      auto N = pSolver->levels[level]+1;
      auto N2 = (N+2)*(N+2);
      auto N3 = N2*(N+2);
      std::vector<dfloat> Sx(N2*Nelements);
      std::vector<dfloat> Sy(N2*Nelements);
      std::vector<dfloat> Sz(N2*Nelements);
      std::vector<dfloat> D(N3*Nelements);
      std::vector<dfloat> wts(N*N*4*3*Nelements);
      get_nek_operators(Sx.data(), Sy.data(), Sz.data(), D.data(), wts.data(), 3-level);
      curr_level->fdm_op->build(Sx.data(), Sy.data(), Sz.data(), D.data(), wts.data());
      output_nek_operators(Sx.data(), Sy.data(), Sz.data(), D.data(), wts.data(), 3-level);
    }
  }
  close_logfile();
}
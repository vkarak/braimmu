#ifndef BRAIN_H
#define BRAIN_H

#include <vector>
#include "stdio.h"
#include "stdlib.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>

#include "nifti1.h"
#include "nifti1_io.h"
#include "znzlib.h"

#include "pointers.h"
#include "input.h"
#include "memory.h"
#include "init.h"
#include "comm.h"
#include "output.h"
#include "region.h"

namespace brain_NS {

class Brain {
 public:
  Brain(int, char **, int, int);
  ~Brain();

  void allocations();
  void destroy();

  // run functions
  void integrate(int);
  void update();
  void derivatives();
  int find_id(int, int, int);

  // classes
  Input *input;
  Memory *memory;
  Init *init;
  Comm *comm;
  Output *output;
  Region *region;

  int me,nproc;

  friend class Input;

  int *npart;
  int *nv, *nvl; // number of voxels in each dimension, global and local
  tagint nvoxel; // total number of voxels
  int nlocal, nghost, nall; // number of voxels for each core, local/ghost/all

  int step, Nrun, Nlog; // step, number of steps, log output

  double dt;  // timestep
  int nevery; // period of neural activity = nevery timesteps
  double *boxlo,*boxhi,*lbox; // box boundaries, and size
  double *xlo,*xhi; // boundaries for each partition

  double **x; // voxel position
  double vlen, vlen_1, vlen_2, vvol, vvol_1; // voxel size

  std::vector<int> type, group;
  std::vector<bool> is_loc; // local voxel = 1, ghost voxel = 0

  tagint *tag; // tag of each voxel

  int num_neigh_max, num_conn_max; // maximum number of neighbors/connections
  int *num_neigh, *num_conn; // number of neighbors, number of long-range connections
  int **neigh; // neighbor ids for each voxel
  tagint **conn; // connection tags for each voxel

  /// MRI image variables
  nifti_image *nim;

  bool newton_flux;

  /// model parameters
  double init_val[num_agents];
  std::array<std::vector<double>, num_agents> agent, deriv;
  std::array<std::vector<double>, ndim> Dtau; // diffusion tensor for tau protein
  double Dtau_max, diff_tau; // maximum diffusion of tau protein
  double dnt; // neuronal death rate due to tau accumulation
  double D_sAb, diff_sAb; // diffusivity of sAb
  double D_mic, diff_mic; // diffusivity of microglia
  double cs, sens_s, cf, sens_f; // microglia chemotaxis sensitivity
  double kp, kn; // rate of polymerization and nucleation
  double ds,df; // clearance rate of sAb and fAb by microglia
  double es; // rate of sAb efflux in CSF
  double Ha; // Michaelis-Menten constant for astrogliosis
  double ka; // rate of astrogliosis
  double C_cir, c_cir, tau_cir, omega_cir; // circadian rhythm parameters
  double ktau; // rate of tau tangle formation from phosphorylated tau
  double kphi; // phosphorylation rate of tau proteins due to F and N
  double ephi; // rate of phosphorylated-tau efflux in CSF

  MPI_Comm world;

};

}

#endif

#include <fstream>
#include <ctime>

#include "md_vortex.h"

using namespace md_vortex;

// material dependent properties
constexpr const double RESTIVITY                      = 18.; // Ohm*nm
constexpr const double Vortex::T0_COHERENCE_LENGTH    = 38; // nm
constexpr const double Vortex::T0_PENETRATION_DEPTH   = 39; // nm
constexpr const double T0_LINE_ENERGY = MAG_FLUX_QUANTUM*MAG_FLUX_QUANTUM/(4*PI*Vortex::T0_PENETRATION_DEPTH*Vortex::T0_PENETRATION_DEPTH);

// system parameters
constexpr const int RIGHT_WALL                        = Vortex::T0_PENETRATION_DEPTH;
constexpr const int TOP_WALL                          = Vortex::T0_PENETRATION_DEPTH*128;
constexpr const int BOTTOM_WALL                       = 0;
constexpr const int NUCLEATION_STRIP_WIDTH            = Vortex::T0_PENETRATION_DEPTH;
constexpr const int MAX_NUCEALTIONS                   = 100;
constexpr const double GRAD_T                         = 1/(Vortex::T0_PENETRATION_DEPTH*3);

// define temperature map. Arguments to TemperatureMap are coefficients of
// polynomial of order sizeof...(args). Arguments must be rvalue since
const Temperature Vortex::temperature = TemperatureMap(std::forward<const double>(GRAD_T),0.);

// define algorithm for updating steps
template <typename F, typename E>
  struct EulerStepper
{
    using Force = F;
    using Energy = E;

    static PositionVector step(Vortex * vortex,const double& tstep)
    {
        PositionVector pos = vortex->get_pos();
        ForceVector force = vortex->get_force();

        double invXi = vortex->get_inv_coherence_length();
        constexpr double T0_MOBILITY = 2.*PI*RESTIVITY/(MAG_FLUX_QUANTUM*MAG_FLUX_QUANTUM);
        double mobility = T0_MOBILITY/(invXi*invXi);
        return force*mobility*tstep;
    }
};

// ************************************************************************
int main()
{
    // construct vortex lattice with pre-defined interactions
    using InteractionForce = interactions::TempDependentForce;
    using InteractionEnergy = interactions::TempDependentEnergy;
    VortexLattice<EulerStepper<InteractionForce,InteractionEnergy>> lattice;

    // defining boundary conditions
    typedef ReflectiveBoundaryCondition ReflectiveBC;
    auto right_wall = std::make_shared<ReflectiveBC>(
      ReflectiveBC(RIGHT_WALL,Orientation::VERTICAL,Comparator::GREATER_THAN));
    auto top_wall = std::make_shared<ReflectiveBC>(
      ReflectiveBC(TOP_WALL,Orientation::HORIZONTAL,Comparator::GREATER_THAN));
    auto bottom_wall = std::make_shared<ReflectiveBC>(
      ReflectiveBC(BOTTOM_WALL,Orientation::HORIZONTAL,Comparator::LESS_THAN));
    lattice << right_wall << top_wall << bottom_wall;

    // add external forces
    auto thermal_gradient = std::make_unique<ForceVector>(
      ForceVector(T0_LINE_ENERGY*GRAD_T,0));
    lattice << std::move(thermal_gradient);

    // add vortices
    auto init_vort1 = new Vortex(RIGHT_WALL/2.,TOP_WALL/2.);
    lattice << init_vort1;

    // simulations
    double * chemical_potential = static_cast<double*> (
      malloc(sizeof(double)*MAX_NUCEALTIONS));
    long seed = -time(NULL);
    double time_step = .01;

    for (size_t n_nucleations = 0; n_nucleations < MAX_NUCEALTIONS; n_nucleations++)
    {
        // generate vortex at random location in NUCLEATION_STRIP.
        size_t nucleation_pos_x = ran1(RIGHT_WALL - NUCLEATION_STRIP_WIDTH,RIGHT_WALL,&seed);
        size_t nucleation_pos_y = ran1(BOTTOM_WALL,TOP_WALL,&seed);
        lattice << new Vortex(nucleation_pos_x,nucleation_pos_y);

        // step the dynamics until energy doesn't change (delta_E/this->energy() < tolerance = 0.01)
        auto [n_attempts,delta_E] = lattice.equilibrate(0.01,time_step);

        double new_energy = lattice.energy();
        printf("%3zuth at (%3zu,%3zu) delta_E: %.2e E: %.2e  attmps: %2zu  tstep: %.2e.\n"
              ,n_nucleations,nucleation_pos_x,nucleation_pos_y, delta_E,new_energy,n_attempts,time_step);
    }

    std::ofstream out("a.csv");
    lattice.dump(out);
    free(chemical_potential);

    return 0;
}

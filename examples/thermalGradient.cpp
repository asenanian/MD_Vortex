#include <fstream>
#include <ctime>

#include "md_vortex.h"
#include <array>

using namespace md_vortex;

// material dependent properties
constexpr const double RESTIVITY                      = 18.; // Ohm*nm
constexpr const double Vortex::T0_COHERENCE_LENGTH    = 40; // nm
constexpr const double Vortex::T0_PENETRATION_DEPTH   = 40; // nm
constexpr const double T0_LINE_ENERGY = MAG_FLUX_QUANTUM*MAG_FLUX_QUANTUM/(4*PI*Vortex::T0_PENETRATION_DEPTH*Vortex::T0_PENETRATION_DEPTH);

// system parameters
constexpr const int RIGHT_WALL                        = Vortex::T0_PENETRATION_DEPTH*1;
constexpr const int TOP_WALL                          = Vortex::T0_PENETRATION_DEPTH*128;
constexpr const int BOTTOM_WALL                       = 0;
constexpr const int NUCLEATION_STRIP_WIDTH            = Vortex::T0_PENETRATION_DEPTH/4;
constexpr const int MAX_NUCEALTIONS                   = 20;
constexpr const int MAX_REALIZATIONS                  = 1;
constexpr const double GRAD_T                         = 1/(1000*Vortex::T0_PENETRATION_DEPTH);

// define temperature map. Arguments to TemperatureMap are coefficients of
// polynomial of order sizeof...(args). Arguments must be rvalue
const Temperature Vortex::temperature = TemperatureMap(std::forward<const double>(-GRAD_T),1.);

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
      ReflectiveBC(RIGHT_WALL,Orientation::VERTICAL,Comparator::LESS_THAN));
    auto top_wall = std::make_shared<ReflectiveBC>(
      ReflectiveBC(TOP_WALL,Orientation::HORIZONTAL,Comparator::GREATER_THAN));
    auto bottom_wall = std::make_shared<ReflectiveBC>(
      ReflectiveBC(BOTTOM_WALL,Orientation::HORIZONTAL,Comparator::LESS_THAN));
    lattice << right_wall << top_wall << bottom_wall;

    // add external forces
    auto thermal_gradient = std::make_unique<ForceVector>(
      ForceVector(-T0_LINE_ENERGY*GRAD_T,0));
    lattice << std::move(thermal_gradient);

    // ************************************************************************
    std::array<double,MAX_NUCEALTIONS> ext_field;
    ext_field.fill(0.);

    long seed = -time(NULL);
    double time_step = .005;
    std::ofstream out("a.csv");

    for (size_t n_realizations = 0; n_realizations < MAX_REALIZATIONS; n_realizations++)
    {
    // reset
    lattice.clearVortices();

    // add vortices
    double nucleation_pos_x = ran1(RIGHT_WALL,RIGHT_WALL + NUCLEATION_STRIP_WIDTH,&seed);
    double nucleation_pos_y = TOP_WALL/2.;
    auto init_vort1 = new Vortex(nucleation_pos_x,nucleation_pos_y);

    lattice << init_vort1;
    ext_field[0] = lattice.energy();

    printf("0th at (%3f,%4f)\n",nucleation_pos_x,nucleation_pos_y);

        for (size_t n_nucleations = 1; n_nucleations < MAX_NUCEALTIONS; n_nucleations++)
        {
            // generate vortex at random location in NUCLEATION_STRIP.
            double nucleation_pos_x = ran1(RIGHT_WALL, RIGHT_WALL + NUCLEATION_STRIP_WIDTH,&seed);
            double nucleation_pos_y = TOP_WALL/2.;

            double energy_eq = lattice.energy();
            lattice << new Vortex(nucleation_pos_x,nucleation_pos_y);

            // step the dynamics until energy doesn't change (delta_E/this->energy() < tolerance = 0.01)
            auto [n_attempts,delta_E] = lattice.equilibrate(0.01,time_step);
            double new_energy = lattice.energy();
            ext_field[n_nucleations] += 2*PERMEABILITY*(new_energy - energy_eq)/MAG_FLUX_QUANTUM;

            printf("%3zuth at (%3.0f,%4.0f) E: %.2e  attmps: %2zu  tstep: %.2e.\n"
                  ,n_nucleations,nucleation_pos_x,nucleation_pos_y,new_energy,n_attempts,time_step);
        }
        lattice.dump(out);
    }
    out << ext_field[MAX_NUCEALTIONS-1]/MAX_REALIZATIONS << "," << 0 << std::endl;
    std::cout << "Ext Field: " << ext_field[MAX_NUCEALTIONS-1]/MAX_REALIZATIONS << std::endl;


    return 0;
}

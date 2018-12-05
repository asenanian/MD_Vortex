#include "md_vortex.h"

using namespace md_vortex;

// system parameters
const int RIGHT_WALL                        = 128;
const int LEFT_WALL                         = 0;
const int TOP_WALL                          = 128;
const int BOTTOM_WALL                       = 0;
const int NUCLEATION_STRIP_WIDTH            = 16;

// material dependent properties
const double RESTIVITY                      = 18.; // Ohm*nm
const double Vortex::T0_COHERENCE_LENGTH    = 40; // nm
const double Vortex::T0_PENETRATION_DEPTH   = 40; // nm

// optional
Vortex::Temperature Vortex::temperature = TemperatureMap({0.,0.});

int main()
{
    struct Stepper
    {
        static PositionVector step(Vortex * vortex,const double& tstep)
        {
            PositionVector pos = vortex->get_pos();
            ForceVector force = vortex->get_force();

            double invXi = vortex->get_inv_coherence_length();
            double mobility = 2*PI*RESTIVITY/(invXi*MAG_FLUX_QUANTUM*MAG_FLUX_QUANTUM);
            return force*mobility*tstep;
        }
    };

    VortexLattice lattice;

    // defining boundary conditions
    typedef bc::ReflectiveBoundaryCondition ReflectiveBC;
    auto right_wall = std::make_shared<ReflectiveBC>(
      ReflectiveBC(RIGHT_WALL,bc::Orientation::VERTICAL,bc::Comparator::GREATER_THAN));
    auto top_wall = std::make_shared<ReflectiveBC>(
      ReflectiveBC(TOP_WALL,bc::Orientation::HORIZONTAL,bc::Comparator::GREATER_THAN));
    auto bottom_wall = std::make_shared<ReflectiveBC>(
      ReflectiveBC(BOTTOM_WALL,bc::Orientation::HORIZONTAL,bc::Comparator::LESS_THAN));
    lattice << right_wall << top_wall << bottom_wall;

    // add external forces
    double gradT = 100.1;
    double thermal_gradient_force_mag = MAG_FLUX_QUANTUM*MAG_FLUX_QUANTUM/(4*PI*PERMEABILITY*Vortex::T0_PENETRATION_DEPTH*Vortex::T0_PENETRATION_DEPTH);
    auto thermal_gradient = std::make_unique<ForceVector>(
      ForceVector(thermal_gradient_force_mag*gradT,0));
    lattice << std::move(thermal_gradient);

    // define vortex-vortex interactions
    using Force = interactions::TempDependentForce;
    using Energy = interactions::TempDependentEnergy;

    // choose integration algorithm
    Stepper stepper;

    // add vortices
    auto init_vort1 = new Vortex(127,15);
    auto init_vort2 = new Vortex(126,65);
    lattice << init_vort1 << init_vort2;

    const int LATTICE_SIZE = TOP_WALL - BOTTOM_WALL;
    long seed = -4245;
    int steps = 0;
    int n_nucleations = 0;
    double time_step = .01;
    double energy = lattice.Energy<Energy>();
    std::cout << energy << std::endl;
    lattice.Dump();


    for (;;)
    {
        steps++;
        lattice.Step<Force,Stepper>(time_step);

        if ( steps % 100 == 0 )
        {
            double new_energy = lattice.Energy<Energy>();
            double delta_E = energy - new_energy;
            if ( delta_E/energy < 1 && delta_E > 0 )
            // If energy changes by less than .1 percent over 1000 time steps
            // add a new vortex along NUCLEATION_STRIP.
            {
                n_nucleations++;
                std::cout << n_nucleations << ": " << delta_E/energy << std::endl;
                int nucleation_pos_x = ran1(RIGHT_WALL - NUCLEATION_STRIP_WIDTH,RIGHT_WALL,&seed);
                int nucleation_pos_y = ran1(BOTTOM_WALL,LATTICE_SIZE,&seed);
                lattice << new Vortex(nucleation_pos_x,nucleation_pos_y);
                energy = lattice.Energy<Energy>();
                //lattice.dump();
            }
        }
        if ( n_nucleations > 100 ) break;
    }
    lattice.Dump();
    return 0;
}

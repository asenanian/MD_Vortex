#include "md_vortex.h"

int main()
{
    typedef bc::ReflectiveBoundaryCondition ReflectiveBC;

    VortexLattice lattice;

    // defining boundary conditions
    auto right_wall = std::make_shared<ReflectiveBC>(
      ReflectiveBC(RIGHT_WALL,bc::Orientation::VERTICAL,bc::Comparator::GREATER_THAN));
    auto top_wall = std::make_shared<ReflectiveBC>(
      ReflectiveBC(TOP_WALL,bc::Orientation::HORIZONTAL,bc::Comparator::GREATER_THAN));
    auto bottom_wall = std::make_shared<ReflectiveBC>(
      ReflectiveBC(BOTTOM_WALL,bc::Orientation::HORIZONTAL,bc::Comparator::LESS_THAN));

    // implementing boundary conditions to system
    lattice << right_wall << top_wall << bottom_wall;

    // add vortices
    auto init_vort1 = new Vortex(127,15);
    auto init_vort2 = new Vortex(126,65);
    lattice << init_vort1 << init_vort2;

    lattice.dump();

    // add external forces
    double gradT = 0.01;
    auto thermal_gradient = std::make_unique<force_vector>(
      force_vector(gradT,0));
    lattice << std::move(thermal_gradient);

    const int LATTICE_SIZE = TOP_WALL - BOTTOM_WALL;
    long seed = -42453;
    int steps = 0;
    int n_nucleations = 0;
    double energy = lattice.energy();

    for (;;)
    {
        steps++;
        lattice.step();

        if ( steps % 1000 == 0 )
        {
            lattice.dump();
            double new_energy = lattice.energy();
            double delta_E = energy - new_energy;
            if ( delta_E/energy < .5 )
            // If energy changes by less than .1 percent over 1000 time steps
            // add a new vortex along NUCLEATION_STRIP.
            {
                int nucleation_pos_x = ran1(RIGHT_WALL - NUCLEATION_STRIP_WIDTH,RIGHT_WALL,&seed);
                int nucleation_pos_y = ran1(BOTTOM_WALL,LATTICE_SIZE,&seed);
                lattice << new Vortex(nucleation_pos_x,nucleation_pos_y);
                n_nucleations++;
            }
            energy = new_energy;
        }
        if ( n_nucleations > 100 ) break;
    }
    lattice.dump();
    return 0;
}

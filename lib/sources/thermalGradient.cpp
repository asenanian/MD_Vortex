#include "utilities.h"
#include "vortex_lattice.h"
#include "boundary_condition.h"
#include "vec2d.h"
#include "vortex.h"

int main()
{
    typedef bc::ReflectiveBoundaryCondition Wall;

    VortexLattice lattice;

    // defining boundary conditions
    auto right_wall = std::make_shared<Wall>(
      Wall(RIGHT_WALL,bc::Orientation::VERTICAL,bc::Comparator::GREATER_THAN));
    auto top_wall = std::make_shared<Wall>(
      Wall(TOP_WALL,bc::Orientation::HORIZONTAL,bc::Comparator::GREATER_THAN));
    auto bottom_wall = std::make_shared<Wall>(
      Wall(BOTTOM_WALL,bc::Orientation::HORIZONTAL,bc::Comparator::LESS_THAN));

    // implementing bc's to system
    lattice << right_wall;
    lattice << top_wall;
    lattice << bottom_wall;

    // add vortices
    Vortex * init_vort1 = new Vortex(17,15);
    Vortex * init_vort2 = new Vortex(26,67);
    lattice << init_vort1 << init_vort2;

    const int LATTICE_SIZE = TOP_WALL - BOTTOM_WALL;
    double gradT = 0.01;
    long seed = -42453;
    int steps = 0;
    int n_nucleations = 0;
    double energy = lattice.energy();

    for (;;)
    {
        steps++;
        lattice.step(gradT);

        if ( steps % 1000 == 0 )
        {
            double new_energy = lattice.energy();
            double delta_E = energy - new_energy;
            //std::cout << delta_E << std::endl;
            if ( delta_E/energy < .5 )
            {
                int nucleation_pos_x = ran1(RIGHT_WALL - NUCLEATION_STRIP_WIDTH,RIGHT_WALL,&seed);
                int nucleation_pos_y = ran1(BOTTOM_WALL,LATTICE_SIZE,&seed);
                lattice << new Vortex(nucleation_pos_x,nucleation_pos_y);
                n_nucleations++;
            }
            energy = new_energy;
        }
        if ( n_nucleations > 400 ) break;
    }
    lattice.dump();
    return 0;
}

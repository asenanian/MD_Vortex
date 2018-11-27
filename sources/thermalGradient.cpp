#include "utilities.h"
#include "vortex_lattice.h"
#include "vec2d.h"
#include "vortex.h"

int main()
{
    const int LATTICE_SIZE = TOP_WALL - BOTTOM_WALL;
    VortexLattice lattice = VortexLattice(LATTICE_SIZE,LATTICE_SIZE);
    lattice.nucleateVortex(128,0);
    lattice.nucleateVortex(127,7);

    double gradT = 0.1;
    long seed = -424523;
    int steps = 0;
    int n_nucleations = 0;
    double energy = lattice.energy();
    for (;;)
    {
        steps++;
        lattice.step(gradT);

        if ( steps % 100 == 0 )
        {
            double new_energy = lattice.energy();
            if ( abs((new_energy - energy)/energy) < .1 )
            {
                int nucleation_pos = static_cast<int>(LATTICE_SIZE*ran1(&seed)) + BOTTOM_WALL;
                lattice.nucleateVortex(128,nucleation_pos);
                std::cout << "Vortex nucleated.\n";
                lattice.dump();
                n_nucleations++;
            }
            energy = new_energy;
        }
        if ( n_nucleations > 20 ) break;
    }
    std::cout << "Number of steps: " << steps << std::endl;
    lattice.dump();
    return 0;
}

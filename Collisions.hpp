#ifndef _VEX3D_COLLISIONS_
#define _VEX3D_COLLISIONS_
#include "Entity.hpp" // inevitably this should be left out but leaving in for my linter

namespace Vex {
	namespace Collisions {
		auto AABB(Vex::Entity& e1, Vex::Entity& e2) -> bool;
	}
}

auto AABB(Vex::Entity& e1, Vex::Entity& e2) -> bool {
	std::vector<float> a1 = e1.GetAABB();
	std::vector<float> a2 = e2.GetAABB();

	// a = {min_x, min_y, min_z, max_x, max_y, max_z}
	if (a1[3] < a2[0] || a1[0] > a2[3]) return false;
	if (a1[4] < a2[1] || a1[1] > a2[4]) return false;
	if (a1[5] < a2[2] || a1[2] > a2[5]) return false;
	return true;
}

#endif

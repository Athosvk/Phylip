#pragma once

#include "cloth_mesh.hpp"
#include "sphere_primitive.hpp"

#include <memory>

namespace phyl {
	class Simulation {
		public:
			Simulation() : Simulation(nullptr, nullptr) {};
			Simulation(std::shared_ptr<ClothMesh> cm, std::vector<SpherePrimitive>* p) : mesh(cm), prims(p), gravityConst(9.8) {};
			~Simulation() {};

			void setClothMesh(std::shared_ptr<ClothMesh>);
			void setPrimitiveList(std::vector<SpherePrimitive>*);

			void update(double dt);

		private:
			std::shared_ptr<ClothMesh> mesh;
			std::vector<SpherePrimitive>* prims;

			const double gravityConst;
			Eigen::VectorXd extF;
	};
}

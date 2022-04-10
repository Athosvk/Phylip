#include "simulation.hpp"
#include <iostream>

namespace phyl{
	void Simulation::setClothMesh(std::shared_ptr<ClothMesh> cm){
		mesh = cm;
	}

	void Simulation::setPrimitiveList(std::vector<SpherePrimitive>* p){
		prims = p;
	}

	void Simulation::update(double dt){
		if(mesh == nullptr) return;
		/* External Forces */
		extF.resize(mesh->getVertexCount() * 3); /* xyzxyzxyz... */
		extF.setZero();
		// gravity
		for (int i = 0; i < mesh->getVertexCount(); ++i) {
			extF[3*i+1] += -gravityConst;
		}
		extF = mesh->getMassMatrix() * extF;

		/* Integrate velocity and positions of the cloth */
		Eigen::VectorXd penetration = Eigen::VectorXd(mesh->getVertexCount()*3);
		/* Collision resolution */
		if(prims == nullptr) return;
		for(const auto &p : *prims) {
			penetration.setZero();
			for(int i = 0; i < mesh->getVertexCount(); ++i){
				Eigen::Vector3d v = mesh->getVertex(i);
				Eigen::Vector3d normal;
				double distance;
				if(p.intersection(v, normal, distance)){
					penetration.block<3,1>(3*i, 0) += (distance) * normal;
					std::cout << penetration.block<3,1>(3*i, 0) << std::endl;
				}
			}
			mesh->setPosition(mesh->getPosition() - penetration);
		}
		/* */
	}
};

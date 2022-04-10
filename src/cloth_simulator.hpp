#include <Eigen/SparseCore>
#include <Eigen/SparseCholesky>
#include <vector>
#include "spring_constraint.hpp"

namespace phyl {

class ClothMesh;

class ClothSimulator
{
public:
	ClothSimulator(ClothMesh& mesh);

	void update(const float dt);
private:
	std::vector<SpringConstraint> createSpringConstraints() const;

	ClothMesh& m_mesh;
	// A default value used in the paper's solution
	double m_stiffnessCoefficient = 80.;
	Eigen::SimplicialLLT<Eigen::SparseMatrix<double>, Eigen::Upper> m_cholesky_solver;
	Eigen::SparseMatrix<double> m_mass;
	Eigen::VectorXd m_inertiaY;
	Eigen::VectorXd m_externalForces;
	std::vector<SpringConstraint> m_springConstraints;
	//Eigen::SparseMatrix<double> 
};
}
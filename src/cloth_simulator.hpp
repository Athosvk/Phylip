#include <Eigen/SparseCore>
#include <Eigen/SparseCholesky>
#include "spring_constraint.hpp"
#include "sphere_primitive.hpp"

#include <vector>
#include <memory>

namespace phyl {

class ClothMesh;

class ClothSimulator
{
public:
	ClothSimulator(std::shared_ptr<ClothMesh> mesh);

	void update(const std::vector<SpherePrimitive> &prims, const float dt);
private:
	void integratePositions(float dt);
	Eigen::VectorXd calculateGradient(const Eigen::VectorXd& currentEvaluationPositions, float dt) const;
	std::vector<SpringConstraint> createSpringConstraints() const;
	double searchLine(const Eigen::VectorXd& currentEvaluationPositions, const Eigen::VectorXd& gradientDirection, const Eigen::VectorXd& descentDirection);

	std::shared_ptr<ClothMesh> m_mesh;
	// A default value used in the paper's solution
	double m_stiffnessCoefficient = 80.;
	Eigen::VectorXd m_gravityC;
	Eigen::SparseMatrix<double> m_mass;
	Eigen::VectorXd m_inertiaY;
	Eigen::VectorXd m_externalForces;
	std::vector<SpringConstraint> m_springConstraints;
	//Eigen::SparseMatrix<double> 
};
}

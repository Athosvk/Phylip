#include <Eigen/SparseCore>
#include <Eigen/SparseCholesky>
#include "spring_constraint.hpp"
#include "attachment_constraint.hpp"
#include "sphere_primitive.hpp"
#include "options.hpp"

#include <vector>
#include <memory>

namespace phyl {

class ClothMesh;

class ClothSimulator
{
public:
	ClothSimulator(Options* opts);

	void update(const std::vector<SpherePrimitive> &prims, const float dt);
	void setCloth(std::shared_ptr<ClothMesh> mesh);
private:
	void integratePositions(float dt);
	Eigen::VectorXd calculateGradient(const Eigen::VectorXd& currentEvaluationPositions, float dt) const;
	std::vector<SpringConstraint> createSpringConstraints() const;
	std::vector<AttachmentConstraint> createAttachmentConstraints() const;
	double searchLine(float dt, const Eigen::VectorXd& currentEvaluationPositions, const Eigen::VectorXd& gradientDirection, const Eigen::VectorXd& descentDirection);
	double evaluateObjectiveFunction(float dt, const Eigen::VectorXd& currentEvaluationPositions) const;

	std::shared_ptr<ClothMesh> m_mesh;
	// A default value used in the paper's solution TODO: read from Options object
	double m_elasticStiffnessCoefficient 		= 80.;
	double m_bendingStiffnessCoefficient 		= 20.;
	double m_attachmentStiffnessCoefficient 	= 120.;
	double m_dampeningCoefficient 				= 0.001;
	double m_gravityCoeff						= 9.8;

	Eigen::VectorXd m_gravity;
	Eigen::SparseMatrix<double> m_mass;
	Eigen::VectorXd m_inertiaY;
	Eigen::VectorXd m_externalForces;
	std::vector<SpringConstraint> m_springConstraints;
	std::vector<AttachmentConstraint> m_attachConstraints;
	Eigen::VectorXd m_velocities;
};
}

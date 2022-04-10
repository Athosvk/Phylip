#include <Eigen/Core>

namespace phyl {
	struct SpringConstraint
	{
		unsigned int StartVertex;
		unsigned int EndVertex;
		double RestLength;

		Eigen::Vector3d EvaluateGradient(const Eigen::VectorXd& evaluationPositions, double stiffness) const;
	};
}
#include "spring_constraint.hpp"

namespace phyl
{
	Eigen::Vector3d SpringConstraint::EvaluateGradient(const Eigen::VectorXd& evaluationPositions, double stiffness) const
	{
		// This isn't the wrong way around, apparently we use the gradient obtained in this order of subtraction
		Eigen::Vector3d x_ij = evaluationPositions.segment<3>(StartVertex) - evaluationPositions.segment<3>(EndVertex);

		// k * extension * direction
		return stiffness * (x_ij.norm() - RestLength) * x_ij.normalized();
	}

	double SpringConstraint::EvaluatePotentialEnergy(const Eigen::VectorXd& evaluationPositions) const
	{
		return 0.0;
	}
}

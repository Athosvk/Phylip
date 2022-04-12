#include "attachment_constraint.hpp"

namespace phyl
{
	Eigen::Vector3d AttachmentConstraint::EvaluateGradient(const Eigen::VectorXd& evaluationPositions) const
	{
		// This isn't the wrong way around, apparently we use the gradient obtained in this order of subtraction
		Eigen::Vector3d x_ij = evaluationPositions.segment<3>(Vertex * 3) - Pos;

		// k * extension * direction
		return Stiffness * (x_ij);
	}

	double AttachmentConstraint::EvaluatePotentialEnergy(const Eigen::VectorXd& evaluationPositions) const
	{
		Eigen::Vector3d difference = Pos - evaluationPositions.segment<3>(Vertex * 3);
		return 0.5 * Stiffness * difference.squaredNorm();
	}
}
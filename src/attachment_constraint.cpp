#include "attachment_constraint.hpp"

namespace phyl
{
	Eigen::Vector3d AttachmentConstraint::EvaluateGradient(const Eigen::VectorXd& evaluationPositions, double stiffness) const
	{
		if (!Enabled)
		{
			return Eigen::Vector3d::Zero();
		}
		// This isn't the wrong way around, apparently we use the gradient obtained in this order of subtraction
		Eigen::Vector3d x_ij = evaluationPositions.segment<3>(Vertex * 3) - Pos;

		// k * extension * direction
		return stiffness * (x_ij);
	}

	double AttachmentConstraint::EvaluatePotentialEnergy(const Eigen::VectorXd& evaluationPositions, double stiffness) const
	{
		if (!Enabled)
		{
			return 0.;
		}
		Eigen::Vector3d difference = Pos - evaluationPositions.segment<3>(Vertex * 3);
		return 0.5 * stiffness * difference.squaredNorm();
	}
}

#pragma once
#include <Eigen/Core>

namespace phyl {
	struct AttachmentConstraint
	{
		double Stiffness;
		Eigen::Vector3d Pos;
		unsigned int Vertex;

		Eigen::Vector3d EvaluateGradient(const Eigen::VectorXd& evaluationPositions) const;
		double EvaluatePotentialEnergy(const Eigen::VectorXd& evaluationPositions) const;
	};
}

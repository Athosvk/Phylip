#pragma once
#include <Eigen/Core>

namespace phyl {
	struct AttachmentConstraint
	{
		Eigen::Vector3d Pos;
		unsigned int Vertex;
		bool Enabled = true;

		Eigen::Vector3d EvaluateGradient(const Eigen::VectorXd& evaluationPositions, double stiffness) const;
		double EvaluatePotentialEnergy(const Eigen::VectorXd& evaluationPositions, double stiffness) const;
	};
}

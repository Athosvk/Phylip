#pragma once
#include <Eigen/Core>

namespace phyl {
	struct SpringConstraint
	{
		double stiffness;
		unsigned int StartVertex;
		unsigned int EndVertex;
		double RestLength;

		Eigen::Vector3d EvaluateGradient(const Eigen::VectorXd& evaluationPositions) const;
		double EvaluatePotentialEnergy(const Eigen::VectorXd& evaluationPositions) const;
	};
}

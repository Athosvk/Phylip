#pragma once

#include "Eigen/Dense"

namespace phyl{
	class Constraint {
		public:
			Constraint(double stiff, double restL) : Stiffness{stiff}, RestLength{restL} {}

			double Stiffness;
			double RestLength;

		protected:
			virtual Eigen::Vector3d valuateGradient(const Eigen::VectorXd& evaluationPositions) const = 0;
			virtual double EvaluatePotentialEnergy(const Eigen::VectorXd& evaluationPositions) const = 0;
	};
}

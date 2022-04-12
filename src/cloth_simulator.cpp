#include "cloth_simulator.hpp"
#include "cloth_mesh.hpp"

#include <cstdint>

namespace phyl {
	ClothSimulator::ClothSimulator(std::shared_ptr<ClothMesh> mesh) :
		m_mesh(mesh),
		m_springConstraints(createSpringConstraints()) {
		size_t componentCount = mesh->GetVertexCount() * 3;
		m_inertiaY.resize(componentCount);
		m_externalForces.resize(componentCount);
		Eigen::Vector3d gravity = Eigen::Vector3d(0,-9.81,0);
		m_gravityC = gravity.replicate(m_mesh->GetVertexCount(), 1);
	}
	
	void ClothSimulator::integratePositions(float dt)
	{
		Eigen::VectorXd currentBest = m_inertiaY;
		bool converged = false;
		uint32_t currentIteration = 0;
		const uint32_t MaxIterations = 10;

		while (!converged && currentIteration++ < MaxIterations)
		{
			Eigen::VectorXd gradient = calculateGradient(currentBest, dt);
			// This is the epsilon used by the authors of the paper.
			// If this is smaller, it means we've already established
			// the positions that satisfy the constraints
			if (gradient.squaredNorm() < 1e-15)
			{
				converged = true;
			}
			else
			{
				Eigen::VectorXd descentDirection = -gradient;
				double descentLength;
				currentBest += descentDirection * descentLength;

				// We hardly had to move at all to reach convergence,
				// which means we have satisfied the constraint
				if (descentLength <= 1e-15)
				{
					converged = true;
				}
			}	

		}
	}

	Eigen::VectorXd ClothSimulator::calculateGradient(const Eigen::VectorXd& currentEvaluationPositions, float dt) const
	{
		Eigen::VectorXd gradient = Eigen::VectorXd::Zero(m_mesh->GetVertexCount() * 3);
		for (const auto& constraint : m_springConstraints)
		{
			Eigen::VectorXd constraintGradient = constraint.EvaluateGradient(currentEvaluationPositions, m_stiffnessCoefficient);
			// evaluate gradient
			gradient.segment<3>(constraint.StartVertex) += constraintGradient;
			gradient.segment<3>(constraint.EndVertex) -= constraintGradient;
		}
		gradient -= m_externalForces;

		return m_mesh->GetVertexMasses() * (currentEvaluationPositions - m_inertiaY) + dt * dt * gradient;
	}

	std::vector<SpringConstraint> ClothSimulator::createSpringConstraints() const
	{
		std::vector<SpringConstraint> constraints;
		const Eigen::VectorXd positions = m_mesh->GetVertexPositions();
		for (const auto& edge : m_mesh->GetEdges())
		{
			Eigen::Vector3d edgeStart = positions.segment<3>(edge.VertexStart);
			Eigen::Vector3d edgeEnd = positions.segment<3>(edge.VertexEnd);
			constraints.emplace_back(SpringConstraint{ edge.VertexStart, edge.VertexEnd, (edgeEnd - edgeStart).norm() });
		}

		// Laziness
		auto getIndex = [this](uint32_t column, uint32_t row) { return column * m_mesh->GetSize() + row; };

		// Creation of bending constraints, which preserves the dihedral angle between two attached triangles
		// This creates the triangle pairs with vertices (i, j), (i, j + 1), (i + 1, j + 1), (i, j + 2)
		// and (i, j), (i + 1, j), (i + 1, j + 1), (i + 2, j). See also slides on mass-spring systems
		// TODO: Not sure about the order of the vertices, we might have to switch these loops around? Might not matter
		// since it's always square for us
		for (uint32_t i = 0; i < m_mesh->GetSize(); i++)
		{
			for (uint32_t j = 0; j < m_mesh->GetSize(); j++)
			{
				uint32_t triIndexV0 = getIndex(i, j);
				Eigen::Vector3d triV0 = positions.segment<3>(triIndexV0);

				if (i + 2 < m_mesh->GetSize())
				{
					// The name is slightly misleading since these are not to the "triangles itself", but 
					// it's the most descriptive I could come up with
					uint32_t triIndexV1 = getIndex(i + 2, j);
					Eigen::Vector3d triV1 = positions.segment<3>(triIndexV1);
					constraints.emplace_back(SpringConstraint{ triIndexV0, triIndexV1, (triV1 - triV0).norm() });
				}

				if (j + 2 < m_mesh->GetSize())
				{
					// The name is slightly misleading since these are not to the "triangles itself", but 
					// it's the most descriptive I could come up with
					uint32_t triIndexV1 = getIndex(i, j + 2);
					Eigen::Vector3d triV1 = positions.segment<3>(triIndexV1);
					constraints.emplace_back(SpringConstraint{ triIndexV0, triIndexV1, (triV1 - triV0).norm() });
				}
			}
		}

		return constraints;
	}

	double ClothSimulator::searchLine(const Eigen::VectorXd& currentEvaluationPositions, const Eigen::VectorXd& gradientDirection, const Eigen::VectorXd& descentDirection)
	{
		Eigen::VectorXd currentLinePosition;
		// This is just used by the factor to control the speed of convergence for the step size itself, 
		//	0.1 being the default
		const double StepsizeBetaFactor = 0.1;
		double currentStepSize = 1.0 / StepsizeBetaFactor;

		// Initial values simply to trigger the while loop
		double lhs = 1.0;
		double rhs = 0.0;
		double initialObjectiveValue = 0.0;/* evaluateOb */

		// If our step size is too small, we basically already found our solution. If it's
		// non-zero, it means we have some way to go, but this might point to a local minimum.
		// Therefore we might need multiple iterations along the way
		while (currentStepSize > 1e-15 && lhs >= rhs)
		{
			currentStepSize *= StepsizeBetaFactor;
			lhs = 0.0 /* evaluateObjectiveFunction */;

			// Not exactly sure how this factor works, but I believe this is a factor to control 
			// the accuracy, with higher alpha being a higher tolerance for errors.
			const double StepsizeAlphaFactor = 0.25;
			rhs = initialObjectiveValue + StepsizeAlphaFactor * currentStepSize *
				// Implemented in the original code as g^t * -g, but this should be the same
				gradientDirection.dot(descentDirection);
		}
		return currentStepSize < 1e-15 ? 0.0 : currentStepSize;
	}

	void ClothSimulator::update(const std::vector<SpherePrimitive> &prims, const float dt) {
		/* External force */
		m_externalForces.resize(m_mesh->GetVertexCount() * 3);
		m_externalForces.setZero();

		m_externalForces = m_mesh->GetVertexMasses() * m_gravityC + m_externalForces;

		// I have no idea why they call this inertia y, it's just used as a "what if there are no constraints"
		// integrated position
		m_inertiaY = m_mesh->GetVertexPositions() + m_mesh->GetVertexVelocities() * dt;
		
		/* Collisions resolution */
		Eigen::VectorXd penetration = Eigen::VectorXd(m_mesh->GetVertexCount()*3);
		/* Collision resolution */
		for(const auto &p : prims) {
			penetration.setZero();
			for(int i = 0; i < m_mesh->GetVertexCount(); ++i){
				Eigen::Vector3d v = m_mesh->GetVertex(i);
				Eigen::Vector3d normal;
				double distance;
				if(p.intersection(v, normal, distance)){
					penetration.block<3,1>(3*i, 0) += (distance) * normal;
				}
			}
			m_mesh->SetVertexPositions(m_mesh->GetVertexPositions() - penetration);
		}

		m_mesh->SetVertexVelocities(m_mesh->GetVertexVelocities() + m_externalForces * dt);
		m_mesh->SetVertexPositions(m_mesh->GetVertexPositions() + m_mesh->GetVertexVelocities() * dt);
		m_mesh->updateMesh();
	}
}

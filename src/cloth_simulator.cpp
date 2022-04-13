#include "cloth_simulator.hpp"
#include "cloth_mesh.hpp"

#include <cstdint>

namespace phyl {
	ClothSimulator::ClothSimulator(Options *opts) {
		m_elasticStiffnessCoefficient = opts->getDouble("elastic_stiffness", 80.0);
		m_bendingStiffnessCoefficient = opts->getDouble("bending_stiffness", 20.0);
		m_attachmentStiffnessCoefficient = opts->getDouble("attachment_stiffness", 120.0);
		m_dampeningCoefficient = opts->getDouble("dampening_coeff", 0.001);
		m_gravityCoeff = opts->getDouble("gravity_coeff", 9.8);
		double wx = opts->getDouble("wind_direction_x", 0.0);
		double wy = opts->getDouble("wind_direction_y", 0.0);
		double wz = opts->getDouble("wind_direction_z", 0.0);
		m_windIntensity = opts->getDouble("wind_intensity", 0.0);
		m_windDirection = Eigen::Vector3d(wx, wy, wz);
	}

	void ClothSimulator::setCloth(std::shared_ptr<ClothMesh> mesh){
		m_mesh = mesh;
		m_springConstraints = createSpringConstraints();
		m_attachConstraints = createAttachmentConstraints();
		size_t componentCount = mesh->GetVertexCount() * 3;
		m_inertiaY.resize(componentCount);
		m_externalForces = Eigen::VectorXd::Zero(componentCount);
		m_velocities = Eigen::VectorXd::Zero(componentCount);
		Eigen::Vector3d gravity = Eigen::Vector3d(0,-m_gravityCoeff,0);
		m_gravity = gravity.replicate(m_mesh->GetVertexCount(), 1);
	}
	
	void ClothSimulator::integratePositions(float dt)
	{
		Eigen::VectorXd currentBest = m_inertiaY;
		bool converged = false;
		uint32_t currentIteration = 0;
		const uint32_t MaxIterations = 200;

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
				double descentLength = searchLine(dt, currentBest, gradient, descentDirection);
				currentBest += descentDirection * descentLength;

				// We hardly had to move at all to reach convergence,
				// which means we have satisfied the constraint
				if (descentLength <= 1e-15)
				{
					converged = true;
				}
			}
		}

		m_velocities = (currentBest - m_mesh->GetVertexPositions()) / dt;
		m_mesh->SetVertexPositions(currentBest);
	}

	Eigen::VectorXd ClothSimulator::calculateGradient(const Eigen::VectorXd& currentEvaluationPositions, float dt) const
	{
		Eigen::VectorXd gradient = Eigen::VectorXd::Zero(m_mesh->GetVertexCount() * 3);
		for (const auto& constraint : m_springConstraints)
		{
			Eigen::VectorXd constraintGradient = constraint.EvaluateGradient(currentEvaluationPositions);
			// evaluate gradient
			gradient.segment<3>(constraint.StartVertex * 3) += constraintGradient;
			gradient.segment<3>(constraint.EndVertex * 3) -= constraintGradient;
		}
		for (const auto& constraint : m_attachConstraints)
		{
			Eigen::VectorXd constraintGradient = constraint.EvaluateGradient(currentEvaluationPositions);
			// evaluate gradient
			gradient.segment<3>(constraint.Vertex * 3) += constraintGradient;
		}

		gradient -= m_externalForces;
		return m_mesh->GetVertexMasses() * (currentEvaluationPositions - m_inertiaY) + dt * dt * gradient;
	}

	std::vector<AttachmentConstraint> ClothSimulator::createAttachmentConstraints() const
	{
		if(!m_mesh->hasFixedVertices()) return {};
		std::vector<AttachmentConstraint> constraints;
		constraints.push_back(AttachmentConstraint{m_attachmentStiffnessCoefficient,
													m_mesh->GetVertexPosition(0), 
													0});
		unsigned int lastIdx = m_mesh->GetSize()-1;
		constraints.push_back(AttachmentConstraint{m_attachmentStiffnessCoefficient,
													m_mesh->GetVertexPosition(lastIdx),
													lastIdx
													});
		return constraints;
	}

	std::vector<SpringConstraint> ClothSimulator::createSpringConstraints() const
	{
		std::vector<SpringConstraint> constraints;
		const Eigen::VectorXd positions = m_mesh->GetVertexPositions();
		for (const auto& edge : m_mesh->GetEdges())
		{
			Eigen::Vector3d edgeStart = positions.segment<3>(edge.VertexStart * 3);
			Eigen::Vector3d edgeEnd = positions.segment<3>(edge.VertexEnd * 3);
			constraints.emplace_back(SpringConstraint{ m_elasticStiffnessCoefficient, edge.VertexStart, edge.VertexEnd, (edgeEnd - edgeStart).norm() });
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
				Eigen::Vector3d triV0 = positions.segment<3>(triIndexV0 * 3);

				if (i + 2 < m_mesh->GetSize())
				{
					// The name is slightly misleading since these are not to the "triangles itself", but 
					// it's the most descriptive I could come up with
					uint32_t triIndexV1 = getIndex(i + 2, j);
					Eigen::Vector3d triV1 = positions.segment<3>(triIndexV1 * 3);
					constraints.emplace_back(SpringConstraint{m_bendingStiffnessCoefficient, triIndexV0, triIndexV1, (triV1 - triV0).norm() });
				}

				if (j + 2 < m_mesh->GetSize())
				{
					// The name is slightly misleading since these are not to the "triangles itself", but 
					// it's the most descriptive I could come up with
					uint32_t triIndexV1 = getIndex(i, j + 2);
					Eigen::Vector3d triV1 = positions.segment<3>(triIndexV1 * 3);
					constraints.emplace_back(SpringConstraint{m_bendingStiffnessCoefficient, triIndexV0, triIndexV1, (triV1 - triV0).norm() });
				}
			}
		}
		return constraints;
	}

	double ClothSimulator::searchLine(float dt, const Eigen::VectorXd& currentEvaluationPositions, const Eigen::VectorXd& gradientDirection, const Eigen::VectorXd& descentDirection)
	{
		Eigen::VectorXd currentLinePosition;
		// This is just used by the factor to control the speed of convergence for the step size itself, 
		//	0.1 being the default
		const double StepsizeBetaFactor = 0.9;
		double currentStepSize = 1.0 / StepsizeBetaFactor;

		// Initial values simply to trigger the while loop
		double lhs = 1.0;
		double rhs = 0.0;
		double initialObjectiveValue = evaluateObjectiveFunction(dt, currentEvaluationPositions);

		// If our step size is too small, we basically already found our solution. If it's
		// non-zero, it means we have some way to go, but this might point to a local minimum.
		// Therefore we might need multiple iterations along the way
		while (currentStepSize > 1e-15 && lhs >= rhs)
		{
			currentStepSize *= StepsizeBetaFactor;
			lhs = evaluateObjectiveFunction(dt, currentEvaluationPositions + currentStepSize * descentDirection);

			// Not exactly sure how this factor works, but I believe this is a factor to control 
			// the accuracy, with higher alpha being a higher tolerance for errors.
			const double StepsizeAlphaFactor = 0.5;
			rhs = initialObjectiveValue + StepsizeAlphaFactor * currentStepSize *
				// Implemented in the original code as g^t * -g, but this should be the same
				gradientDirection.dot(descentDirection);
		}
		return currentStepSize < 1e-15 ? 0.0 : currentStepSize;
	}

	double ClothSimulator::evaluateObjectiveFunction(float dt, const Eigen::VectorXd& currentEvaluationPositions) const
	{
		double potential = 0.0;
		for (const SpringConstraint& constraint : m_springConstraints)
		{
			potential += constraint.EvaluatePotentialEnergy(currentEvaluationPositions);
		}
		for (const AttachmentConstraint& constraint : m_attachConstraints)
		{
			potential += constraint.EvaluatePotentialEnergy(currentEvaluationPositions);
		}

		potential -= currentEvaluationPositions.dot(m_externalForces);

		Eigen::VectorXd y = currentEvaluationPositions - m_inertiaY;
		double inertiaTerm = 0.5 * y.transpose() * m_mesh->GetVertexMasses() * y;
		return inertiaTerm + potential * dt * dt;
	}

	void ClothSimulator::update(const std::vector<SpherePrimitive> &prims, const float dt) {
		// I have no idea why they call this inertia y, it's just used as a "what if there are no constraints"
		// integrated position
		m_inertiaY = m_mesh->GetVertexPositions() + m_velocities * dt;

		//wind_force = c[norm \dot (dir - vel)]*norm
		Eigen::VectorXd windForce = m_windIntensity * (m_mesh->GetVertexNormals().dot(m_windDirection-m_velocities))*m_mesh->GetVertexNormals();
		m_externalForces = m_mesh->GetVertexMasses() * m_gravity + windForce;
		integratePositions(dt);
		
		/* Collisions resolution */
		Eigen::VectorXd penetration = Eigen::VectorXd(m_mesh->GetVertexCount()*3);
		for(const auto &p : prims) {
			penetration.setZero();
			for(int i = 0; i < m_mesh->GetVertexCount(); ++i){
				Eigen::Vector3d v = m_mesh->GetVertexPosition(i);
				Eigen::Vector3d normal;
				double distance;
				if(p.intersection(v, normal, distance)){
					double VisualAdjustmentMultiplier = 1.09;
					penetration.block<3,1>(3*i, 0) += (distance * VisualAdjustmentMultiplier) * normal;
				}
			}
			m_mesh->SetVertexPositions(m_mesh->GetVertexPositions() - penetration);
		}
	
		/* Damp Velocities: http://physbam.stanford.edu/~fedkiw/papers/stanford2012-03.pdf */
		Eigen::Vector3d posCM = Eigen::Vector3d::Zero();
		Eigen::Vector3d velCM = Eigen::Vector3d::Zero();
		auto vertexMasses = m_mesh->GetVertexMasses();
		double den = 0.0;
		for(int i = 0; i < m_mesh->GetVertexCount(); ++i) {
			double mass = vertexMasses.coeff(i*3, i*3);

			posCM += mass * m_mesh->GetVertexPosition(i);
			velCM += mass * m_velocities.block<3,1>(i*3, 0);
			den += mass;
		}

		posCM = posCM / den; /* Center of mass */
		velCM = velCM / den; /* Velocity of the CoM */

		Eigen::Vector3d L = Eigen::Vector3d::Zero(); /* Angular momentum */
		Eigen::Matrix3d I = Eigen::Matrix3d::Zero(); /* Inertia Tensor */
		for(int i = 0; i < m_mesh->GetVertexCount(); ++i) {
			Eigen::Vector3d arm = m_mesh->GetVertexPosition(i) - posCM;
			double mass = vertexMasses.coeff(i*3, i*3);
			L = L + arm.cross(mass * m_velocities.block<3,1>(i*3, 0));
			//rTilde is the 3 by 3 matrix with the property rTilde * v = ri × v
			Eigen::Matrix3d rTilde = Eigen::Matrix3d::Zero();
			rTilde.coeffRef(0,1) = arm.z();
			rTilde.coeffRef(0,2) = -arm.y();
			rTilde.coeffRef(1,0) = -arm.z();
			rTilde.coeffRef(1,2) = arm.x();
			rTilde.coeffRef(2,0) = arm.y();
			rTilde.coeffRef(2,1) = -arm.x();
			I = I + (rTilde * rTilde.transpose() * mass);
		}
		Eigen::Vector3d angularVel = I.inverse() * L;

		for(int i = 0; i < m_mesh->GetVertexCount(); ++i) {
			Eigen::Vector3d qi = m_mesh->GetVertexPosition(i);
			Eigen::Vector3d arm = qi - posCM;
			Eigen::Vector3d deltaV = velCM + angularVel.cross(arm) - m_velocities.block<3,1>(i*3, 0);
			m_velocities.block<3,1>(i*3, 0) = m_velocities.block<3,1>(i*3, 0) + m_dampeningCoefficient * deltaV;
		}
	}
}

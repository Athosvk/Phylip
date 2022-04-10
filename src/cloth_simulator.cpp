#include "cloth_simulator.hpp"
#include "cloth_mesh.hpp"

#include <cstdint>

namespace phyl {
	ClothSimulator::ClothSimulator(ClothMesh& mesh) :
		m_mesh(mesh),
		m_springConstraints(createSpringConstraints()) {
		size_t componentCount = mesh.GetVertexCount() * 3;
		m_inertiaY.resize(componentCount);
		m_externalForces.resize(componentCount);
	}
	
	std::vector<SpringConstraint> ClothSimulator::createSpringConstraints() const
	{
		std::vector<SpringConstraint> constraints;
		const Eigen::VectorXd positions = m_mesh.GetVertexPositions();
		for (const auto& edge : m_mesh.GetEdges())
		{
			Eigen::Vector3d edgeStart = positions.segment<3>(edge.VertexStart);
			Eigen::Vector3d edgeEnd = positions.segment<3>(edge.VertexEnd);
			constraints.emplace_back(SpringConstraint{ edge.VertexStart, edge.VertexEnd, m_stiffnessCoefficient, (edgeEnd - edgeStart).norm() });
		}

		// Laziness
		auto getIndex = [this](uint32_t column, uint32_t row) { return column * m_mesh.GetSize() + row; };

		// Creation of bending constraints, which preserves the dihedral angle between two attached triangles
		// This creates the triangle pairs with vertices (i, j), (i, j + 1), (i + 1, j + 1), (i, j + 2)
		// and (i, j), (i + 1, j), (i + 1, j + 1), (i + 2, j). See also slides on mass-spring systems
		// TODO: Not sure about the order of the vertices, we might have to switch these loops around? Might not matter
		// since it's always square for us
		for (uint32_t i = 0; i < m_mesh.GetSize(); i++)
		{
			for (uint32_t j = 0; j < m_mesh.GetSize(); j++)
			{
				uint32_t triIndexV0 = getIndex(i, j);
				Eigen::Vector3d triV0 = positions.segment<3>(triIndexV0);

				if (i + 2 < m_mesh.GetSize())
				{
					// The name is slightly misleading since these are not to the "triangles itself", but 
					// it's the most descriptive I could come up with
					uint32_t triIndexV1 = getIndex(i + 2, j);
					Eigen::Vector3d triV1 = positions.segment<3>(triIndexV1);
					constraints.emplace_back(SpringConstraint{ triIndexV0, triIndexV1, m_stiffnessCoefficient, (triV1 - triV0).norm() });
				}

				if (j + 2 < m_mesh.GetSize())
				{
					// The name is slightly misleading since these are not to the "triangles itself", but 
					// it's the most descriptive I could come up with
					uint32_t triIndexV1 = getIndex(i, j + 2);
					Eigen::Vector3d triV1 = positions.segment<3>(triIndexV1);
					constraints.emplace_back(SpringConstraint{ triIndexV0, triIndexV1, m_stiffnessCoefficient, (triV1 - triV0).norm() });
				}
			}
		}

		return constraints;
	}

	void ClothSimulator::update(const float dt) {
		m_inertiaY = m_mesh.GetVertexPositions() + m_mesh.GetVertexVelocities() * dt;

		Eigen::Vector3d gravity;
		gravity << 0, -9.81, 0;
		Eigen::VectorXd gravityC = gravity.replicate(m_mesh.GetVertexCount(), 1);
		m_externalForces += m_mesh.GetVertexMasses() * gravityC;
	}
}

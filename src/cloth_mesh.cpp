#include "cloth_mesh.hpp"
#include <iostream>

#define SQRT_2 1.4142135623730951

namespace phyl{
	ClothMesh::ClothMesh(int w, int h, double mass, int lod, bool hasFixed) : width(w), height(h), lod(lod), mass(mass), hasFixed(hasFixed) {
		genMesh();
		genEdges();
		genConstraints();
		mat = LoadMaterialDefault();
		mat.maps[0].color.r = 225;
		mat.maps[0].color.g = 125;
		mat.maps[0].color.b = 50;

		Eigen::Vector3d gravTmp; gravTmp << 0, -9.81, 0;
		gravF = massMatrix * gravTmp.replicate(currPositions.size() / 3, 1);

		
		// Just some test code, remove later
		//for (int i = 0; i < currVelocities.rows() / 3; i++)
		//{
			//currVelocities[i * 3 + 1] = i * i;
		//}
	}

	void ClothMesh::draw(){
		DrawMesh(mesh, mat, transform.getTransformationMatrix());
	}

	void ClothMesh::update(const float dt){
		updateMesh();
		// Will probably move to some simulation class to decouple from rendering
		integrateVelocity(dt);
		integratePosition(dt);
	}

	ClothMesh::~ClothMesh(){
		UnloadMesh(mesh);
		UnloadMaterial(mat);
	}
/*
 * Lengyel, Eric. “Building an Edge List for an Arbitrary Mesh”. Terathon Software 3D Graphics Library, 2005. http://www.terathon.com/code/edges.html 
 * https://web.archive.org/web/20160317181124/http://www.terathon.com/code/edges.html
 */
	int ClothMesh::genEdges() {
		long maxEdgeCount = mesh.triangleCount * 3;
		int vertexCount = mesh.vertexCount;

		unsigned short *firstEdge = new unsigned short[vertexCount + maxEdgeCount];
		unsigned short *nextEdge = firstEdge + vertexCount;

		for (long a = 0; a < vertexCount; a++) 
			firstEdge[a] = 0xFFFF;
		// First pass over all triangles. This finds all the edges satisfying the
		// condition that the first vertex index is less than the second vertex index
		// when the direction from the first vertex to the second vertex represents
		// a counterclockwise winding around the triangle to which the edge belongs.
		// For each edge found, the edge index is stored in a linked list of edges
		// belonging to the lower-numbered vertex index i. This allows us to quickly
		// find an edge in the second pass whose higher-numbered vertex index is i.

		unsigned int edgeCount = 0;
		const unsigned short* triangle = &mesh.indices[0];
		unsigned int i1, i2;
		for (long a = 0; a < mesh.triangleCount; a++) {
			long i1 = triangle[2];
			for (long b = 0; b < 3; b++) {
				long i2 = triangle[b];
				if (i1 < i2) {
					Edge edge;
					edge.verts[0] = (unsigned short) i1;
					edge.verts[1] = (unsigned short) i2;
					edge.tris[0] = (unsigned short) a;
					edge.tris[1] = (unsigned short) a;
					edges.push_back(edge);

					unsigned short edgeIndex = firstEdge[i1];
					if (edgeIndex == 0xFFFF) {
						firstEdge[i1] = edgeCount;
					}
					else {
						for (;;) {
							long index = nextEdge[edgeIndex];
							if (index == 0xFFFF) {
								nextEdge[edgeIndex] = edgeCount;
								break;
							}
							edgeIndex = index;
						}
					}
					nextEdge[edgeCount] = 0xFFFF;
					edgeCount++;
				}
				i1 = i2;
			}
			triangle += 3;
		}

		// Second pass over all triangles. This finds all the edges satisfying the
		// condition that the first vertex index is greater than the second vertex index
		// when the direction from the first vertex to the second vertex represents
		// a counterclockwise winding around the triangle to which the edge belongs.
		// For each of these edges, the same edge should have already been found in
		// the first pass for a different triangle. So we search the list of edges
		// for the higher-numbered vertex index for the matching edge and fill in the
		// second triangle index. The maximum number of comparisons in this search for
		// any vertex is the number of edges having that vertex as an endpoint.

		triangle = &mesh.indices[0];
		for (long a = 0; a < mesh.triangleCount; a++) {
			long i1 = triangle[2];
			for (long b = 0; b < 3; b++) {
				long i2 = triangle[b];
				if (i1 > i2){
					bool isNewEdge = true; /* Non watertight mesh, we need to add edges that belong to only one triangle */
					for (long edgeIndex = firstEdge[i2]; edgeIndex != 0xFFFF; edgeIndex = nextEdge[edgeIndex]) {
						Edge *edge = &edges[edgeIndex];
						if ((edge->verts[1] == i1) && (edge->tris[0] == edge->tris[1])) {
							edge->tris[1] = (unsigned short) a;
							isNewEdge = false;
							break;
						}
					}
					if(isNewEdge) {
						Edge edge;
						edge.verts[0] = i1;
						edge.verts[1] = i2;
						edge.tris[0] = a;
						edge.tris[1] = a;
						edges.push_back(edge);

						unsigned int eIdx = firstEdge[i1];
						if(eIdx == 0xFFFF) {
							firstEdge[i1] = edgeCount;
						}
						else {
							while(true) {
								unsigned int idx = nextEdge[eIdx];
								if(idx == 0xFFFF) {
									nextEdge[eIdx] = edgeCount;
									break;
								}
								eIdx = idx;
							}
						}
						nextEdge[edgeCount] = 0xFFFF;
						edgeCount++;
					}
				}
				i1 = i2;
			}
			triangle+=3;
		}

		delete[] firstEdge;
		return (edgeCount);
	}

	void ClothMesh::transformPoints(MTransform &t) {
		Eigen::Matrix4d tEigen = t.getEigenTransformationMatrix();
		Eigen::Affine3d a(tEigen);
		for(int i = 0; i < mesh.vertexCount; ++i){
			Eigen::Vector3d tmp = a * currPositions.block<3,1>(3*i, 0);
			currPositions.block<3,1>(3*i, 0) = tmp;
		}
		updateMesh();
	}

	void ClothMesh::genConstraints(){
		if(hasFixed){
			// TODO: Constraint the first row first and last points to stay fixed in the starting position
			//constraints.push_back(Constraint(pos of first vertex in first row, first vertex in first row, 0));
			//constraints.push_back(Constraint(pos of last vertex in first row, last vertex in first row, 0));
		}
		for(const auto &e : edges){
			double dist = Vector3Distance(
					Vector3{mesh.vertices[e.verts[0] * 3 + 0], mesh.vertices[e.verts[0] * 3 + 1], mesh.vertices[e.verts[0] * 3 + 2]},
					Vector3{mesh.vertices[e.verts[1] * 3 + 0], mesh.vertices[e.verts[1] * 3 + 1], mesh.vertices[e.verts[1] * 3 + 2]});
			constraints.push_back(Constraint(e.verts[0], e.verts[1], dist));
		}
	}
	
	void ClothMesh::genMesh(){
		mesh = GenMeshPlane(width, height, lod, lod, true);

		int nVerts = mesh.vertexCount;

		double massPerVertex = mass/(double)nVerts;
		double invMassPerVertex = 1.0/massPerVertex;

		currPositions.resize(nVerts * 3, 1);
		currVelocities = Eigen::VectorXd::Zero(nVerts*3, 1);
		for(int i = 0; i < nVerts * 3; i++){
			currPositions[i] = mesh.vertices[i];
		}

		std::vector<Eigen::Triplet<double>> massTriplets;
		std::vector<Eigen::Triplet<double>> invMassTriplets;
		for(int i = 0; i < nVerts * 3; ++i){
			massTriplets.push_back(Eigen::Triplet<double>(i, i, massPerVertex));
			invMassTriplets.push_back(Eigen::Triplet<double>(i, i, invMassPerVertex));
		}
		massMatrix.resize(nVerts * 3, nVerts * 3);
		massMatrix.setFromTriplets(massTriplets.begin(), massTriplets.end());
		invMassMatrix.resize(nVerts * 3, nVerts * 3);
		invMassMatrix.setFromTriplets(invMassTriplets.begin(), invMassTriplets.end());
		gridSize = Vector3Distance(
				Vector3{mesh.vertices[0], mesh.vertices[1], mesh.vertices[2]},
				Vector3{mesh.vertices[3], mesh.vertices[4], mesh.vertices[5]}
				);
		std::cout << "Grid Size: " << gridSize << "\n";
	}

	void ClothMesh::integrateVelocity(const float dt)
	{
		currVelocities = currVelocities + gravF * dt;
	}

	void ClothMesh::integratePosition(const float dt){
		currPositions += dt * currVelocities;
	}

	void ClothMesh::updateMesh()
	{
		for(int i = 0; i < mesh.vertexCount * 3; i++){
			mesh.vertices[i] = currPositions[i];
		}
		// VBO index 0 are the vertex positions, see source code for other buffers
		UpdateMeshBuffer(mesh, 0, mesh.vertices, mesh.vertexCount * 3 * sizeof(float), 0);
	}

};

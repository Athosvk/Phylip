#pragma once

namespace phyl{
	class ClothMesh {
		public:
			ClothMesh(int width, int height);
			void draw();
			void update(const float dt);

		private:
			/*
			 * Generate vertices and triangles of the mesh
			 */
			void genMesh();

			int width, height;
	};
};

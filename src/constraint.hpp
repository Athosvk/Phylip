#pragma once

namespace phyl{
	class Constraint{
		public:
			/*
			 * Create a constraint between the vertex at idx1 and idx2 with a given rest length.
			 * @param idx1 index of the first vertex involved in the constraint;
			 * @param idx2 index of the second vertex involved in the constraint;
			 * @param rest rest length between the two vertices;
			 */
			Constraint(int _idx1, int _idx2, double _rest) : idx1{_idx1}, idx2{_idx2}, rest{_rest} {};

			const int idx1, idx2;
			const double rest;
	};
};

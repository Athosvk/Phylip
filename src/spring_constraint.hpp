namespace phyl {
	struct SpringConstraint
	{
		unsigned int StartVertex;
		unsigned int EndVertex;
		double Stiffness;
		double RestLength;
	};
}
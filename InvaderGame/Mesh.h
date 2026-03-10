#pragma once

#include "EngineFramework.h"
#include "Direct3D.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool Load(const std::string& path);

	const std::vector<VertexType3D>& GetVertices() const { return _vertices; }
	const std::vector<unsigned int>& GetIndices() const { return _indices; }

private:
	std::vector<VertexType3D> _vertices;
	std::vector<unsigned int> _indices;
};
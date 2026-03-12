#pragma once

#include "EngineBehaviour.h"
#include "Mesh.h"

class MeshRenderer : public EngineBehaviour
{
public:
	NAME(MeshRenderer)
		SERIALIZE3(EngineBehaviour,
			)

	void Start() override;
	void Update() override;

private:
	Mesh* _mesh;
};
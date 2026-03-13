#pragma once

#include "EngineBehaviour.h"
#include "Mesh.h"

class MeshRenderer : public EngineBehaviour
{
public:
	NAME(MeshRenderer)
		SERIALIZE3(EngineBehaviour,
			)

	MeshRenderer();
	void Start() override;
	void Update() override;
	void EditorUpdate() override;

private:
	Mesh* _mesh;
	void Render();
};
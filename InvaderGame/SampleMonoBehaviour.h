#pragma once

#include "MonoBehaviour.h"

class SampleMonoBehaviour : public MonoBehaviour
{
public:
	SampleMonoBehaviour();
	void OnCollisionEnter2D(Collision2D* collision) override;

private:
	
};
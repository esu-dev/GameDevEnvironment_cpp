#include "SampleMonoBehaviour.h"

#include "GameEngine.h"

SampleMonoBehaviour::SampleMonoBehaviour()
{
	
}

void SampleMonoBehaviour::OnCollisionEnter2D(Collision2D* collision)
{
	collision->GetCollider()->gameObject->SetActive(false);

	this->gameObject->GetComponent<AudioSource>()->Play();
}

void SampleMonoBehaviour::Update()
{
	
}
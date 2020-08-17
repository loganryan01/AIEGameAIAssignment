#include "AttackBehaviour.h"

Vector2 AttackBehaviour::Normalise(Vector2 vector2)
{
	Vector2 normalisedVector = { 0, 0 };

	if (vector2.x == NULL && vector2.y == NULL)
		return normalisedVector;

	normalisedVector = vector2;

	float magnitude = sqrtf((normalisedVector.x * normalisedVector.x) + (normalisedVector.y * normalisedVector.y));

	normalisedVector.x /= magnitude;
	normalisedVector.y /= magnitude;

	return normalisedVector;
}

Vector2 AttackBehaviour::Update(Agent* agent, float deltaTime)
{
	Vector2 desiredVelocity = { 0, 0 };
	Vector2 steering = { 0, 0 };

	// If the player is in range of the enemy then the enemy will attack
	// - Enemy will go to the player 
	// - Once they are within reach, attack

	desiredVelocity = Vector2Scale(Normalise(Vector2Subtract(m_target, agent->GetPosition())), m_maxVelocity);
	steering = Vector2Subtract(desiredVelocity, agent->GetVelocity());

	float distance = Vector2Distance(agent->GetPosition(), m_target);

	if (distance < agent->GetAttackRadius())
	{
		agent->ResetAttackCharger();
	}

	return steering;
}
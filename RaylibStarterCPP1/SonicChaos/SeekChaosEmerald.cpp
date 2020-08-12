#include "SeekChaosEmerald.h"

Vector2 SeekChaosEmerald::Normalise(Vector2 vector2)
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

bool SeekChaosEmerald::OnArrival()
{
	// TODO: complete this function
	// Get the player position
	// Check how far away the player is from the node
	// if the player is on the node then move to the next node
	SetTarget({ 800, 800 });
	return true;
}

Vector2 SeekChaosEmerald::Update(Agent* agent, float deltaTime)
{
	Vector2 desiredVelocity = { 0, 0 };
	Vector2 steering = { 0, 0 };
	float distance = 0;

	desiredVelocity = Vector2Subtract(m_target, agent->GetPosition());
	distance = Vector2Distance(agent->GetPosition(), desiredVelocity);

	if (distance < 100.0f)
	{
		//desiredVelocity = Vector2Scale(Vector2Scale(Normalise(desiredVelocity), m_maxVelocity), (distance / 100.0f));
		OnArrival();
	}
	else
		desiredVelocity = Vector2Scale(Normalise(desiredVelocity), m_maxVelocity);

	steering = Vector2Subtract(desiredVelocity, agent->GetVelocity());
	return steering;
}
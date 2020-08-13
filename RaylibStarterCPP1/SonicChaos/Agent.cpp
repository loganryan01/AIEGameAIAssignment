#include "Agent.h"
#include "Behaviour.h"

Agent::Agent()
{}

Agent::~Agent()
{}

// Update the agent and its behaviours
void Agent::Update(float deltaTime)
{
	Vector2 force = m_behaviour->Update(this, deltaTime);

	m_velocity = (Vector2Add(m_velocity, Vector2Scale(force, deltaTime)));
	m_position = (Vector2Add(m_position, Vector2Scale(m_velocity, deltaTime)));
}

// Draw the agent
void Agent::Draw()
{
	DrawRectangle(m_position.x, m_position.y, 16, 16, RED);
}

// Add a behaviour to the agent
void Agent::SetBehaviour(Behaviour* behaviour)
{
	m_behaviour = behaviour;
}
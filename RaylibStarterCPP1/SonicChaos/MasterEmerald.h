#pragma once
#include "GameObject.h"

class MasterEmerald : public GameObject
{
public:
	MasterEmerald();
	virtual ~MasterEmerald() {};

	virtual void Draw();

	void SetTexture(Texture2D texture) { m_texture = texture; }
	Texture2D GetTexture() { return m_texture; }

	float GetRadius() { return m_radius; }

private:
	Texture2D m_texture;

	float m_radius = 32.0f;
};


#pragma once
#include "GameObject.h"

class ChaosEmerald : public GameObject
{
public:
	ChaosEmerald();
	virtual ~ChaosEmerald() {};

	virtual void Draw();

	void SetTexture(Texture2D texture) { m_texture = texture; }
	Texture2D GetTexture() { return m_texture; }

private:
	Texture2D m_texture;
};


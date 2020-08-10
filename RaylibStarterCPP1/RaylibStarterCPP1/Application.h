#pragma once

class Application
{
public:

	Application(int windowWidth, int windowHeight);
	~Application();

	void Run();

	void Update(float deltaTime);
	void Draw();

protected:
private:

	int m_windowWidth;
	int m_windowHeight;
};


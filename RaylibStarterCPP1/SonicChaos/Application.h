#pragma once

class GameStateManager;

class Application
{
public:
	// Constructor
	Application(int windowWidth, int windowHeight);

	// Destructor
	~Application();

	// Run the game
	void Run();

	// Update the game
	void Update(float deltaTime);

	// Draw all the pictures in the game
	void Draw();

	GameStateManager* GetGameStateManager()
	{
		return m_gameStateManager;
	}

	int GetWindowWidth() { return m_windowWidth; }
	int GetWindowHeight() { return m_windowHeight; }

private:
	int m_windowWidth;
	int m_windowHeight;

	GameStateManager* m_gameStateManager = nullptr;
};


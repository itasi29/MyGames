#pragma once

class Application
{
private:
	Application();
	
	Application(const Application& app) = delete;
	void operator= (const Application& app) = delete;

public:
	static Application& GetInstance();

	bool Init();
	void Run();
	void Terminate();

	/// <summary>
	/// ÉQÅ[ÉÄÇèIóπÇ≥ÇπÇÈ
	/// </summary>
	void End() { m_isEnd = true; }

private:
	bool m_isEnd;
	bool m_isWindows;
};


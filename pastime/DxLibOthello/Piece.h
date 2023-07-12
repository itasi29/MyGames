#pragma once

class Piece
{
public:
	Piece();
	~Piece();

	void Init();
	void Update();
	void Draw() const;

private:
	int m_posX;
	int m_posY;
};
#ifndef _VB_TRAILS_H_
#define _VB_TRAILS_H_
#include "VBGO.h"
#include "vertex.h"
#include "gamedata.h"

class Trails : public VBGO
{
public:

	Trails(ID3D11Device* pd3dDevice);
	~Trails();
	void Tick(GameData* GD);
	virtual void Transform()
	{
		for (int i = 0; i < m_numPrims * 3; i++)
		{
			m_vertices[i].Color *= ((i / 3) % 2) ? 1.0f : 0.5f;
			m_vertices[i].Color.w = 1.0f;

			Vector3 vertPosition = m_vertices[i].Position;
			Vector3 spherePosition = m_vertices[i].Position;

			spherePosition.Normalize();

			m_vertices[i].Position = 3.0f * spherePosition;
		}
	}
	void ClearTrails();
	void Spawn(Vector3 _position);
	bool isAlive();
protected:
	bool m_alive;
	int m_size;
	myVertex* m_vertices;
};

#endif
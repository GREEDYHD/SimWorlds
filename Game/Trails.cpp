#include "Trails.h"
Trails::Trails(ID3D11Device* _pd3dDevice)
{
	//m_Velocity = _Velocity;
	m_scale *= 0.2;
	m_size = 11;
	m_alive = false;
	m_Position = Vector3(100.0f, 1.0f, 1.0f);
	//calculate number of vertices and primatives
	int numVerts = 6 * 6 * (m_size - 1) * (m_size - 1);
	m_numPrims = numVerts / 3;
	m_vertices = new myVertex[numVerts];
	WORD* indices = new WORD[numVerts];
	int vert = 0;
	//Vector3 m_distanceToParent = XMVectorSubtract(m_Position, m_point);
	//orbitalDistance = (sqrtf(sqrtf((m_distanceToParent.x * m_distanceToParent.x) + (m_distanceToParent.y + m_distanceToParent.y)) + (m_distanceToParent.z * m_distanceToParent.y)));

	//as using the standard VB shader set the tex-coords somewhere safe
	for (int i = 0; i < numVerts; i++)
	{
		indices[i] = i;
		m_vertices[i].texCoord = Vector2::One;
		m_vertices[vert].Color = Color(1.0f, 1.0f, 1.0f, 1.0f);
		vert++;
	}
	vert = 0;
	//in each loop create the two traingles for the matching sub-square on each of the six faces

	for (int i = -(m_size - 1) / 2; i < (m_size - 1) / 2; i++)
	{
		for (int j = -(m_size - 1) / 2; j < (m_size - 1) / 2; j++)
		{
			//top

			m_vertices[vert++].Position = Vector3((float)i, 0.5f * (float)(m_size - 1), (float)j);
			m_vertices[vert++].Position = Vector3((float)i, 0.5f * (float)(m_size - 1), (float)(j + 1));
			m_vertices[vert++].Position = Vector3((float)(i + 1), 0.5f * (float)(m_size - 1), (float)j);

			m_vertices[vert++].Position = Vector3((float)(i + 1), 0.5f * (float)(m_size - 1), (float)j);
			m_vertices[vert++].Position = Vector3((float)i, 0.5f * (float)(m_size - 1), (float)(j + 1));
			m_vertices[vert++].Position = Vector3((float)(i + 1), 0.5f * (float)(m_size - 1), (float)(j + 1));

			//back
			m_vertices[vert++].Position = Vector3((float)i, (float)j, 0.5f * (float)(m_size - 1));
			m_vertices[vert++].Position = Vector3((float)(i + 1), (float)j, 0.5f * (float)(m_size - 1));
			m_vertices[vert++].Position = Vector3((float)i, (float)(j + 1), 0.5f * (float)(m_size - 1));

			m_vertices[vert++].Position = Vector3((float)(i + 1), (float)j, 0.5f * (float)(m_size - 1));
			m_vertices[vert++].Position = Vector3((float)(i + 1), (float)(j + 1), 0.5f * (float)(m_size - 1));
			m_vertices[vert++].Position = Vector3((float)i, (float)(j + 1), 0.5f * (float)(m_size - 1));

			//right
			m_vertices[vert++].Position = Vector3(0.5f * (float)(m_size - 1), (float)i, (float)j);
			m_vertices[vert++].Position = Vector3(0.5f * (float)(m_size - 1), (float)(i + 1), (float)j);
			m_vertices[vert++].Position = Vector3(0.5f * (float)(m_size - 1), (float)i, (float)(j + 1));

			m_vertices[vert++].Position = Vector3(0.5f * (float)(m_size - 1), (float)(i + 1), (float)j);
			m_vertices[vert++].Position = Vector3(0.5f * (float)(m_size - 1), (float)(i + 1), (float)(j + 1));
			m_vertices[vert++].Position = Vector3(0.5f * (float)(m_size - 1), (float)i, (float)(j + 1));

			//Bottom
			m_vertices[vert++].Position = Vector3((float)j, -0.5f * (float)(m_size - 1), (float)i);
			m_vertices[vert++].Position = Vector3((float)(j + 1), -0.5f * (float)(m_size - 1), (float)i);
			m_vertices[vert++].Position = Vector3((float)j, -0.5f * (float)(m_size - 1), (float)(i + 1));

			m_vertices[vert++].Position = Vector3((float)j, -0.5f * (float)(m_size - 1), (float)(i + 1));
			m_vertices[vert++].Position = Vector3((float)(j + 1), -0.5f * (float)(m_size - 1), (float)i);
			m_vertices[vert++].Position = Vector3((float)(j + 1), -0.5f * (float)(m_size - 1), (float)(i + 1));

			//front
			m_vertices[vert++].Position = Vector3((float)j, (float)i, -0.5f * (float)(m_size - 1));
			m_vertices[vert++].Position = Vector3((float)j, (float)(i + 1), -0.5f * (float)(m_size - 1));
			m_vertices[vert++].Position = Vector3((float)(j + 1), (float)i, -0.5f * (float)(m_size - 1));

			m_vertices[vert++].Position = Vector3((float)j, (float)(i + 1), -0.5f * (float)(m_size - 1));
			m_vertices[vert++].Position = Vector3((float)(j + 1), (float)(i + 1), -0.5f * (float)(m_size - 1));
			m_vertices[vert++].Position = Vector3((float)(j + 1), (float)i, -0.5f * (float)(m_size - 1));

			//left
			m_vertices[vert++].Position = Vector3(-0.5f * (float)(m_size - 1), (float)j, (float)i);
			m_vertices[vert++].Position = Vector3(-0.5f * (float)(m_size - 1), (float)j, (float)(i + 1));
			m_vertices[vert++].Position = Vector3(-0.5f * (float)(m_size - 1), (float)(j + 1), (float)i);

			m_vertices[vert++].Position = Vector3(-0.5f * (float)(m_size - 1), (float)j, (float)(i + 1));
			m_vertices[vert++].Position = Vector3(-0.5f * (float)(m_size - 1), (float)(j + 1), (float)(i + 1));
			m_vertices[vert++].Position = Vector3(-0.5f * (float)(m_size - 1), (float)(j + 1), (float)i);
		}
	}

	//carry out some kind of transform on these vertices to make this object more interesting
	//Transform();


	//calculate the normals for the basic lighting in the base shader
	for (int i = 0; i<m_numPrims; i++)
	{
		WORD V1 = 3 * i;
		WORD V2 = 3 * i + 1;
		WORD V3 = 3 * i + 2;

		//build normals
		Vector3 norm;
		Vector3 vec1 = m_vertices[V1].Position - m_vertices[V2].Position;
		Vector3 vec2 = m_vertices[V3].Position - m_vertices[V2].Position;
		norm = vec1.Cross(vec2);
		norm.Normalize();

		m_vertices[V1].Norm = norm;
		m_vertices[V2].Norm = norm;
		m_vertices[V3].Norm = norm;
	}

	BuildIB(_pd3dDevice, indices);
	BuildVB(_pd3dDevice, numVerts, m_vertices);

	delete[] m_vertices; //this is no longer needed as this is now in the Vertex Buffer
}
Trails::~Trails()
{

}
void Trails::Tick(GameData* GD)
{
	VBGO::Tick(GD);
}
void Trails::Spawn(Vector3 _position)
{
	m_alive = true;
	m_Position = _position;
}
void Trails::ClearTrails()
{
	m_alive = false;
}
bool Trails::isAlive()
{
	return m_alive;
}
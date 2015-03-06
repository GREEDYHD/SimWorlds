#include "VBSphere.h"
VBSphere::VBSphere(ID3D11Device* _pd3dDevice)
{
	//m_Position = Vector3(0,0,0);
	//m_Velocity = _Velocity;
	m_scale = 2.0f  * m_scale;
	m_size = 2;
	
	m_Offset.x = m_Position.x + m_size;
	m_Offset.y = m_Position.y + m_size;
	m_Offset.z = m_Position.z + m_size;

	m_distanceSquared = 0;
	m_GForce = 0;


	int numVerts = 6 * 6;
	m_numPrims = numVerts / 3;

	m_vertices = new myVertex[numVerts];
	WORD* indices = new WORD[numVerts];
	time = 0;
	int vert = 0;

	for (int i = 0; i < numVerts; i++)
	{
		indices[i] = i;
		m_vertices[i].texCoord = Vector2::One;
		m_vertices[vert].Color = Color(1.0f, 1.0f, 1.0f, 1.0f);
		vert++;
	}
	vert = 0;


	/*m_vertices[vert++].Position = Vector3(-m_Offset.x, m_Offset.y, m_Offset.z);
	m_vertices[vert++].Position = Vector3(-m_Offset.x, -m_Offset.y, m_Offset.z);
	m_vertices[vert++].Position = Vector3(-m_Offset.x, -m_Offset.y, -m_Offset.z);*/

	m_vertices[vert++].Position = Vector3(-m_Offset.x, -m_Offset.y, -m_Offset.z);//-,-,-
	m_vertices[vert++].Position = Vector3(m_Offset.x, -m_Offset.y, -m_Offset.z);//+,-,-
	m_vertices[vert++].Position = Vector3(-m_Offset.x, m_Offset.y, -m_Offset.z);//+,+,-

	m_vertices[vert++].Position = Vector3(m_Offset.x, m_Offset.y, -m_Offset.z);//
	m_vertices[vert++].Position = Vector3(-m_Offset.x, m_Offset.y, -m_Offset.z);//
	m_vertices[vert++].Position = Vector3(m_Offset.x, -m_Offset.y, -m_Offset.z);//

	/*
		//calculate number of vertices and primatives
		int numVerts = 6 * 6 * (m_size - 1) * (m_size - 1);
		m_numPrims = numVerts / 3;
		m_vertices = new myVertex[numVerts];
		WORD* indices = new WORD[numVerts];
		time = 0;
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
		Transform();
	*/

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
VBSphere::~VBSphere()
{

}

void VBSphere::Tick(GameData* GD, Vector3 _ParticlesPosition)
{	
	_ParticlesPosition = Vector3(0, 0, 0);

	m_XYZ_Distance = XMVectorSubtract(_ParticlesPosition, m_Position);

	m_distanceSquared = m_XYZ_Distance.LengthSquared();
	
	m_GForce = 0.1;

	m_XYZ_Distance.Normalize();

	m_Velocity += m_XYZ_Distance * m_GForce;
	
	m_Position += m_Velocity;
	
	VBGO::Tick(GD);
}

void VBSphere::Spawn()
{
	m_Position = Vector3(150, 20 * (((float)rand() / (float)RAND_MAX) - 0.5f), 0.0f);
	//m_Position = Vector3(100 * (((float)rand() / (float)RAND_MAX) - 0.5f), 100 * (((float)rand() / (float)RAND_MAX) - 0.5f), -100 * (((float)rand() / (float)RAND_MAX) - 0.5f));
	m_Velocity = Vector3(10.0f, 3.0f, 0.0f);
	//m_Velocity = Vector3(30 * (((float)rand() / (float)RAND_MAX) - 0.5f), 10 * (((float)rand() / (float)RAND_MAX) - 0.5f), -10 * (((float)rand() / (float)RAND_MAX) - 0.5f));
}

void VBSphere::orbit(float _time)
{
	m_Position.x = m_point.x + (10.0f * (orbitalDistance * sin(time)));
	m_Position.z = m_point.z + (10.0f * (orbitalDistance * cos(time)));
	m_Position.y = m_point.y + 10.0f * (orbitalDistance * (sin(time) + cos(time)));
}
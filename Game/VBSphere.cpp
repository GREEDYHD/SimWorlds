#include "VBSphere.h"
VBSphere::VBSphere(ID3D11Device* _pd3dDevice)
{
	//m_Position = _Position;
	//m_Velocity = _Velocity;
	m_size = 11;
	m_scale = 0.5f  * m_scale;
	m_distance = 0;
	m_GForce = 0;
	Theta = Vector3(0, 0, 0);

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

	//Matrix ParticleProperties = Matrix(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	//time += 3.0f * GD->dt;
	//orbit(time)
	m_XYZ_Distance = XMVectorSubtract(m_Position, _ParticlesPosition);
	m_distance = ((sqrtf(sqrtf(((m_XYZ_Distance.x * m_XYZ_Distance.x) + (m_XYZ_Distance.y + m_XYZ_Distance.y)) + (m_XYZ_Distance.z * m_XYZ_Distance.y)))));
	//if (m_distance < 0.000001)
	//{
	m_GForce = GravitaionalConstant * 1.0/(m_distance * m_distance);
	//}
	//else m_GForce = 0;
	
	Theta = Vector3(XMVectorSubtract(m_Position,m_Velocity));
		

	if (m_XYZ_Distance.x < 0)
	{
		m_Velocity.x -= m_GForce * cos(Theta.x);
	}
	else
	{
		m_Velocity.x += m_GForce * cos(Theta.x);
	}
	if (m_XYZ_Distance.y < 0)
	{
		m_Velocity.y -= m_GForce * cos(Theta.y);
	}
	else
	{
		m_Velocity.y += m_GForce * cos(Theta.y);
	}
	if (m_XYZ_Distance.z < 0)
	{
		m_Velocity.z -= m_GForce * cos(Theta.z);
	}
	else
	{
		m_Velocity.z += m_GForce * cos(Theta.z);
	}

	/*m_Velocity.y += m_GForce * cos(Theta.y);
	m_Velocity.z += m_GForce * cos(Theta.z);*/

	//m_Velocity += _ParticlesPosition;
	m_Position = XMVectorAdd(m_Position, m_Velocity);
	VBGO::Tick(GD);
}
void VBSphere::Spawn()
{
	m_Position = Vector3(10 * (((float)rand() / (float)RAND_MAX) - 0.5f), 10 * (((float)rand() / (float)RAND_MAX) - 0.5f), -10 * (((float)rand() / (float)RAND_MAX) - 0.5f));
	m_Velocity = Vector3(0.0f, 0.0f, 0.0f);
	//m_Velocity = Vector3(0.01 * (((float)rand() / (float)RAND_MAX) - 0.5f), 0.01 * (((float)rand() / (float)RAND_MAX) - 0.5f), -0.01 * (((float)rand() / (float)RAND_MAX) - 0.5f));
}

void VBSphere::orbit(float _time)
{
	m_Position.x = m_point.x + (10.0f * (orbitalDistance * sin(time)));
	m_Position.z = m_point.z + (10.0f * (orbitalDistance * cos(time)));
	m_Position.y = m_point.y + 10.0f * (orbitalDistance * (sin(time) + cos(time)));
}
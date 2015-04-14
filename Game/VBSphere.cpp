#include "VBSphere.h"
VBSphere::VBSphere(ID3D11Device* _pd3dDevice, Vector3 _position)
{
	m_Position = _position;
	//m_Velocity = _Velocity;
	m_scale *= 1; // gives us scale or r^3 if density and mass of object is 1
	m_size = 11;
	m_alive = false;
	trailNumber = 0;
	currentTrailNumber = 0;
	m_distanceSquared = 0;
	m_GForce = 0;
	for (int i = 0; i < 99; i++)
	{
		SphereTrails.push_back(new Trails(_pd3dDevice));
	}
		//calculate number of vertices and primatives
		int numVerts = 6 * 6 * (m_size - 1) * (m_size - 1);
		m_numPrims = numVerts / 3;
		m_vertices = new myVertex[numVerts];
		m_Color = Color((5 * ((float)rand() / (float)RAND_MAX) / 0.5f), (5 * ((float)rand() / (float)RAND_MAX) / 0.5f),(5 * ((float)rand() / (float)RAND_MAX) / 0.5f), 1.0f);
		WORD* indices = new WORD[numVerts];
		int vert = 0;
		//Vector3 m_distanceToParent = XMVectorSubtract(m_Position, m_point);
		//orbitalDistance = (sqrtf(sqrtf((m_distanceToParent.x * m_distanceToParent.x) + (m_distanceToParent.y + m_distanceToParent.y)) + (m_distanceToParent.z * m_distanceToParent.y)));

		//as using the standard VB shader set the tex-coords somewhere safe
		for (int i = 0; i < numVerts; i++)
		{
			indices[i] = i;
			m_vertices[i].texCoord = Vector2::One;
			m_vertices[vert].Color = m_Color;
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

void VBSphere::Draw(DrawData* DD)
{
	if (m_alive)
	{
		for (vector<Trails *>::iterator it = SphereTrails.begin(); it != SphereTrails.end(); it++)
		{
			if ((*it)->isAlive())
			{
				(*it)->Draw(DD);
			}
		}
		VBGO::Draw(DD);
	}
}

void VBSphere::Tick(GameData* GD)
{
	m_Position += m_Velocity;
	VBGO::Tick(GD);
	if (trailNumber == 100)
	{
 		SphereTrails[currentTrailNumber]->Spawn(m_Position);
		currentTrailNumber++;
		if (currentTrailNumber == 99)
		{
			currentTrailNumber = 0;
		}
		trailNumber = 0;
	}
	trailNumber++;
	for (vector<Trails *>::iterator it = SphereTrails.begin(); it != SphereTrails.end(); it++)
	{
		if ((*it)->isAlive())
		{
			(*it)->Tick(GD);
		}
	}
	if (abs(m_Position.y > 600))
	{
		DeSpawn();
	}
	if (abs(m_Position.x > 1000))
	{
		DeSpawn();
	}
}

void VBSphere::Spawn(Vector3 _position)
{
	for (vector<Trails *>::iterator it = SphereTrails.begin(); it != SphereTrails.end(); it++)
	{
		if ((*it)->isAlive())
		{
			(*it)->ClearTrails();
		}
	}
	m_alive = true;
	m_Position = _position;
	
	//m_Position = Vector3(500 * (((float)rand() / (float)RAND_MAX) / 0.5f), 500 * (((float)rand() / (float)RAND_MAX) / 0.5f), 0.0f);
	//m_Velocity = Vector3(0.07f * (((float)rand() / (float)RAND_MAX) / 0.5f), 0.07f * (((float)rand() / (float)RAND_MAX) / 0.5f), 0.0f);
	m_Velocity = Vector3(0, 0, 0);
	//m_mass = 10;
	m_mass = toupper(100 * ((float)rand() / (float)RAND_MAX) / 0.5f);
	float tmp = tolower((0.75f * XM_PI) * m_mass);
	m_scale = (Vector3(1, 1, 1) * tmp) * 0.05;
}

void VBSphere::DeSpawn()
{
	for (vector<Trails *>::iterator it = SphereTrails.begin(); it != SphereTrails.end(); it++)
	{
		if ((*it)->isAlive())
		{
			(*it)->ClearTrails();
		}
	}
	m_alive = false;
}

void VBSphere::CalculateVelocity(Vector3 _ParticlesPosition, int _ParticlesMass, Vector3 _ParticlesVelocity)
{
	Vector3 tempDistance = _ParticlesPosition - m_Position;
	m_distanceSquared = tempDistance.LengthSquared();
	m_GForce = 0.05 * (_ParticlesMass)/m_distanceSquared;
	tempDistance.Normalize();
	m_NewVelocity += _ParticlesVelocity + tempDistance * m_GForce;
}
void VBSphere::SetNewVelocity()
{
	m_Velocity = m_NewVelocity;
}
void VBSphere::CirculariseOrbit(Vector3 _ParticlesPosition, int _ParticlesMass, Vector3 _ParticlesVelocity)
{
	Vector3 tempDistance = _ParticlesPosition - m_Position;
	m_distanceSquared = tempDistance.Length();
	m_GForce = sqrt(0.05 * (_ParticlesMass) / m_distanceSquared);
	tempDistance.Normalize();
	m_NewVelocity += _ParticlesVelocity + tempDistance * m_GForce;
	m_NewVelocity = Vector3(sin(m_NewVelocity.x), sin(m_NewVelocity.y), sin(m_NewVelocity.z));
}
bool VBSphere::isColliding(Vector3 _ParticlesPosition)
{
	Vector3 tempDistance = _ParticlesPosition - m_Position;
	if (tempDistance.Length() < m_scale.x)
	{
		m_Velocity *= 0.01;
		return true;
	}
	else
	{
		return false;
	}
}
bool VBSphere::isAlive()
{
	return m_alive;
}

Vector3 VBSphere::GetVelocity()
{
	return m_Velocity;
}
Color VBSphere::GetColor()
{
	return m_Color;
}
int VBSphere::GetMass()
{
	return m_mass;
}
void VBSphere::SetMass(int _newMass)
{
	m_mass = _newMass;
	float tmp = tolower((0.75f * XM_PI) * m_mass);
	m_scale = (Vector3(1,1,1) * tmp) * 0.05;
}
void VBSphere::SetVelocity(Vector3 _newVelocity)
{
	m_NewVelocity = _newVelocity;
}
float VBSphere::GetScale()
{
	return m_scale.x;
}
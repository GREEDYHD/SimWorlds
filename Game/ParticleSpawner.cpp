#include "ParticleSpawner.h"
#include "GameData.h"


ParticleSpawner::ParticleSpawner(ID3D11Device* _pd3dDevice)
{
	m_Position = Vector3(0.0f, 0.0f, 0.0f);
	maxParticles = 15;
	m_Gravity = 0.0001;
	temp = Vector3(0.0f, 0.0f, 0.0f);
	m_runningSimulation = false;
	usingCircularOrbits = false;
	currentParticle = 0;
	burn = 0;
	Vector3 SpherePos = Vector3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < maxParticles; i++)
	{
		SpherePos += Vector3(0, 25 * (((float)rand() / (float)RAND_MAX) / 0.5f), 25 * (((float)rand() / (float)RAND_MAX) / 0.5f));
		if (!i)
		{
			Particles.push_back(new Particle(_pd3dDevice, SpherePos, PT_STAR));
		}
		else
		{
			Particles.push_back(new Particle(_pd3dDevice, SpherePos, PT_PLANET));
		}
	}
}
ParticleSpawner::~ParticleSpawner()
{

}
void ParticleSpawner::Draw(DrawData* DD)
{
	for (vector<Particle *>::iterator it = Particles.begin(); it != Particles.end(); it++)
	{
		(*it)->Draw(DD);
	}
}
void ParticleSpawner::Tick(GameData* GD)
{
	Vector3 m_PPos = Vector3(500 * (((float)rand() / (float)RAND_MAX) / 0.5f), 500 * (((float)rand() / (float)RAND_MAX) / 0.5f), 0.0f);
	if (GD->keyboard[DIK_Q] & 0x80 && !(GD->prevKeyboard[DIK_Q] & 0x80))
	{
		if (Particles[currentParticle]->isAlive())
		{
			Particles[currentParticle]->DeSpawn();
		}
	}
	if (GD->keyboard[DIK_W] & 0x80 && !(GD->prevKeyboard[DIK_W] & 0x80))
	{
		if (!Particles[currentParticle]->isAlive())
		{
			Particles[currentParticle]->Spawn(m_PPos);
		}
	}
	if (GD->keyboard[DIK_E] & 0x80 && !(GD->prevKeyboard[DIK_E] & 0x80) && currentParticle != 0)
	{
		currentParticle--;
	}
	if (GD->keyboard[DIK_R] & 0x80 && !(GD->prevKeyboard[DIK_R] & 0x80) && currentParticle != maxParticles - 1)
	{
		currentParticle++;
	}
	if (GD->mouse->rgbButtons[0] & 0x80 && !(GD->prevMouse->rgbButtons[0] & 0x80))
	{
		if (currentParticle != maxParticles - 1 && !Particles[currentParticle]->isAlive())
		{
			Particles[currentParticle]->Spawn(Vector3(GetMousePosition().x, GetMousePosition().y, 0));
			currentParticle++;
		}		
	}
	if (GD->keyboard[DIK_V] & 0x80 && GD->mouse->rgbButtons[0] & 0x80 && !(GD->prevMouse->rgbButtons[0] & 0x80))
	{
		Vector3 pVel = Vector3(GetMousePosition().x - Particles[currentParticle]->GetPos().x, GetMousePosition().y - Particles[currentParticle]->GetPos().y, 0) / 100;//Creates velocity vector based on mouse click
		Particles[currentParticle]->SetVelocity(pVel);
	}

	if (GD->keyboard[DIK_F] & 0x80 && !(GD->prevKeyboard[DIK_F] & 0x80))
	{
		SetGravity(0.5);
	}
	if (GD->keyboard[DIK_G] & 0x80 && !(GD->prevKeyboard[DIK_G] & 0x80))
	{
		SetGravity(2);
	}

	for (vector<Particle *>::iterator it = Particles.begin(); it != Particles.end(); it++)
	{
		
		Vector3 ParticlesPosition = Vector3(0.0f, 0.0f, 0.0f);
		int ParticlesMass = 0;
		Vector3 ParticlesVelocity = Vector3(0.0f, 0.0f, 0.0f);
		if ((*it)->isAlive())
		{
			for (vector<Particle *>::iterator it2 = Particles.begin(); it2 != Particles.end(); it2++)
			{
				if ((*it2)->isAlive())
				{
					if (it != it2)
					{
						ParticlesPosition = (*it2)->GetPosition();
						ParticlesMass = (*it2)->GetMass();
						if ((*it)->isColliding(ParticlesPosition, (*it)->GetType()) && (*it)->GetMass() >= (*it2)->GetMass()) 
						{
							//Different behaviour if colliding with planet or star
							if ((*it)->GetType() == PT_PLANET)
							{	
								int newMass = (*it)->GetMass() + (*it2)->GetMass();
								Vector3 prevMomentum = ((*it)->GetVelocity() * (*it)->GetMass()) + ((*it2)->GetVelocity()*(*it2)->GetMass());//Ensures conservation of momentum
								Vector3 newVelocity = (prevMomentum / newMass);
								(*it2)->DeSpawn();//Despawns the smaller planet as it will be consumed by the larger one
								(*it)->SetMass(newMass);
								(*it)->SetVelocity(newVelocity);
							}
							else if ((*it)->GetType() == PT_STAR)
							{
								burn++;
								if (burn == 5)
								{
									((*it2)->SetMass((*it2)->GetMass() - 1));//Reduces the mass as if its burning up
									burn = 0;
								}
							}
						}					
					}
				}
			(*it)->CalculateVelocity(ParticlesPosition, ParticlesMass, ParticlesVelocity, m_Gravity);
			}
		(*it)->SetNewVelocity();//Waits till all bodies have had their forces calculated before updating them
		}
	}
	for (vector<Particle *>::iterator it = Particles.begin(); it != Particles.end(); it++)
	{
		if ((*it)->isAlive())
		{
			(*it)->Tick(GD);
		}
	}
}
int* ParticleSpawner::GetCurrentParticle()
{
	return &currentParticle;
}
bool* ParticleSpawner::GetParticleAliveState()
{
	bool pAlive = Particles[currentParticle]->isAlive();
	return &pAlive;
}
Vector3* ParticleSpawner::GetParticlePosition()
{
	Vector3 pPosition = Particles[currentParticle]->GetPos();
	return &pPosition;
}
Vector3* ParticleSpawner::GetParticleVelocity()
{
	Vector3 pVelocity = Particles[currentParticle]->GetVelocity();
	return &pVelocity;
}
Color* ParticleSpawner::GetParticleColor()
{
	Color pColor = Particles[currentParticle]->GetColor();
	return &pColor;
}
int* ParticleSpawner::GetMaxParticles()
{
	return &maxParticles;
}

int* ParticleSpawner::GetParticleMass()
{
	int pMass = Particles[currentParticle]->GetMass();
	return &pMass;
}

Vector2 ParticleSpawner::GetMousePosition()
{
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	float x = -1920 / 2 + cursorPos.x;
	float y = 1080 / 2 - cursorPos.y;
	Vector2 cursorPositionVector = Vector2(x, y);
	return cursorPositionVector;
}
float* ParticleSpawner::GetGravity()
{
	return &m_Gravity;
}
void ParticleSpawner::SetGravity(float _modifier)
{
	m_Gravity *= _modifier;
}

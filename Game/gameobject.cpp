#include "gameobject.h"

using namespace std;

GameObject::GameObject()
{
	//set the Gameobject to the origin with no rotation and unit scaling 
	m_Position = Vector3::Zero;
	m_pitch = 0.0f;
	m_yaw = 0.0f;
	m_roll = 0.0f;
	m_scale = Vector3::One;

	m_worldMat = Matrix::Identity;
	m_fudge = Matrix::Identity;
}

GameObject::~GameObject()
{

}

void GameObject::Tick(GameData* _GD)
{
	//build up the world matrix depending on the new Positionition of the GameObject
	//the assumption is that this class will be inherited by the class that ACTUALLY changes this
	Matrix  scaleMat	= Matrix::CreateScale( m_scale);
	m_rotMat = Matrix::CreateFromYawPitchRoll(m_yaw, m_pitch, m_roll);//Positionsible not the best way of doing this!
	Matrix  transMat = Matrix::CreateTranslation(m_Position);

	m_worldMat = scaleMat *m_rotMat * transMat;
}
#include "tpscamera.h"
//a basic Third person camera which follows a give GameObject at a displacement of _dPosition

TPSCamera::TPSCamera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, GameObject* _target, Vector3 _up, Vector3 _dPosition)
	:Camera(_fieldOfView, _aspectRatio, _nearPlaneDistance, _farPlaneDistance, _target->GetPosition(), _up)
{
	m_targetObject = _target;
	m_dPosition = _dPosition;
}

TPSCamera::~TPSCamera()
{

}

void TPSCamera::Tick(GameData* _GD)
{
	//Set up Positionition of camera and target Positionition of camera based on new Positionition and orientation of target object
	m_target = m_targetObject->GetPosition();

	Matrix rotMat = Matrix::CreateRotationY(m_targetObject->GetYaw());
	m_Position = m_target + Vector3::Transform(m_dPosition, rotMat);

	//and then set up proj and view matrices
	Camera::Tick(_GD);
}
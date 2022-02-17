#include "ThirdPersonCamera.h"
#include "PARKEngine/PARKEngine.h"
#include "Matrix/Matrix.h"
#include "FirstPersonCamera.h"

ThirdPersonCamera::ThirdPersonCamera(): camTransform_(nullptr), camRigid_(nullptr), rotating_(false), changedCamera_(false), borders_(0)
{
}


ThirdPersonCamera::~ThirdPersonCamera()
{
	std::cout << "Destructora de ThirdPersonCamera" << std::endl;
}

void ThirdPersonCamera::start()
{
	//No se puede hacer en el start porque puede que la matriz no se haya creado toa
	cam_ = SceneManager::instance()->currentState()->getEntitiesWithComponent<Camera>()[0]->getComponent<Camera>();
	camTransform_ = cam_->getBrotherComponent<Transform>();
	camRigid_= cam_->getBrotherComponent<Rigidbody>();
	defaultPos_ = camTransform_->getPosition();

	cam_->lookAt(Vector3(0, 0, 0));
	camTransform_->yaw(-45, REF_SYSTEM::GLOBAL);
	camRigid_->setActive(false);
}

void ThirdPersonCamera::load(json file)
{
	addParameter(MAX_HEIGTH, file["maxHeigth"]);
	addParameter(MIN_HEIGTH, file["minHeigth"]);
	addParameter(borders_, file["borders"]);
}

void ThirdPersonCamera::receive(Message* msg)
{
	switch (msg->mType_)
	{
	case THIRD_PERSON_CAMERA:
	{
		camRigid_->setActive(false);

		//Update position and rotation
		Vector3 prevPos = camTransform_->getPosition();
		cam_->lookAt(Vector3(prevPos.x - 1, prevPos.y, prevPos.z - 1));
		camTransform_->pitch(-45);
		camTransform_->setPosition(defaultPos_);
		
		//Update rigidbody position
		camRigid_->setTransform(camTransform_);

		//Show cursor
		HUDManager::instance()->showMouseCursor();

		//Switch components
		getBrotherComponent<FirstPersonCamera>()->setActive(false);
		setActive(true);
		break;
	}
	default:
		break;
	}
}

bool ThirdPersonCamera::handleEvent(unsigned int time)
{
	//Standard increment (for camera transfomations)
	float stdIncr = ((float)time / 2);
	//Pillamos la info del ratón y de la ventana
	Vector2 mouse = { (float)InputManager::instance()->getMouse()->getMouseState().X.abs, (float)InputManager::instance()->getMouse()->getMouseState().Y.abs };
	Vector2 windowSize = { OgreManager::instance()->getWindowSizeX(),  OgreManager::instance()->getWindowSizeY() };

	//Incremento de la posición
	Vector3 delta = { 0,0,0 };

	//Change camera perspective (Third / first person)
	if (InputManager::instance()->isKeyDown("ChangeCamera"))
	{
		if(!changedCamera_)
		{
			changedCamera_ = true;
			send(new Message(MessageId::FIRST_PERSON_CAMERA));
			return true;
		}
	}


	//Third person
	else
	{
		changedCamera_ = false;

		//ADELANTE/ATRÁS
		if (mouse.y < windowSize.y * borders_ || InputManager::instance()->isKeyDown("MoveForwards"))
			delta += Vector3::UNIT_Y.crossProduct(camTransform_->right()) * stdIncr;
		else if (mouse.y > windowSize.y - windowSize.y * borders_ || InputManager::instance()->isKeyDown("MoveBack"))
			delta += Vector3::UNIT_Y.crossProduct(camTransform_->right()) * -stdIncr;

		//IZQUIERDA/DERECHA
		if (mouse.x < windowSize.x * borders_ || InputManager::instance()->isKeyDown("MoveLeft"))
			delta += camTransform_->right() * -stdIncr;
		else if (mouse.x > windowSize.x - windowSize.x * borders_ || InputManager::instance()->isKeyDown("MoveRight"))
			delta += camTransform_->right() * stdIncr;

		//Rueda del ratón para hacer zoom (no se como se pone esto en el archivo del input porque no son teclas como tales)
		//ZOOM IN/OUT
		if (InputManager::instance()->getMouse()->getMouseState().Z.rel > 0)
			delta += camTransform_->forward() * stdIncr * 1.5; //TODO: poner un parámetro de sensibilidad

		else if (InputManager::instance()->getMouse()->getMouseState().Z.rel < 0)
			delta += camTransform_->forward() * stdIncr * -1.5;

		//ROTACIONES
		if (InputManager::instance()->isKeyDown("RotateLeft"))
			orbit(-90);
		else if (InputManager::instance()->isKeyDown("RotateRight"))
			orbit(90);
		else
			rotating_ = false;

	}

	//Move the camera
	if (delta.length() != 0)
		moveCamera(delta);

	return false;
}


void ThirdPersonCamera::orbit(float degrees)
{
	if (!rotating_)
	{
		int index;
		index = (degrees > 0) ? 1 : -1;

		//Throw a ray
		Vector3 focus = OgreManager::instance()->raycast().second;
		Vector3 horizDistance = { focus.x - camTransform_->getPosition().x, 0 , focus.z - camTransform_->getPosition().z };
		//Rotate the camera
		camTransform_->yaw(degrees, REF_SYSTEM::GLOBAL);
		//Translate the camera getting the difference betwwen the 2 vectors
		camTransform_->translate(camTransform_->right() * index *horizDistance.length());
		camTransform_->translate(Vector3::UNIT_Y.crossProduct(camTransform_->right()) * -horizDistance.length());

		rotating_ = true;
	}
}

void ThirdPersonCamera::moveCamera(Vector3 deltaPos)
{
	//Note that what we want to be inside the map isn't the camera itself, but the point 
	//which it is heading (the "focus"). Therefore, we make the calculus with this focus in mind
	//(except for the Y coordinate)

	camTransform_->translate(deltaPos);
	//If the ray is out of the matrix or we want to go too up/down
	float y = camTransform_->getPosition().y;
	if (OgreManager::instance()->raycast().first == nullptr || y > MAX_HEIGTH || y < MIN_HEIGTH)
		camTransform_->translate(-deltaPos);
}
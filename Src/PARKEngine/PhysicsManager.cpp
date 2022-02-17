#include "PhysicsManager.h"

#include "btBulletDynamicsCommon.h"

std::unique_ptr<PhysicsManager> PhysicsManager::instance_;

btDiscreteDynamicsWorld * PhysicsManager::dynamicsWorld = nullptr;

std::vector<btCollisionShape*> PhysicsManager::shapes = std::vector<btCollisionShape*>();


PhysicsManager::PhysicsManager()
{
	// collision configuration contains default setup for memory , collision setup . Advanced users can create their own configuration
	collisionConfiguration = new btDefaultCollisionConfiguration();

	//use the default collision dispatcher . For parallel processing you can use a diffent dispatcher(see Extras / BulletMultiThreaded)
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	//btDbvtBroadphase is a good general purpose broadphase . You can also try outbtAxis3Sweep .
	overlappingPairCache = new btDbvtBroadphase();

	// the default constraint solver . For parallel processing you can use a different solver (see Extras / BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	//EL MUNDO FÍSICO
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	//Gravedad
	dynamicsWorld->setGravity(btVector3(DEFAULT_GRAVITY.x, DEFAULT_GRAVITY.y, DEFAULT_GRAVITY.z));
}


void PhysicsManager::initInstance()
{
	//Devuelve la instancia si exise, si no crea una nueva
	if (instance_.get() == nullptr)
		instance_.reset(new PhysicsManager());
}

PhysicsManager * PhysicsManager::instance()
{
	//Devuelve la instancia
	assert(instance_.get() != nullptr);
	return instance_.get();
}


void PhysicsManager::stepSimulation(unsigned int time)
{
	//Actualiza la física
	dynamicsWorld->stepSimulation((float)time / 1000);

	//Información de Debug sobre los Rigidbodys
	for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	{
		btCollisionObject * obj = dynamicsWorld->getCollisionObjectArray()[j];
		btRigidBody * body = btRigidBody::upcast(obj);
		btTransform trans;
		//Está en movimiento
		if (body && body->getMotionState())
			body->getMotionState()->getWorldTransform(trans);
		//Está parado (cogemos la posición inicial)
		else
			trans = obj->getWorldTransform();

		//Informa de la posición
		//printf(" world   pos  object  %d = %f ,%f ,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	}
}

void PhysicsManager::cleanupPhysics()
{
	
	//// remove the rigidbodies from the dynamics world and delete them
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject * obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody * body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	// delete collision shapes
	for (int j = 0; j< shapes.size(); j++)
	{
		btCollisionShape * shape = shapes[j];
		shapes[j] = 0;
		delete shape;
	}

	// delete dynamics world
	if(dynamicsWorld!= nullptr)
		delete dynamicsWorld;

	// delete 
	if(solver != nullptr)
		delete solver;

	// delete broadphase
	if(overlappingPairCache != nullptr)
		delete overlappingPairCache;

	// delete dispatcher
	if(dispatcher != nullptr)
		delete dispatcher;

	if (collisionConfiguration != nullptr) {
		delete collisionConfiguration;
	}
}

btRigidBody * PhysicsManager::createRigidBody( Shape forma, Vector3 dimensions, float mass)
{
	
	btCollisionShape* shape = nullptr;
	switch (forma)
	{
	case Shape::EmptyShape:
		shape = new btEmptyShape();
		break;
	case Shape::BoxShape:
		shape = new btBoxShape(btVector3(dimensions.x / 2, dimensions.y / 2, dimensions.z /2));
		break;
	case Shape::SphereShape:
		shape = new btSphereShape(dimensions.x);
		break;
	case Shape::CapsuleShape:
		shape = new btCapsuleShape(dimensions.x, dimensions.y);
		break;
	case Shape::PlaneShape:
		shape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
		break;
	case Shape::CylinderShape:
		shape = new btCylinderShape(btVector3(dimensions.x, dimensions.y, dimensions.z));
		break;
	case Shape::ConeShape:
		shape = new btConeShape(dimensions.x, dimensions.y);
		break;
	default:
		break;
	}

	//Añadimos la forma al vector
	shapes.push_back(shape);

	//Posición del rigidbody
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0,0,0));

	//Estado de movimiento empezando en la posición inicial del nodo
	btDefaultMotionState *motionState = new btDefaultMotionState(startTransform);
	//Inercia de la forma
	btVector3 localInertia(0, 0, 0);
	shape->calculateLocalInertia(mass, localInertia);
	
	//Creamos el Rigidbody y lo  añadimos al mundo
	btRigidBody* rigid;
	if(mass != 0)
		rigid = new btRigidBody(mass, motionState, shape, localInertia); //, localInertia)
	else
		rigid = new btRigidBody(mass, nullptr, shape, localInertia);
	rigid->setRestitution(0);
	dynamicsWorld->addRigidBody(rigid);

	//Lo devolvemos
	return rigid;
}

void PhysicsManager::addShape(btCollisionShape * shape)
{
	shapes.push_back(shape);
}




void PhysicsManager::addRigidBody(btRigidBody * rigid)
{
	dynamicsWorld->addRigidBody(rigid);
}

PhysicsManager::~PhysicsManager()
{
	std::cout << "Destructora de PhysicsManager" << std::endl;
	if (instance_ != nullptr) {
		cleanupPhysics();

		instance_.release();
	}
	
}


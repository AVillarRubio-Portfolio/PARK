#pragma once
#include <stdint.h>
#include <vector>

//En este mismo fichero están las clases de Listener y Emitter para que no haya que incluir 3 ficheros, con este sera
//suficiente ya que todo su contenido tiene que ver con mensajes y su forma de envio

enum MessageId {
	TRANSFORM_CHANGED,
	RIGIDBODY_CHANGED,

	//A partir de aquí, estos mensajes deberían ir en la lógica del juego y no en este archivo
	GAME_START,

	//Cámara
	FIRST_PERSON_CAMERA,
	THIRD_PERSON_CAMERA,

	//NPCs
	NPC_IN,
	NPC_OUT,
	NPC_SELECTED,
	NPC_ENTERED_ATTRACTION,

	//Construcciones
	CREATED_BUILDING,
	CANNOT_BUILD,
	DESTROYED_BUILDING,

	//Relacionados con la burocracia
	BANKRUPTCY,
	COLLECT_ALL_TAXES,

	//TODO: hacer structs y meter en el mensaje el tipo de tax
	COLLEXT_ELECTRIX_TAX,
	COLLEXT_MAINTEINANCE_TAX,

	NEW_UNLOCKED_BUILDING,//Aqui igual para decir cual ha sido
	IS_BUILDING_UNLOCKED,
	BUILDING_NOT_UNLOCKED,
	BUILDING_UNLOCKED,
};

typedef uint16_t header_t_;


class Transform;

//Mensaje por defecto del que se puede heredar para introducir más información
struct Message {
	Message(MessageId mType, header_t_ size = sizeof(Message)) :
		size_(size), mType_(mType) {
	}
	header_t_ size_;
	uint8_t mType_;
};

struct TransformChanged : public Message {
	TransformChanged(MessageId mType, Transform* transform, header_t_ size = sizeof(Message)) :
		Message(mType, size), transform_(transform) {
	}
	Transform* transform_;
};

//Es muy posible que para varios mensajes haya que hacer un struct para mandar al receptor algo mas que el tipo de mensaje, y por tanto
//a la hora de recibirlo hacer un static_cast para así saber que tipo de mensaje es, ya que todos los stuct heredaran de Message
namespace Architecture
{
	class Listener {
	public:
		Listener() {}
		virtual ~Listener() {};
		virtual void receive(Message* msg) {};
	};

	class Emitter {
	public:
		Emitter() {}
		virtual ~Emitter() {};
		
		//Necesario guardar los posibles receptores de tus mensajes para así no tener que mandarselo a todos
		virtual void registerListener(Listener* o){ listeners_.push_back(o); }
		//Borrar a 
		virtual void removeListener(Listener* o)
		{
			std::vector<Listener*>::iterator position = std::find(listeners_.begin(),
				listeners_.end(), o);
			if (position != listeners_.end())
				listeners_.erase(position);
		}
		virtual void send(Message* msg)
		{
			for (Listener* o : listeners_) {
				o->receive(msg);
			}
		}
	protected:
		std::vector<Listener*> listeners_;
	};
}


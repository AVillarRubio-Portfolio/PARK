# Cosas que sabemos que pueden hacerse mejor pero que no sabemos cómo / no nos ha dado tiempo:

* 1.Algunos acoplamientos y dependencias entre el proyecto del Motor y el de la Lógica
   - Las funciones de CallBackManager hacen referencia a nuestro propio juego, siendo CallbackManager del motor	
   - El tipo enumerado MessageId contiene enumerados de nuestro juego aparte de los del motor
     (eso si, los mensajes en sí están en el juego con sus correspondientes structs)
   - Necesidad de incluir las dependencias del Motor (Ogre, Bullet, CEGUI...) desde la Lógica (aunque sea indirectamente)

* 2.Fallos en el motor:
   - No tenemos una forma limpia de eliminar las entidades del estado de juego actual
   - Faltan funcionalidades en algunos Managers (como ResourceManager e InputManager) 
   - Funcionalidades incompletas / que no hemos tenido tiempo de terminar (clase Animation, 
     guardado de componentes a archivo, posibilidad de crear varias cámaras...) 


# Por el resto, creemos que hemos conseguido un motor funcional:
  -> Con una arquitetura basada en componentes
  -> Con otros patrones de diseño como Observer, ResourceManager, Singleton
  -> Y que usa otras ayudas como macros

* En lo que al juego respecta, está bastante incompleto debido al género que elegimos,
pero creemos que muestra bastante bien lo que es capaz de hacerse con el motor "PARKEngine"






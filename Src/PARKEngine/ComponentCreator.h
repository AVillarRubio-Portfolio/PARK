#pragma once
#include "EntityFactory.h"

// ·> Macro para el registro de factorías de componentes
/*
- #: Convierte el nombre del parámetro en un <String> -> Si Comp = Transform, entonces registerType("Transform", this)
- ##: Sirve para concatenar el nombre ->  Si Comp = Transform, entonces class TransformCreator, etc.

Usamos una variable 'static' porque la constructora de los objetos 'static' se llama antes de la llamada la main().
De esta manera, todas las factorías de componentes quedan registradas automáticamente antes de la ejecución del programa en sí.
*/
#define REGISTER_TYPE(Comp) \
    class Comp##Creator : public BaseCreator { \
    public: \
        Comp##Creator() \
        { \
            EntityFactory::registerType(#Comp, this); \
        } \
       virtual Component* createComponent() const { \
            return new Comp(); \
        } \
    }; \
    static Comp##Creator global_##Comp##Creator;
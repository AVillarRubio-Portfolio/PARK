#include "DatosEdificio.h"
#include "PriceTextBox.h"
#include "PARKEngine/ResourceManager.h"

DatosEdificio::DatosEdificio()
{
}


DatosEdificio::~DatosEdificio()
{
}

void DatosEdificio::load(json file)
{
	json js = ResourceManager::instance()->getJsonByKey("EdificioData.json");

	for (json jsonAux2 : js["Edificios"]) {
		Datos* aux = new Datos();

		json jsonAux = jsonAux2["parameters"];

		addParameter(aux->price_, jsonAux["Price"]);
		addParameter(aux->entryCost_, jsonAux["EntryCost"]);
		addParameter(aux->LockedPrice_, jsonAux["LockedPrice"]);
		addParameter(aux->Locked_, jsonAux["Locked"]);

		addParameter(aux->PeePeeRestore_, jsonAux["PeePee"]);
		addParameter(aux->HungryRestore_, jsonAux["Hungry"]);
		addParameter(aux->funRestore_, jsonAux["Fun"]);

		addParameter(aux->maxCola_, jsonAux["Tam_Cola"]);

		addParameter(aux->tam.x, jsonAux["Tam"]["x"]);
		addParameter(aux->tam.y, jsonAux["Tam"]["y"]);
		addParameter(aux->height_, jsonAux["Height"]);

		addParameter(aux->entry.x, jsonAux["Entry"]["x"]);
		addParameter(aux->entry.y, jsonAux["Entry"]["y"]);
		addParameter(aux->exit.x, jsonAux["Exit"]["x"]);
		addParameter(aux->exit.y, jsonAux["Exit"]["y"]);

		if (jsonAux.find("Duration") != jsonAux.end()) {
			if (addParameter(aux->duration_, jsonAux["Duration"]))
			{
				aux->duration_ *= 1000; //Conversión a milisegundos;
				aux->actDuration_ = aux->duration_;
			}
		}

		addParameter(aux->capacity_, jsonAux["Capacity"]);
		addParameter(aux->bName, jsonAux["bName"]);


		datos_.insert(std::make_pair(aux->bName, aux));
	}
}

Datos * DatosEdificio::findData(std::string s)
{
	auto it = datos_.find(s);
	if (it != datos_.end()) {
		return (*it).second;
	}
}

void DatosEdificio::setLocked(std::string s, bool b) {
	findData(s)->Locked_ = b;
}


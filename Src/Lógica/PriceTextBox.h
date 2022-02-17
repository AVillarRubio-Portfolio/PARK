#pragma once
#include "PARKEngine/TextBox.h"

class BureauCrazyManager;
class DatosEdificio;
struct Datos;
class PriceTextBox : public TextBox
{

public:
	PriceTextBox();
	~PriceTextBox();

	void load(json file);

	virtual void update(unsigned int time);
	virtual void receive(Message* msg) {};

	virtual void save(json& file) {};

	void changeTextPrice();

	virtual std::string getInfo() { return "PriceTextBox"; };

private:
	std::string buildName_;
	DatosEdificio* datosEdificio_;
	Datos* datos_;
	BureauCrazyManager* bureauCrazyManager_;

};

REGISTER_TYPE(PriceTextBox)
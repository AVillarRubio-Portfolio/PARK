#include "BureaucracyManager.h"

#include <PARKEngine/PARKEngine.h>
#include "Edificio.h"
#include "NPC.h"
#include "PARKMessages.h"
#include <ctime>
#include <cmath>
#include <cstdio>


BureauCrazyManager::BureauCrazyManager() : startedToCount_(false), bankruptcy_(false)
, ruptcyStartTime_(0), textDinero_(nullptr), infoNPC_(nullptr), selectedNPC_(nullptr),ingTime_(0)
, infoImpuestos_(nullptr)
{

	start = std::clock();
}

BureauCrazyManager::~BureauCrazyManager()
{
	std::cout << "Destructora de BureauCrazyManager" << std::endl;
}

void BureauCrazyManager::updateMoneyText()
{
	textDinero_->setText(std::to_string((int)actualMoney_) + " $");
	infoDinero_->setText("Dinero actual: " + std::to_string((int)actualMoney_) + " $");
}

void BureauCrazyManager::resetTimer() {
	std::cout << "RESET TIMER LLAMADO" << std::endl;
	start = std::clock();
	seconds = 0;
	minutes = 0;
	hours = 0;
	timer();
}


void BureauCrazyManager::checkBankRuptcy()
{
	
	if (!startedToCount_ && actualMoney_ < floorRuptcyMoney_) {
		ruptcyStartTime_ = clock(); //Empieza a contar
		startedToCount_ = true;
	}
	
	if (startedToCount_ && (((clock() - ruptcyStartTime_) / CLOCKS_PER_SEC ) >= ruptcyMaxTime_)) //Ha superado el tiempo maximo que se puede estar en negativo
	{
		bankruptcy_ = true;

	}
	if (actualMoney_ > floorRuptcyMoney_) { //Si vuelvo a estar en positivo dejo de contar
		startedToCount_ = false;
	}
}

void BureauCrazyManager::taxCollect(TaxType taxType)
{
	actualMoney_ -= bills_[taxType].amount_;

	updateMoneyText();


}

void BureauCrazyManager::taxCollectALL()
{
	for (TaxBill t : bills_) {
		actualMoney_ -= t.amount_;
	}

	updateMoneyText();

}

void BureauCrazyManager::addAmountToTax(TaxType type, float amount)
{
	bills_[type].amount_ += amount;
}

void BureauCrazyManager::changeTaxAmount(TaxType type, float amount)
{
	bills_[type].amount_ = amount;
}

void BureauCrazyManager::timer()
{
	ingTime_ = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	seconds = ingTime_;
	minutes = seconds / 60;
	hours = minutes / 60;

}

void BureauCrazyManager::cobradorImpuestos()
{

	if (electricidadCobrado != minutes && minutes != 0 && minutes % 2 == 0)
	{
		electricidadCobrado = minutes;
		taxCollect(TaxType::ELECTRIC_TAX);
		electric = true;
		maintenance = false;
	}
	if (electric)nextTax_->setText("Sig. Impuesto de electricidad: " + to_string(-(int)bills_[TaxType::MAINTENANCE_TAX].amount_) + " $" +
		" en : " + to_string(60-seconds %60));

	if (mantenimientoCobrado != minutes && minutes % 2 != 0) {
		mantenimientoCobrado = minutes;
		taxCollect(TaxType::MAINTENANCE_TAX);
		electric = false;
		maintenance = true;
	}

	if (maintenance)nextTax_->setText("Sig. Impuesto de mantenimiento: " + to_string(-(int)bills_[TaxType::ELECTRIC_TAX].amount_) +
		" $" + " en " + to_string(60 - seconds %60));
	
}



bool BureauCrazyManager::bankruptcy()
{
	return bankruptcy_;
}

void BureauCrazyManager::load(json file)
{
	for (json taxes : file["Taxes"])
	{
		TaxBill bill = { TaxType(taxes["Id"]), taxes["Amount"] };
		bills_.push_back(bill);
	}

	ruptcyMaxTime_ = file["BankRuptcy"]["ruptcyMaxTime"] ;
	floorRuptcyMoney_ = file["BankRuptcy"]["floorRuptcyMoney"];
	Time2Tax_ = file["BankRuptcy"]["InitialTime2Tax"];
	actualMoney_ = file["BankRuptcy"]["ActualMoney"];
	visitors_ = file["BankRuptcy"]["InitVisitors"];
	totalVisitors_ = file["BankRuptcy"]["InitVisitors"];
}


void BureauCrazyManager::update(unsigned int time)
{
	if (SceneManager::instance()->hasExitPlay()) {
		std::cout << "Update Bureaucracy: habiamos salido del StatePlay" << std::endl;
		resetTimer();
		SceneManager::instance()->resetExitedPlay();
	}

	//std::cout << "BureaucrazyManager::Update parameter (time): " << time << std::endl;

	timer();

	cobradorImpuestos();

	checkBankRuptcy();

	if (bankruptcy())
	{
		send(&Message(BANKRUPTCY));

	}

	//TODO ESTO DEBERÍA IR EN EL START/POR MENSAJES (mejor lo segundo)
	if (textDinero_ == nullptr) {
		textDinero_ = SceneManager::instance()->currentState()->getEntity("TextDinero")->getComponent<TextBox>();
		textDinero_->setText(std::to_string((int)actualMoney_) + " $");
	}
	if (infoDinero_ == nullptr) {
		infoDinero_ = SceneManager::instance()->currentState()->getEntity("InfoDinero")->getComponent<TextBox>();
		infoDinero_->setText("Dinero actual: " + std::to_string((int)actualMoney_) + " $");
	}
	if (infoNumPersonas_ == nullptr) {
		infoNumPersonas_ = SceneManager::instance()->currentState()->getEntity("InfoNumPersonas")->getComponent<TextBox>();
		infoNumPersonas_->setText("Visitantes Actuales: " + std::to_string(visitors_));
	}

	if (infoNumPersonasTotales_ == nullptr) {
		infoNumPersonasTotales_ = SceneManager::instance()->currentState()->getEntity("infoNumPersonasTotales_")->getComponent<TextBox>();
		infoNumPersonasTotales_->setText("Visitantes Totales Del Parque: " + std::to_string(totalVisitors_));
	}

	if (infoNPC_ == nullptr)
	{
		infoNPC_ = SceneManager::instance()->currentState()->getEntity("NPCInfoPanel")->getComponent<FrameWindowBox>();
		infoNPC_->hide();
	}
	if (infoImpuestos_ == nullptr) {
		infoImpuestos_ = SceneManager::instance()->currentState()->getEntity("TextTimer")->getComponent<TextBox>();
		infoImpuestos_->setText(to_string(hours) + " : " + to_string(minutes) + " : " + to_string(seconds % 60));
	}
	if (selectedNPC_ != nullptr)
		showNPCInfoBars(selectedNPC_);

	if (nextTax_ == nullptr) {
		nextTax_ = SceneManager::instance()->currentState()->getEntity("TextNextTax")->getComponent<TextBox>();
		nextTax_->setText("Sig. Impuesto: -15 $ en 60 segundos");
	}

	infoImpuestos_->setText(to_string(hours) + " : " + to_string(minutes) + " : " + to_string(seconds % 60));
}


void BureauCrazyManager::addUnlockedBuilding(const Edificio & building)
{
	unlockedBuildings_.push_back(building);
}

void BureauCrazyManager::receive(Message * msg)
{

	switch (msg->mType_)
	{
	case COLLECT_ALL_TAXES:
		taxCollectALL();
		break;
	case NEW_UNLOCKED_BUILDING:
		break;
	case IS_BUILDING_UNLOCKED:
	{
		IsBuildingUnlocked* unlMsg = static_cast<IsBuildingUnlocked*>(msg);
		if (isBuildingUnlocked(*unlMsg->edificio_))
			send(&BuildingUnlocked(BUILDING_UNLOCKED, unlMsg->edificio_));
		else 
			send(&BuildingNotUnlocked(BUILDING_NOT_UNLOCKED, unlMsg->edificio_));

		break;
	}
	case COLLEXT_ELECTRIX_TAX:
		taxCollect(ELECTRIC_TAX);
		break;
	case COLLEXT_MAINTEINANCE_TAX:
		taxCollect(MAINTENANCE_TAX);
		break;
	//Cambios de perspectiva en la cámara (esconde/muestra los paneles de gestión)
	case FIRST_PERSON_CAMERA: 
	{
		infoNPC_->hide();
		textDinero_->hide();
		SceneManager::instance()->currentState()->getEntity("ToolsPanel")->getComponent<FrameWindowBox>()->hide();
		SceneManager::instance()->currentState()->getEntity("ConstructButton")->getComponent<Button>()->hide();
		SceneManager::instance()->currentState()->getEntity("InfoButton")->getComponent<Button>()->hide();
		SceneManager::instance()->currentState()->getEntity("MainMenuButton")->getComponent<Button>()->hide();
		SceneManager::instance()->currentState()->getEntity("TextNextTax")->getComponent<TextBox>()->hide();
		SceneManager::instance()->currentState()->getEntity("TextTimer")->getComponent<TextBox>()->hide();
		//std::vector<Entity*> widgets = SceneManager::instance()->currentState()->getEntitiesWithComponent<Widget>();
		//for (Entity* e : widgets)
		//	e->getComponent<Widget>()->hide();
		break;
	}
	case THIRD_PERSON_CAMERA:
	{
		infoNPC_->show();
		textDinero_->show();
		SceneManager::instance()->currentState()->getEntity("ToolsPanel")->getComponent<FrameWindowBox>()->show();
		SceneManager::instance()->currentState()->getEntity("ConstructButton")->getComponent<Button>()->show();
		SceneManager::instance()->currentState()->getEntity("InfoButton")->getComponent<Button>()->show();
		SceneManager::instance()->currentState()->getEntity("MainMenuButton")->getComponent<Button>()->show();
		SceneManager::instance()->currentState()->getEntity("TextNextTax")->getComponent<TextBox>()->show();
		SceneManager::instance()->currentState()->getEntity("TextTimer")->getComponent<TextBox>()->show();
		break;
	}

	case NPC_SELECTED:
	{
		NPCSelected* npcSel = static_cast<NPCSelected*>(msg);
		setSelectedNPC(npcSel->selected_);
		break;
	}

	case NPC_IN:
		visitors_++;
		totalVisitors_++;
		infoNumPersonas_->setText("Visitantes Actuales: " + std::to_string(visitors_));
		infoNumPersonasTotales_->setText("Visitantes Totales Del Parque: " + std::to_string(totalVisitors_));
		break;
	case NPC_OUT:
		visitors_--;
		infoNumPersonas_->setText("Visitantes Actuales: " + std::to_string(visitors_));
		break;
	case NPC_ENTERED_ATTRACTION:
	{
		//Add money
		NPCEnteredAttraction* npcEntered = static_cast<NPCEnteredAttraction*>(msg);
		//std::cout << npcEntered->npc_->getEntity()->getName() << " entered " << npcEntered->attraction_->getBuildingName() << " for " << npcEntered->attraction_->getPrice() << "$" << std::endl;
		addMoney(npcEntered->attraction_->getEntryCost());

		//Play sound
		AudioManager::instance()->PLAY_3D_SOUND("money3d", npcEntered->attraction_->getBrotherComponent<Transform>()->getPosition());
		break;
	}

	default:
		break;
	}
}

bool BureauCrazyManager::isBuildingUnlocked(const Edificio & building)
{
	std::list<Edificio>::iterator findIter = std::find(unlockedBuildings_.begin(), unlockedBuildings_.end(),building);

	return findIter != unlockedBuildings_.end();
}

void BureauCrazyManager::addMoney(float f)
{
	actualMoney_ += f;
	
	updateMoneyText();
}

void BureauCrazyManager::showNPCInfoBars(NPC * npc)
{
	if (npc->leavesPark()) {
		infoNPC_->hide();
	} else {
		//Más comodidad
		Stat fun = npc->getFun();
		Stat hunger = npc->getHunger();
		Stat peepee = npc->getPeepee();

		infoNPC_->setProperty("Text", npc->getEntity()->getName());
		static_cast<ProgressBar*>(infoNPC_->getChildElement("FunBar"))->setProgress(fun.value_ / fun.MAX_VALUE);
		static_cast<ProgressBar*>(infoNPC_->getChildElement("HungerBar"))->setProgress((hunger.MAX_VALUE - hunger.value_) / hunger.MAX_VALUE);
		static_cast<ProgressBar*>(infoNPC_->getChildElement("PeePeeBar"))->setProgress((peepee.MAX_VALUE - peepee.value_) / peepee.MAX_VALUE);
		infoNPC_->getChildElement("LastAttractionText")->setText("Viene de: " + npc->getLastAttraction());
	}
}

void BureauCrazyManager::setSelectedNPC(NPC * npc)
{
	selectedNPC_ = npc;
	infoNPC_->show();
}




#include "NPC.h"
#include <iostream>

NPC::NPC()
{
}

NPC::~NPC()
{
}

void NPC::load(json file)
{
	hunger = file["hunger"];
	thirst = file["thirst"];
}
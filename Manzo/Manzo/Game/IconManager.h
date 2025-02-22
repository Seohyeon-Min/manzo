#pragma once

#include "Icon.h"
#include "..\Engine\GameObject.h"
#include <list>

class IconManager
{
public:
	void						Add(Icon* icon);
	void						UpdateAll(double dt);
	void						Unload();
	void						DrawAll();

private:
	std::vector <Icon*>			icons;

};
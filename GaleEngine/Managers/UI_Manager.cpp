#include "UI_Manager.h"

using namespace Managers;
using namespace std;

UI_Manager UI_Manager::instance;

void UI_Manager::Init() {

}

UI_Manager& UI_Manager::Get() {
	return instance;
}

UI_Manager::UI_Manager() {}
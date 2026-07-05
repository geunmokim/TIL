#pragma once

#define WINCX 800
#define WINCY 600

#define SAFE_DELETE(p) if (p) { delete p; p = nullptr; }

#define OBJ_NOEVENT 0
#define OBJ_DEAD 1

#define PI 3.141592f
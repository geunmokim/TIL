#pragma once
namespace BULLET                             //namespace 붙인 이유 -> BULLET과 MONSTER가 DIR,LEFT,RIGHT로 같은 이름을 사용중이기에 namespace로 분류한것
{
	enum DIR { LEFT, RIGHT , UP, DOWN,MY_DIR, END};
}

namespace OBJID
{
	enum ID{ PLAYER, BULLET,ARROW, MONSTER, MOUSE,SHIELD,UI, END};
}

namespace RENDERID
{
	enum ID { BACKGROUND, OBJECT, EFFECT, UI, END};
}
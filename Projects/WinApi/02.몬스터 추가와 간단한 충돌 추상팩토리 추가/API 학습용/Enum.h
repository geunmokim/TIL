#pragma once
namespace BULLET                             //namespace 붙인 이유 -> BULLET과 MONSTER가 DIR,LEFT,RIGHT로 같은 이름을 사용중이기에 namespace로 분류한것
{
	enum DIR { LEFT, RIGHT , UP, DOWN, END};
}

namespace OBJID
{
	enum ID{ PLAYER, BULLET, MONSTER, END};
}

namespace MONSTER
{
	enum DIR{LEFT, RIGHT};
}

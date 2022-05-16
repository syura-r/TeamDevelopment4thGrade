#include "BaseLocus.h"
#include "ObjectManager.h"
#include "DrawMode.h"

BaseLocus::BaseLocus(const float arg_angle)
	:angle(arg_angle)
{
	lines.clear();
}

BaseLocus::~BaseLocus()
{
}

Line* BaseLocus::GetLine(const int arg_num)
{
	return lines[arg_num];
}

int BaseLocus::GetMaxNumLine()
{
	return lines.size();
}

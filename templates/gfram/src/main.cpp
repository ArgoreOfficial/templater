#include <stdio.h>

#include <Core/cApplication.h>

int main()
{
	cApplication::create();
	cApplication::getInstance().run();
	cApplication::destroy();
}
#include "Application.h"


int main() 
{
	Application app(Application::width4K, Application::height4K, "OpenGL App");
	app.Run();
	return 0;
}
#include "Application.h"


int main() 
{

	Application app(Application::widthHD, Application::heightHD, "OpenGL App");
	app.Run();
	return 0;
}
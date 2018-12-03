#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;


//using namespace std;



[STAThread]
int main(array<System::String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	

	RayTracer7::MyForm form;
	Application::Run(%form);
	return 0;

	
}

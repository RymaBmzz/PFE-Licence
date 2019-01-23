// pfe_gui.cpp : main project file.

#include "stdafx.h"
#include "Form1.h"
#include "Accueil.h"

using namespace pfe_gui;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	MessageBox::Show("Lancement de l'application...","EN COURS");
	// Create the main window and run it
	Application::Run(gcnew Accueil());
	MessageBox::Show(" Fermeture de l'application ,Merci de l'avoir utiliser !","Fermeture");
	return 0;
}

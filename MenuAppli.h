#pragma once
#include "simul.h"
#include "Form1.h"


namespace pfe_gui {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MenuAppli
	/// </summary>
	public ref class MenuAppli : public System::Windows::Forms::Form
	{
	public:
		MenuAppli(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MenuAppli()
		{
			if (components)
			{
				delete components;
			}
		}
//		int simul=0;
	private: System::Windows::Forms::Label^  Welcome;
	protected: 
	private: System::Windows::Forms::Button^  Simulation;
	private: System::Windows::Forms::Button^  Result;
	private: System::Windows::Forms::PictureBox^  PhotoMenu;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MenuAppli::typeid));
			this->Welcome = (gcnew System::Windows::Forms::Label());
			this->Simulation = (gcnew System::Windows::Forms::Button());
			this->Result = (gcnew System::Windows::Forms::Button());
			this->PhotoMenu = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PhotoMenu))->BeginInit();
			this->SuspendLayout();
			// 
			// Welcome
			// 
			this->Welcome->AutoSize = true;
			this->Welcome->BackColor = System::Drawing::Color::Transparent;
			this->Welcome->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 12, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->Welcome->Location = System::Drawing::Point(32, 9);
			this->Welcome->Name = L"Welcome";
			this->Welcome->Size = System::Drawing::Size(234, 21);
			this->Welcome->TabIndex = 0;
			this->Welcome->Text = L"Bienvenue sur Notre Application";
			// 
			// Simulation
			// 
			this->Simulation->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)), 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->Simulation->Location = System::Drawing::Point(20, 50);
			this->Simulation->Name = L"Simulation";
			this->Simulation->Size = System::Drawing::Size(265, 86);
			this->Simulation->TabIndex = 1;
			this->Simulation->Text = L"Jeu Puzzle";
			this->Simulation->UseVisualStyleBackColor = true;
			this->Simulation->Click += gcnew System::EventHandler(this, &MenuAppli::Simulation_Click);
			// 
			// Result
			// 
			this->Result->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)), 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->Result->Location = System::Drawing::Point(22, 160);
			this->Result->Name = L"Result";
			this->Result->Size = System::Drawing::Size(263, 97);
			this->Result->TabIndex = 2;
			this->Result->Text = L"Traitement de l\'image";
			this->Result->UseVisualStyleBackColor = true;
			this->Result->Click += gcnew System::EventHandler(this, &MenuAppli::Result_Click);
			// 
			// PhotoMenu
			// 
			this->PhotoMenu->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"PhotoMenu.BackgroundImage")));
			this->PhotoMenu->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->PhotoMenu->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"PhotoMenu.Image")));
			this->PhotoMenu->Location = System::Drawing::Point(306, 50);
			this->PhotoMenu->Name = L"PhotoMenu";
			this->PhotoMenu->Size = System::Drawing::Size(271, 207);
			this->PhotoMenu->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->PhotoMenu->TabIndex = 3;
			this->PhotoMenu->TabStop = false;
			// 
			// MenuAppli
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::Gainsboro;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->ClientSize = System::Drawing::Size(605, 285);
			this->Controls->Add(this->PhotoMenu);
			this->Controls->Add(this->Result);
			this->Controls->Add(this->Simulation);
			this->Controls->Add(this->Welcome);
			this->Cursor = System::Windows::Forms::Cursors::Hand;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"MenuAppli";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"Menu";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MenuAppli::MenuAppli_FormClosing);
			this->Load += gcnew System::EventHandler(this, &MenuAppli::MenuAppli_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PhotoMenu))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Result_Click(System::Object^  sender, System::EventArgs^  e) {
             
					Form1^ frm= gcnew Form1();
				 frm->ShowDialog();

			 }

			 /************************************************************/
			 /*public void aficherSegmenter(PictureBox pp,string ordre)
        {
            if (ordre == "afficher")
            {
                Bitmap bmp = new Bitmap(x,y);
               
                for (int jjj = 0; jjj < ListRegionConex.Count ; jjj++)
                {

                    for (int iii = 0; iii < ListRegionConex[jjj].list40.Count; iii++)
                    {
                        bmp.SetPixel(ListRegionConex[jjj].list40[iii].i, ListRegionConex[jjj].list40[iii].j, Color.FromArgb(255, ListRegionConex[jjj].list40[0].R, ListRegionConex[jjj].list40[0].G, ListRegionConex[jjj].list40[0].B));

                    }


                }
                pp.Image = bmp;

            }*/
			 /************************************************************/
	private: System::Void Simulation_Click(System::Object^  sender, System::EventArgs^  e) {
				/* int Pfirst=0; //faut le déclarer comme param global

				 if(Pfirst != 1 )
				 {
				 MessageBox::Show("Veuillez lancer la simulation en premier !" ,"Erreur");
				 }
				 else { Pfirst=0;*/
				 simul^ objsim= gcnew simul();
				 objsim->ShowDialog();
				 
				 
				 
			 }
			 /*************************************/
	private: System::Void MenuAppli_Load(System::Object^  sender, System::EventArgs^  e) {
                  
				
			 }
private: System::Void MenuAppli_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
/*			 Accueil^ objacc=gcnew Accueil();
			 objacc->ShowDialog();*/
		 }
};
}

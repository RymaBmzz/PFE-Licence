#pragma once
#include "MenuAppli.h"
#include "Aide.h"
namespace pfe_gui {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Accueil
	/// </summary>
	public ref class Accueil : public System::Windows::Forms::Form
	{
	public:
		Accueil(void)
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
		~Accueil()
		{
			if (components)
			{
				delete components;
			}
		}


	private: System::Windows::Forms::Label^  label1;
	protected: 
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Button^  Exitbut;

	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  Propos;







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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Accueil::typeid));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->Exitbut = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->Propos = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->BackColor = System::Drawing::Color::Transparent;
			this->label1->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 15, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->ForeColor = System::Drawing::Color::DarkBlue;
			this->label1->Location = System::Drawing::Point(32, 23);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(709, 38);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Université des Sciences et de la Technologie Houari Boumediene";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->label1->Click += gcnew System::EventHandler(this, &Accueil::label1_Click);
			// 
			// label2
			// 
			this->label2->BackColor = System::Drawing::Color::Transparent;
			this->label2->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 12, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label2->ForeColor = System::Drawing::Color::DarkBlue;
			this->label2->Location = System::Drawing::Point(479, 76);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(327, 43);
			this->label2->TabIndex = 1;
			this->label2->Text = L"Faculté d\'Electronique et d\'Informatique ";
			this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->label2->Click += gcnew System::EventHandler(this, &Accueil::label2_Click);
			// 
			// label3
			// 
			this->label3->BackColor = System::Drawing::Color::Transparent;
			this->label3->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 12, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label3->ForeColor = System::Drawing::Color::DarkBlue;
			this->label3->Location = System::Drawing::Point(524, 119);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(236, 37);
			this->label3->TabIndex = 2;
			this->label3->Text = L"Département Informatique ";
			this->label3->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->label3->Click += gcnew System::EventHandler(this, &Accueil::label3_Click);
			// 
			// label4
			// 
			this->label4->BackColor = System::Drawing::Color::Transparent;
			this->label4->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 12, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label4->ForeColor = System::Drawing::Color::DarkBlue;
			this->label4->ImageAlign = System::Drawing::ContentAlignment::TopCenter;
			this->label4->Location = System::Drawing::Point(9, 79);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(276, 43);
			this->label4->TabIndex = 3;
			this->label4->Text = L"Licence Informatique Académique ";
			this->label4->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->label4->Click += gcnew System::EventHandler(this, &Accueil::label4_Click);
			// 
			// label5
			// 
			this->label5->BackColor = System::Drawing::Color::Transparent;
			this->label5->Font = (gcnew System::Drawing::Font(L"Times New Roman", 20.25F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)), 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->label5->ForeColor = System::Drawing::Color::DarkBlue;
			this->label5->Location = System::Drawing::Point(92, 166);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(618, 116);
			this->label5->TabIndex = 4;
			this->label5->Text = L"Conception d\'un jeu intéractif basé sur la gestuelle de la main";
			this->label5->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->label5->Click += gcnew System::EventHandler(this, &Accueil::label5_Click);
			// 
			// Exitbut
			// 
			this->Exitbut->Location = System::Drawing::Point(680, 344);
			this->Exitbut->Name = L"Exitbut";
			this->Exitbut->Size = System::Drawing::Size(105, 27);
			this->Exitbut->TabIndex = 5;
			this->Exitbut->Text = L"Quitter";
			this->Exitbut->UseVisualStyleBackColor = true;
			this->Exitbut->Click += gcnew System::EventHandler(this, &Accueil::button1_Click_1);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(529, 344);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(112, 27);
			this->button2->TabIndex = 6;
			this->button2->Text = L"Démarrer ";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Accueil::button2_Click);
			// 
			// Propos
			// 
			this->Propos->Location = System::Drawing::Point(13, 346);
			this->Propos->Name = L"Propos";
			this->Propos->Size = System::Drawing::Size(82, 25);
			this->Propos->TabIndex = 7;
			this->Propos->Text = L"à Propos";
			this->Propos->UseVisualStyleBackColor = true;
			this->Propos->Click += gcnew System::EventHandler(this, &Accueil::Propos_Click);
			// 
			// Accueil
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::Gainsboro;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->ClientSize = System::Drawing::Size(815, 400);
			this->Controls->Add(this->Propos);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->Exitbut);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Cursor = System::Windows::Forms::Cursors::Hand;
			this->DoubleBuffered = true;
			this->Font = (gcnew System::Drawing::Font(L"Times New Roman", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->ForeColor = System::Drawing::Color::DarkBlue;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"Accueil";
			this->Padding = System::Windows::Forms::Padding(6, 5, 6, 5);
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Accueil";
			this->TransparencyKey = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Accueil::Accueil_FormClosing);
			this->Load += gcnew System::EventHandler(this, &Accueil::Accueil_Load_1);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void Accueil_Load(System::Object^  sender, System::EventArgs^  e) {
			 }

			 /***********************************************************/
	private: System::Void Accueil_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
				  
    if (MessageBox::Show("Voulez-vous quitter l'application?","Accueil",MessageBoxButtons::YesNo,MessageBoxIcon::Question)
					 == System::Windows::Forms::DialogResult::Yes)
	{  Application::ExitThread();
	//Accueil::Hide();
	}
	else  e->Cancel= true;


			 }
private: System::Void Accueil_Load_1(System::Object^  sender, System::EventArgs^  e) {
       

		 }
		 /****************************************************/
private: System::Void label3_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label5_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label4_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label2_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label6_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label7_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label8_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label10_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label9_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				
		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
              // this->Hide();    
			   MenuAppli^ menu= gcnew MenuAppli();
				 menu->ShowDialog();

		 }
private: System::Void button1_Click_1(System::Object^  sender, System::EventArgs^  e) {
if (MessageBox::Show("Voulez-vous quitter l'application?","Accueil",MessageBoxButtons::YesNo,MessageBoxIcon::Question)
					 == System::Windows::Forms::DialogResult::Yes)
	{Application::ExitThread();}
//	else  e->Cancel= true;
		 }

private: System::Void Propos_Click(System::Object^  sender, System::EventArgs^  e) {
			 Aide^ d1=gcnew Aide();
			 d1->ShowDialog();

		 }
};
}

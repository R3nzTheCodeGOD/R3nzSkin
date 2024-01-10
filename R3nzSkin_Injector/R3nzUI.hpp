#pragma once

#include "xorstr.hpp"

namespace R3nzSkinInjector {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace System::IO;

	static auto btnState{ false };
	static auto gameState{ false };
	static auto cheatState{ false };
	static auto clientState{ false };

	delegate String^ MyDelegate();
	ref class LambdaWrapper sealed
	{
	public:
		String^ InvokeLambda()
		{
			return lambdaFunction();
		}
	private:
		MyDelegate^ lambdaFunction;
	public:
		LambdaWrapper()
		{
			lambdaFunction = gcnew MyDelegate(this, &LambdaWrapper::LambdaMethod);
		}
		String^ LambdaMethod()
		{
			auto characters = gcnew String("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
			auto random = gcnew Random(static_cast<int>(DateTime::Now.Ticks));

			auto buffer = gcnew array<Char>(8);
			for (int i = 0; i < 8; i++)
			{
				buffer[i] = characters[random->Next(characters->Length)];
			}

			return gcnew String(buffer);
		}
	};

	public ref class R3nzUI sealed : public Form
	{
	public:
		R3nzUI()
		{
			InitializeComponent(); loadSettings(); renameProgram();
		}

		void updateScreen()
		{
			while (true) {
				if (clientState) {
					this->clientStatusLabel->Text = L"Found";
					this->clientStatusLabel->ForeColor = Color::FromArgb(255, 252, 220, 107);
				}
				else {
					this->clientStatusLabel->Text = L"Not Found";
					this->clientStatusLabel->ForeColor = Color::FromArgb(255, 245, 8, 83);
				}

				if (gameState) {
					this->gameStatusLabel->Text = L"Found";
					this->gameStatusLabel->ForeColor = Color::FromArgb(255, 252, 220, 107);
					if (cheatState) {
						this->dllStatusLabel->Text = L"Injected";
						this->dllStatusLabel->ForeColor = Color::FromArgb(255, 252, 220, 107);
					}
					else {
						this->dllStatusLabel->Text = L"Not Injected";
						this->dllStatusLabel->ForeColor = Color::FromArgb(255, 245, 8, 83);
					}
				}
				else {
					this->gameStatusLabel->Text = L"Not Found";
					this->gameStatusLabel->ForeColor = Color::FromArgb(255, 245, 8, 83);
					this->dllStatusLabel->Text = L"Not Injected";
					this->dllStatusLabel->ForeColor = Color::FromArgb(255, 245, 8, 83);
				}
				Thread::Sleep(1000);
			}
		}

		void saveSettings()
		{
			auto appDataPath = Environment::GetFolderPath(Environment::SpecialFolder::MyDocuments);
			auto settingsFolder = Path::Combine(appDataPath, L"R3nzSkin");
			auto settingsFilePath = Path::Combine(settingsFolder, L"R3nzSkinInjector");

			if (!Directory::Exists(settingsFolder)) {
				Directory::CreateDirectory(settingsFolder);
			}
			
			File::WriteAllText(settingsFilePath, Convert::ToString(this->toolstripmenuItem2->Checked));
		}

		void loadSettings()
		{
			auto appDataPath = Environment::GetFolderPath(Environment::SpecialFolder::MyDocuments);
			auto settingsFolder = Path::Combine(appDataPath, L"R3nzSkin");
			auto settingsFilePath = Path::Combine(settingsFolder, L"R3nzSkinInjector");

			if (!Directory::Exists(settingsFolder)) {
				Directory::CreateDirectory(settingsFolder);
			}

			if (File::Exists(settingsFilePath)) {
				this->toolstripmenuItem2->Checked = Boolean::Parse(File::ReadAllText(settingsFilePath));
			}
		}

		void renameProgram()
		{
			auto wrapper = gcnew LambdaWrapper();

			// Executable
			File::Move(Application::ExecutablePath, String::Format("{0}\\{1}.exe", Application::StartupPath, wrapper->InvokeLambda()));

			// Title
			this->Text = wrapper->InvokeLambda();
		}
	protected:
		~R3nzUI() { if (components) delete components; }
	private:
		Button^ startButton;
		Label^ injectorStatusLabel;
		Label^ dllStatusLabel;
		Label^ gameStatusLabel;
		Label^ clientStatusLabel;
		GroupBox^ injectorStatusGroupBox;
		GroupBox^ leagueClientStatusGroupBox;
		GroupBox^ leagueGameStatusGroupBox;
		GroupBox^ dllStatusGroupBox;
		LinkLabel^ copyrightLabel;
		System::ComponentModel::Container^ components;
		NotifyIcon^ notifyIcon;
		Windows::Forms::ContextMenu^ contextMenu;
		MenuItem^ menuItem;
		MenuItem^ menuItem2;
		MenuStrip^ menuStrip;
		ToolStripMenuItem^ toolstripmenuItem;
		ToolStripMenuItem^ toolstripmenuItem2;
#pragma region Windows Form Designer generated code
		   void InitializeComponent()
		   {
			   auto resources = gcnew ComponentResourceManager(R3nzUI::typeid);
			   this->startButton = gcnew Button();
			   this->injectorStatusLabel = gcnew Label();
			   this->dllStatusLabel = gcnew Label();
			   this->gameStatusLabel = gcnew Label();
			   this->injectorStatusGroupBox = gcnew GroupBox();
			   this->leagueClientStatusGroupBox = gcnew GroupBox();
			   this->clientStatusLabel = gcnew Label();
			   this->leagueGameStatusGroupBox = gcnew GroupBox();
			   this->dllStatusGroupBox = gcnew GroupBox();
			   this->copyrightLabel = gcnew LinkLabel();
			   this->notifyIcon = gcnew NotifyIcon();
			   this->contextMenu = gcnew Windows::Forms::ContextMenu();
			   this->menuItem = gcnew MenuItem();
			   this->menuItem2 = gcnew MenuItem();
			   this->menuStrip = gcnew MenuStrip();
			   this->toolstripmenuItem = gcnew ToolStripMenuItem();
			   this->toolstripmenuItem2 = gcnew ToolStripMenuItem();
			   this->injectorStatusGroupBox->SuspendLayout();
			   this->leagueClientStatusGroupBox->SuspendLayout();
			   this->leagueGameStatusGroupBox->SuspendLayout();
			   this->dllStatusGroupBox->SuspendLayout();
			   this->SuspendLayout();
			   // 
			   // startButton
			   // 
			   this->startButton->BackColor = Color::FromArgb(245, 8, 83);
			   this->startButton->Cursor = Cursors::Hand;
			   this->startButton->FlatStyle = FlatStyle::Flat;
			   this->startButton->Font = gcnew Drawing::Font(L"Arial", 12, FontStyle::Bold, GraphicsUnit::Point, static_cast<Byte>(162));
			   this->startButton->Location = Point(12, 28);
			   this->startButton->Name = L"startButton";
			   this->startButton->Size = Drawing::Size(250, 50);
			   this->startButton->TabIndex = 0;
			   this->startButton->Text = L"Start";
			   this->startButton->UseVisualStyleBackColor = false;
			   this->startButton->Click += gcnew EventHandler(this, &R3nzUI::startButton_Click);
			   // 
			   // injectorStatusLabel
			   // 
			   this->injectorStatusLabel->AutoSize = true;
			   this->injectorStatusLabel->FlatStyle = FlatStyle::Flat;
			   this->injectorStatusLabel->Font = gcnew Drawing::Font(L"Arial", 11.25F, FontStyle::Bold, GraphicsUnit::Point, static_cast<Byte>(162));
			   this->injectorStatusLabel->ForeColor = Color::FromArgb(245, 8, 83);
			   this->injectorStatusLabel->Location = Point(6, 16);
			   this->injectorStatusLabel->Name = L"injectorStatusLabel";
			   this->injectorStatusLabel->Size = Drawing::Size(68, 18);
			   this->injectorStatusLabel->TabIndex = 1;
			   this->injectorStatusLabel->Text = L"Stopped";
			   // 
			   // dllStatusLabel
			   // 
			   this->dllStatusLabel->AutoSize = true;
			   this->dllStatusLabel->FlatStyle = FlatStyle::Flat;
			   this->dllStatusLabel->Font = gcnew Drawing::Font(L"Arial", 11.25F, FontStyle::Bold, GraphicsUnit::Point, static_cast<Byte>(162));
			   this->dllStatusLabel->ForeColor = Color::FromArgb(245, 8, 83);
			   this->dllStatusLabel->Location = Point(6, 16);
			   this->dllStatusLabel->Name = L"dllStatusLabel";
			   this->dllStatusLabel->Size = Drawing::Size(94, 18);
			   this->dllStatusLabel->TabIndex = 2;
			   this->dllStatusLabel->Text = L"Not Injected";
			   // 
			   // gameStatusLabel
			   // 
			   this->gameStatusLabel->AutoSize = true;
			   this->gameStatusLabel->FlatStyle = FlatStyle::Flat;
			   this->gameStatusLabel->Font = gcnew Drawing::Font(L"Arial", 11.25F, FontStyle::Bold, GraphicsUnit::Point, static_cast<Byte>(162));
			   this->gameStatusLabel->ForeColor = Color::FromArgb(245, 8, 83);
			   this->gameStatusLabel->Location = Point(6, 16);
			   this->gameStatusLabel->Name = L"gameStatusLabel";
			   this->gameStatusLabel->Size = Drawing::Size(82, 18);
			   this->gameStatusLabel->TabIndex = 3;
			   this->gameStatusLabel->Text = L"Not Found";
			   // 
			   // gameStatusLabel
			   // 
			   this->injectorStatusGroupBox->Controls->Add(this->injectorStatusLabel);
			   this->injectorStatusGroupBox->FlatStyle = FlatStyle::Flat;
			   this->injectorStatusGroupBox->Font = gcnew Drawing::Font(L"Arial", 6.75F, FontStyle::Bold, GraphicsUnit::Point, static_cast<Byte>(162));
			   this->injectorStatusGroupBox->ForeColor = Color::White;
			   this->injectorStatusGroupBox->Location = Point(12, 79);
			   this->injectorStatusGroupBox->Name = L"gameStatusLabel";
			   this->injectorStatusGroupBox->Size = Drawing::Size(250, 45);
			   this->injectorStatusGroupBox->TabIndex = 5;
			   this->injectorStatusGroupBox->TabStop = false;
			   this->injectorStatusGroupBox->Text = L"Injector Status";
			   // 
			   // leagueClientStatusGroupBox
			   // 
			   this->leagueClientStatusGroupBox->Controls->Add(this->clientStatusLabel);
			   this->leagueClientStatusGroupBox->FlatStyle = FlatStyle::Flat;
			   this->leagueClientStatusGroupBox->Font = gcnew Drawing::Font(L"Arial", 6.75F, FontStyle::Bold, GraphicsUnit::Point, static_cast<Byte>(162));
			   this->leagueClientStatusGroupBox->ForeColor = Color::White;
			   this->leagueClientStatusGroupBox->Location = Point(12, 130);
			   this->leagueClientStatusGroupBox->Name = L"leagueClientStatusGroupBox";
			   this->leagueClientStatusGroupBox->Size = Drawing::Size(250, 45);
			   this->leagueClientStatusGroupBox->TabIndex = 7;
			   this->leagueClientStatusGroupBox->TabStop = false;
			   this->leagueClientStatusGroupBox->Text = L"LeagueClient Status";
			   // 
		   	   // leagueGameStatusGroupBox
		   	   // 
			   this->leagueGameStatusGroupBox->Controls->Add(this->gameStatusLabel);
			   this->leagueGameStatusGroupBox->FlatStyle = FlatStyle::Flat;
			   this->leagueGameStatusGroupBox->Font = gcnew Drawing::Font(L"Arial", 6.75F, FontStyle::Bold, GraphicsUnit::Point, static_cast<Byte>(162));
			   this->leagueGameStatusGroupBox->ForeColor = Color::White;
			   this->leagueGameStatusGroupBox->Location = Point(12, 181);
			   this->leagueGameStatusGroupBox->Name = L"leagueGameStatusGroupBox";
			   this->leagueGameStatusGroupBox->Size = Drawing::Size(250, 45);
			   this->leagueGameStatusGroupBox->TabIndex = 8;
			   this->leagueGameStatusGroupBox->TabStop = false;
			   this->leagueGameStatusGroupBox->Text = L"LeagueGame Status";
			   // 
		   	   // dllStatusGroupBox
		   	   // 
			   this->dllStatusGroupBox->Controls->Add(this->dllStatusLabel);
			   this->dllStatusGroupBox->FlatStyle = FlatStyle::Flat;
			   this->dllStatusGroupBox->Font = gcnew Drawing::Font(L"Arial", 6.75F, FontStyle::Bold, GraphicsUnit::Point, static_cast<Byte>(162));
			   this->dllStatusGroupBox->ForeColor = Color::White;
			   this->dllStatusGroupBox->Location = Point(12, 232);
			   this->dllStatusGroupBox->Name = L"dllStatusGroupBox";
			   this->dllStatusGroupBox->Size = Drawing::Size(250, 45);
			   this->dllStatusGroupBox->TabIndex = 9;
			   this->dllStatusGroupBox->TabStop = false;
			   this->dllStatusGroupBox->Text = L"R3nzSkin Status";
			   // 
			   // clientStatusLabel
			   // 
			   this->clientStatusLabel->AutoSize = true;
			   this->clientStatusLabel->FlatStyle = FlatStyle::Flat;
			   this->clientStatusLabel->Font = gcnew Drawing::Font(L"Arial", 11.25F, FontStyle::Bold, GraphicsUnit::Point, static_cast<Byte>(162));
			   this->clientStatusLabel->ForeColor = Color::FromArgb(245, 8, 83);
			   this->clientStatusLabel->Location = Point(6, 16);
			   this->clientStatusLabel->Name = L"clientStatusLabel";
			   this->clientStatusLabel->Size = Drawing::Size(82, 18);
			   this->clientStatusLabel->TabIndex = 0;
			   this->clientStatusLabel->Text = L"Not Found";
			   // 
			   // copyrightLabel
			   // 
			   this->copyrightLabel->AutoSize = true;
			   this->copyrightLabel->Cursor = Cursors::Hand;
			   this->copyrightLabel->LinkBehavior = LinkBehavior::NeverUnderline;
			   this->copyrightLabel->LinkColor = Color::Silver;
			   this->copyrightLabel->Location = Point(20, 284);
			   this->copyrightLabel->Name = L"copyrightLabel";
			   this->copyrightLabel->Size = Drawing::Size(207, 14);
			   this->copyrightLabel->TabIndex = 11;
			   this->copyrightLabel->TabStop = true;
			   this->copyrightLabel->Text = L"Copyright (c) 2021-2024 R3nzTheCodeGOD";
			   this->copyrightLabel->TextAlign = ContentAlignment::MiddleCenter;
			   this->copyrightLabel->LinkClicked += gcnew LinkLabelLinkClickedEventHandler(this, &R3nzUI::copyrightLabel_LinkClicked);
			   //
			   // contextMenu
			   //
			   this->contextMenu->MenuItems->AddRange(gcnew array<MenuItem^> { this->menuItem, this->menuItem2 });
			   //
			   // menuItem
			   //
			   this->menuItem2->Index = 0;
			   this->menuItem2->Text = L"Start";
			   this->menuItem2->Click += gcnew EventHandler(this, &R3nzUI::menuItem2_OnClick);
			   this->menuItem->Index = 1;
			   this->menuItem->Text = L"Exit";
			   this->menuItem->Click += gcnew EventHandler(this, &R3nzUI::menuItem_OnClick);
			   //
			   // notifyIcon
			   //
			   this->notifyIcon->Text = L"R3nzSkin";
			   this->notifyIcon->Icon = cli::safe_cast<Drawing::Icon^>(resources->GetObject(L"$this.Icon"));
			   this->notifyIcon->Visible = false;
			   this->notifyIcon->ContextMenu = this->contextMenu;
			   this->notifyIcon->MouseDoubleClick += gcnew MouseEventHandler(this, &R3nzUI::notifyIcon_MouseDoubleClick);
			   //
			   // menuStrip
			   //
			   this->toolstripmenuItem->Text = L"Preferences";
			   this->toolstripmenuItem2->Text = L"Hide to tray";
			   this->toolstripmenuItem2->Click += gcnew EventHandler(this, &R3nzUI::toolstripmenuItem2_OnClick);
			   this->toolstripmenuItem->DropDownItems->Add(this->toolstripmenuItem2);
			   this->menuStrip->Items->Add(this->toolstripmenuItem);
			   // 
			   // R3nzUI
			   // 
			   this->AutoScaleDimensions = SizeF(7, 14);
			   this->AutoScaleMode = Windows::Forms::AutoScaleMode::Font;
			   this->BackColor = Color::FromArgb(32, 30, 30);
			   this->ClientSize = Drawing::Size(273, 307);
			   this->Controls->Add(this->menuStrip);
		   	   this->Controls->Add(this->copyrightLabel);
			   this->Controls->Add(this->dllStatusGroupBox);
			   this->Controls->Add(this->leagueGameStatusGroupBox);
			   this->Controls->Add(this->leagueClientStatusGroupBox);
			   this->Controls->Add(this->injectorStatusGroupBox);
			   this->Controls->Add(this->startButton);
			   this->Cursor = Cursors::Arrow;
			   this->Font = gcnew Drawing::Font(L"Arial", 8.25F, FontStyle::Bold, GraphicsUnit::Point, static_cast<Byte>(162));
			   this->FormBorderStyle = Windows::Forms::FormBorderStyle::Fixed3D;
			   this->Icon = cli::safe_cast<Drawing::Icon^>(resources->GetObject(L"$this.Icon"));
			   this->MaximizeBox = false;
			   this->Name = L"R3nzUI";
			   this->RightToLeft = Windows::Forms::RightToLeft::No;
			   this->Text = L"";
			   this->Load += gcnew EventHandler(this, &R3nzUI::R3nzUI_Load);
			   this->Resize += gcnew EventHandler(this, &R3nzUI::R3nzUI_Resize);
			   this->injectorStatusGroupBox->ResumeLayout(false);
			   this->injectorStatusGroupBox->PerformLayout();
			   this->leagueClientStatusGroupBox->ResumeLayout(false);
			   this->leagueClientStatusGroupBox->PerformLayout();
			   this->leagueGameStatusGroupBox->ResumeLayout(false);
			   this->leagueGameStatusGroupBox->PerformLayout();
			   this->dllStatusGroupBox->ResumeLayout(false);
			   this->dllStatusGroupBox->PerformLayout();
			   this->menuStrip->ResumeLayout(false);
			   this->menuStrip->PerformLayout();
			   this->ResumeLayout(false);
			   this->PerformLayout();
		   }
#pragma endregion
	private:
		Void R3nzUI_Load(Object^ sender, EventArgs^ e) 
		{
			AppDomain::CurrentDomain->UnhandledException += gcnew UnhandledExceptionEventHandler(this, &R3nzUI::R3nzUI_ExceptionHandler);
		}
	private:
		Void R3nzUI_ExceptionHandler(Object^ sender, UnhandledExceptionEventArgs^ e)
		{
			const auto exception = dynamic_cast<Exception^>(e->ExceptionObject)->Message;
			MessageBox::Show(exception);
		}
	private:
		Void startButton_Click(Object^ sender, EventArgs^ e)
		{
			btnState = !btnState;
			if (btnState) 
			{
				this->startButton->BackColor = Color::FromArgb(255, 252, 220, 107);
				this->injectorStatusLabel->ForeColor = Color::FromArgb(255, 252, 220, 107);
				this->startButton->Text = L"Stop";
				this->injectorStatusLabel->Text = L"Working";
				this->menuItem2->Text = L"Stop";
			}
			else 
			{
				this->startButton->BackColor = Color::FromArgb(255, 245, 8, 83);
				this->injectorStatusLabel->ForeColor = Color::FromArgb(255, 245, 8, 83);
				this->startButton->Text = L"Start";
				this->injectorStatusLabel->Text = L"Stopped";
				this->menuItem2->Text = L"Start";
			}
		}
	private:
		Void copyrightLabel_LinkClicked(Object^ sender, LinkLabelLinkClickedEventArgs^ e)
		{
			Diagnostics::Process::Start(L"https://github.com/R3nzTheCodeGOD/R3nzSkin");
		}
	private:
		Void R3nzUI_Resize(Object^ sender, EventArgs^ e)
		{
			if (this->WindowState == FormWindowState::Minimized) 
			{
				if (this->toolstripmenuItem2->Checked) 
				{
					this->Hide();
					this->notifyIcon->Visible = true;
				}
			}
		}
	private:
		Void notifyIcon_MouseDoubleClick(Object^ sender, MouseEventArgs^ e)
		{
			if (this->toolstripmenuItem2->Checked)
			{
				this->Show();
				this->WindowState = FormWindowState::Normal;
				this->notifyIcon->Visible = false;
			}
		}
	private:
		Void menuItem_OnClick(Object^ sender, EventArgs^ e)
		{
			this->Close();
		}
	private:
		Void menuItem2_OnClick(Object^ sender, EventArgs^ e)
		{
			this->startButton_Click(nullptr, nullptr);
		}
	private:
		Void toolstripmenuItem2_OnClick(Object^ sender, EventArgs^ e)
		{
			this->toolstripmenuItem2->Checked = !this->toolstripmenuItem2->Checked;
			this->saveSettings();
		}
	};
}

#pragma once

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
	ref class LambdaWrapper
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

	public ref class R3nzUI : public System::Windows::Forms::Form {
	public:
		R3nzUI(void) { InitializeComponent(); loadSettings(); renameProgram(); }
	public:
		void updateScreen() {
			while (true) {
				if (clientState) {
					this->label4->Text = L"Found";
					this->label4->ForeColor = System::Drawing::Color::FromArgb(255, 252, 220, 107);
				}
				else {
					this->label4->Text = L"Not Found";
					this->label4->ForeColor = System::Drawing::Color::FromArgb(255, 245, 8, 83);
				}

				if (gameState) {
					this->label3->Text = L"Found";
					this->label3->ForeColor = System::Drawing::Color::FromArgb(255, 252, 220, 107);
					if (cheatState) {
						this->label2->Text = L"Injected";
						this->label2->ForeColor = System::Drawing::Color::FromArgb(255, 252, 220, 107);
					}
					else {
						this->label2->Text = L"Not Injected";
						this->label2->ForeColor = System::Drawing::Color::FromArgb(255, 245, 8, 83);
					}
				}
				else {
					this->label3->Text = L"Not Found";
					this->label3->ForeColor = System::Drawing::Color::FromArgb(255, 245, 8, 83);
					this->label2->Text = L"Not Injected";
					this->label2->ForeColor = System::Drawing::Color::FromArgb(255, 245, 8, 83);
				}
				Thread::Sleep(1000);
			}
		}
		void saveSettings() {
			auto appDataPath = Environment::GetFolderPath(Environment::SpecialFolder::MyDocuments);
			auto settingsFolder = Path::Combine(appDataPath, L"R3nzSkin");
			auto settingsFilePath = Path::Combine(settingsFolder, L"R3nzSkinInjector");

			if (!Directory::Exists(settingsFolder)) {
				Directory::CreateDirectory(settingsFolder);
			}
			
			File::WriteAllText(settingsFilePath, System::Convert::ToString(this->toolstripmenuItem2->Checked));
		}
		void loadSettings() {
			auto appDataPath = Environment::GetFolderPath(Environment::SpecialFolder::MyDocuments);
			auto settingsFolder = Path::Combine(appDataPath, L"R3nzSkin");
			auto settingsFilePath = Path::Combine(settingsFolder, L"R3nzSkinInjector");

			if (!Directory::Exists(settingsFolder)) {
				Directory::CreateDirectory(settingsFolder);
			}

			if (File::Exists(settingsFilePath)) {
				this->toolstripmenuItem2->Checked = System::Boolean::Parse(File::ReadAllText(settingsFilePath));
			}
		}
		void renameProgram()
		{
			auto wrapper = gcnew LambdaWrapper();

			// Executable
			System::IO::File::Move(Application::ExecutablePath, String::Format("{0}\\{1}.exe", Application::StartupPath, wrapper->InvokeLambda()));

			// Title
			this->Text = wrapper->InvokeLambda();
		}
	protected:
		~R3nzUI() { if (components) delete components; }
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::GroupBox^ groupBox1;
	private: System::Windows::Forms::GroupBox^ groupBox2;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::GroupBox^ groupBox3;
	private: System::Windows::Forms::GroupBox^ groupBox4;
	private: System::Windows::Forms::LinkLabel^ linkLabel1;
	private: System::ComponentModel::Container^ components;
	private: System::Windows::Forms::NotifyIcon^ notifyIcon;
	private: System::Windows::Forms::ContextMenu^ contextMenu;
	private: System::Windows::Forms::MenuItem^ menuItem;
	private: System::Windows::Forms::MenuItem^ menuItem2;
	private: System::Windows::Forms::MenuStrip^ menuStrip;
	private: System::Windows::Forms::ToolStripMenuItem^ toolstripmenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ toolstripmenuItem2;
#pragma region Windows Form Designer generated code
		   void InitializeComponent(void)
		   {
			   auto resources = (gcnew System::ComponentModel::ComponentResourceManager(R3nzUI::typeid));
			   this->button1 = (gcnew System::Windows::Forms::Button());
			   this->label1 = (gcnew System::Windows::Forms::Label());
			   this->label2 = (gcnew System::Windows::Forms::Label());
			   this->label3 = (gcnew System::Windows::Forms::Label());
			   this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			   this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			   this->label4 = (gcnew System::Windows::Forms::Label());
			   this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			   this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			   this->linkLabel1 = (gcnew System::Windows::Forms::LinkLabel());
			   this->notifyIcon = (gcnew System::Windows::Forms::NotifyIcon());
			   this->contextMenu = (gcnew System::Windows::Forms::ContextMenu());
			   this->menuItem = (gcnew System::Windows::Forms::MenuItem());
			   this->menuItem2 = (gcnew System::Windows::Forms::MenuItem());
			   this->menuStrip = (gcnew System::Windows::Forms::MenuStrip());
			   this->toolstripmenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->toolstripmenuItem2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->groupBox1->SuspendLayout();
			   this->groupBox2->SuspendLayout();
			   this->groupBox3->SuspendLayout();
			   this->groupBox4->SuspendLayout();
			   this->SuspendLayout();
			   // 
			   // button1
			   // 
			   this->button1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(245)), static_cast<System::Int32>(static_cast<System::Byte>(8)), static_cast<System::Int32>(static_cast<System::Byte>(83)));
			   this->button1->Cursor = System::Windows::Forms::Cursors::Hand;
			   this->button1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			   this->button1->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(162)));
			   this->button1->Location = System::Drawing::Point(12, 28);
			   this->button1->Name = L"button1";
			   this->button1->Size = System::Drawing::Size(250, 50);
			   this->button1->TabIndex = 0;
			   this->button1->Text = L"Start";
			   this->button1->UseVisualStyleBackColor = false;
			   this->button1->Click += gcnew System::EventHandler(this, &R3nzUI::button1_Click);
			   // 
			   // label1
			   // 
			   this->label1->AutoSize = true;
			   this->label1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			   this->label1->Font = (gcnew System::Drawing::Font(L"Arial", 11.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(162)));
			   this->label1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(245)), static_cast<System::Int32>(static_cast<System::Byte>(8)), static_cast<System::Int32>(static_cast<System::Byte>(83)));
			   this->label1->Location = System::Drawing::Point(6, 16);
			   this->label1->Name = L"label1";
			   this->label1->Size = System::Drawing::Size(68, 18);
			   this->label1->TabIndex = 1;
			   this->label1->Text = L"Stopped";
			   // 
			   // label2
			   // 
			   this->label2->AutoSize = true;
			   this->label2->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			   this->label2->Font = (gcnew System::Drawing::Font(L"Arial", 11.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(162)));
			   this->label2->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(245)), static_cast<System::Int32>(static_cast<System::Byte>(8)), static_cast<System::Int32>(static_cast<System::Byte>(83)));
			   this->label2->Location = System::Drawing::Point(6, 16);
			   this->label2->Name = L"label2";
			   this->label2->Size = System::Drawing::Size(94, 18);
			   this->label2->TabIndex = 2;
			   this->label2->Text = L"Not Injected";
			   // 
			   // label3
			   // 
			   this->label3->AutoSize = true;
			   this->label3->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			   this->label3->Font = (gcnew System::Drawing::Font(L"Arial", 11.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(162)));
			   this->label3->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(245)), static_cast<System::Int32>(static_cast<System::Byte>(8)), static_cast<System::Int32>(static_cast<System::Byte>(83)));
			   this->label3->Location = System::Drawing::Point(6, 16);
			   this->label3->Name = L"label3";
			   this->label3->Size = System::Drawing::Size(82, 18);
			   this->label3->TabIndex = 3;
			   this->label3->Text = L"Not Found";
			   // 
			   // groupBox1
			   // 
			   this->groupBox1->Controls->Add(this->label1);
			   this->groupBox1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			   this->groupBox1->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(162)));
			   this->groupBox1->ForeColor = System::Drawing::Color::White;
			   this->groupBox1->Location = System::Drawing::Point(12, 79);
			   this->groupBox1->Name = L"groupBox1";
			   this->groupBox1->Size = System::Drawing::Size(250, 45);
			   this->groupBox1->TabIndex = 5;
			   this->groupBox1->TabStop = false;
			   this->groupBox1->Text = L"Injector Status";
			   // 
			   // groupBox2
			   // 
			   this->groupBox2->Controls->Add(this->label4);
			   this->groupBox2->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			   this->groupBox2->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(162)));
			   this->groupBox2->ForeColor = System::Drawing::Color::White;
			   this->groupBox2->Location = System::Drawing::Point(12, 130);
			   this->groupBox2->Name = L"groupBox2";
			   this->groupBox2->Size = System::Drawing::Size(250, 45);
			   this->groupBox2->TabIndex = 7;
			   this->groupBox2->TabStop = false;
			   this->groupBox2->Text = L"LeagueClient Status";
			   // 
			   // label4
			   // 
			   this->label4->AutoSize = true;
			   this->label4->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			   this->label4->Font = (gcnew System::Drawing::Font(L"Arial", 11.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(162)));
			   this->label4->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(245)), static_cast<System::Int32>(static_cast<System::Byte>(8)), static_cast<System::Int32>(static_cast<System::Byte>(83)));
			   this->label4->Location = System::Drawing::Point(6, 16);
			   this->label4->Name = L"label4";
			   this->label4->Size = System::Drawing::Size(82, 18);
			   this->label4->TabIndex = 0;
			   this->label4->Text = L"Not Found";
			   // 
			   // groupBox3
			   // 
			   this->groupBox3->Controls->Add(this->label3);
			   this->groupBox3->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			   this->groupBox3->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(162)));
			   this->groupBox3->ForeColor = System::Drawing::Color::White;
			   this->groupBox3->Location = System::Drawing::Point(12, 181);
			   this->groupBox3->Name = L"groupBox3";
			   this->groupBox3->Size = System::Drawing::Size(250, 45);
			   this->groupBox3->TabIndex = 8;
			   this->groupBox3->TabStop = false;
			   this->groupBox3->Text = L"LeagueGame Status";
			   // 
			   // groupBox4
			   // 
			   this->groupBox4->Controls->Add(this->label2);
			   this->groupBox4->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			   this->groupBox4->Font = (gcnew System::Drawing::Font(L"Arial", 6.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(162)));
			   this->groupBox4->ForeColor = System::Drawing::Color::White;
			   this->groupBox4->Location = System::Drawing::Point(12, 232);
			   this->groupBox4->Name = L"groupBox4";
			   this->groupBox4->Size = System::Drawing::Size(250, 45);
			   this->groupBox4->TabIndex = 9;
			   this->groupBox4->TabStop = false;
			   this->groupBox4->Text = L"R3nzSkin Status";
			   // 
			   // linkLabel1
			   // 
			   this->linkLabel1->AutoSize = true;
			   this->linkLabel1->Cursor = System::Windows::Forms::Cursors::Hand;
			   this->linkLabel1->LinkBehavior = System::Windows::Forms::LinkBehavior::NeverUnderline;
			   this->linkLabel1->LinkColor = System::Drawing::Color::Silver;
			   this->linkLabel1->Location = System::Drawing::Point(20, 284);
			   this->linkLabel1->Name = L"linkLabel1";
			   this->linkLabel1->Size = System::Drawing::Size(207, 14);
			   this->linkLabel1->TabIndex = 11;
			   this->linkLabel1->TabStop = true;
			   this->linkLabel1->Text = L"Copyright (c) 2021-2023 R3nzTheCodeGOD";
			   this->linkLabel1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			   this->linkLabel1->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &R3nzUI::linkLabel1_LinkClicked);
			   //
			   // contextMenu
			   //
			   this->contextMenu->MenuItems->AddRange(gcnew array<MenuItem^> { this->menuItem, this->menuItem2 });
			   //
			   // menuItem
			   //
			   this->menuItem2->Index = 0;
			   this->menuItem2->Text = L"Start";
			   this->menuItem2->Click += gcnew System::EventHandler(this, &R3nzUI::menuItem2_OnClick);
			   this->menuItem->Index = 1;
			   this->menuItem->Text = L"Exit";
			   this->menuItem->Click += gcnew System::EventHandler(this, &R3nzUI::menuItem_OnClick);
			   //
			   // notifyIcon
			   //
			   this->notifyIcon->Text = L"R3nzSkin";
			   this->notifyIcon->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			   this->notifyIcon->Visible = false;
			   this->notifyIcon->ContextMenu = this->contextMenu;
			   this->notifyIcon->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &R3nzUI::notifyIcon_MouseDoubleClick);
			   //
			   // menuStrip
			   //
			   this->toolstripmenuItem->Text = L"Preferences";
			   this->toolstripmenuItem2->Text = L"Hide to tray";
			   this->toolstripmenuItem2->Click += gcnew System::EventHandler(this, &R3nzUI::toolstripmenuItem2_OnClick);
			   this->toolstripmenuItem->DropDownItems->Add(this->toolstripmenuItem2);
			   this->menuStrip->Items->Add(this->toolstripmenuItem);
			   // 
			   // R3nzUI
			   // 
			   this->AutoScaleDimensions = System::Drawing::SizeF(7, 14);
			   this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			   this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(32)), static_cast<System::Int32>(static_cast<System::Byte>(30)), static_cast<System::Int32>(static_cast<System::Byte>(30)));
			   this->ClientSize = System::Drawing::Size(273, 307);
			   this->Controls->Add(this->menuStrip);
		   	   this->Controls->Add(this->linkLabel1);
			   this->Controls->Add(this->groupBox4);
			   this->Controls->Add(this->groupBox3);
			   this->Controls->Add(this->groupBox2);
			   this->Controls->Add(this->groupBox1);
			   this->Controls->Add(this->button1);
			   this->Cursor = System::Windows::Forms::Cursors::Arrow;
			   this->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(162)));
			   this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
			   this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			   this->MaximizeBox = false;
			   this->Name = L"R3nzUI";
			   this->RightToLeft = System::Windows::Forms::RightToLeft::No;
			   this->Text = L"";
			   this->Load += gcnew System::EventHandler(this, &R3nzUI::R3nzUI_Load);
			   this->Resize += gcnew System::EventHandler(this, &R3nzUI::R3nzUI_Resize);
			   this->groupBox1->ResumeLayout(false);
			   this->groupBox1->PerformLayout();
			   this->groupBox2->ResumeLayout(false);
			   this->groupBox2->PerformLayout();
			   this->groupBox3->ResumeLayout(false);
			   this->groupBox3->PerformLayout();
			   this->groupBox4->ResumeLayout(false);
			   this->groupBox4->PerformLayout();
			   this->menuStrip->ResumeLayout(false);
			   this->menuStrip->PerformLayout();
			   this->ResumeLayout(false);
			   this->PerformLayout();
		   }
#pragma endregion
	private:
		System::Void R3nzUI_Load(System::Object^ sender, System::EventArgs^ e) 
		{
			System::AppDomain::CurrentDomain->UnhandledException += gcnew System::UnhandledExceptionEventHandler(this, &R3nzUI::R3nzUI_ExceptionHandler);
		}
	private:
		System::Void R3nzUI_ExceptionHandler(System::Object^ sender, System::UnhandledExceptionEventArgs^ e)
		{
			const auto exception = dynamic_cast<System::Exception^>(e->ExceptionObject)->Message;
			MessageBox::Show(exception);
		}
	private:
		System::Void button1_Click(System::Object^ sender, System::EventArgs^ e)
		{
			btnState = !btnState;
			if (btnState) {
				this->button1->BackColor = System::Drawing::Color::FromArgb(255, 252, 220, 107);
				this->label1->ForeColor = System::Drawing::Color::FromArgb(255, 252, 220, 107);
				this->button1->Text = L"Stop";
				this->label1->Text = L"Working";
				this->menuItem2->Text = L"Stop";
			} else {
				this->button1->BackColor = System::Drawing::Color::FromArgb(255, 245, 8, 83);
				this->label1->ForeColor = System::Drawing::Color::FromArgb(255, 245, 8, 83);
				this->button1->Text = L"Start";
				this->label1->Text = L"Stopped";
				this->menuItem2->Text = L"Start";
			}
		}
	private:
		System::Void linkLabel1_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e)
		{
			System::Diagnostics::Process::Start(L"https://github.com/R3nzTheCodeGOD/R3nzSkin");
		}
	private:
		System::Void R3nzUI_Resize(System::Object^ sender, System::EventArgs^ e)
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
		System::Void notifyIcon_MouseDoubleClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
		{
			if (this->toolstripmenuItem2->Checked)
			{
				this->Show();
				this->WindowState = FormWindowState::Normal;
				this->notifyIcon->Visible = false;
			}
		}
	private:
		System::Void menuItem_OnClick(System::Object^ sender, System::EventArgs^ e)
		{
			this->Close();
		}
	private:
		System::Void menuItem2_OnClick(System::Object^ sender, System::EventArgs^ e)
		{
			this->button1_Click(nullptr, nullptr);
		}
	private:
		System::Void toolstripmenuItem2_OnClick(System::Object^ sender, System::EventArgs^ e)
		{
			this->toolstripmenuItem2->Checked = !this->toolstripmenuItem2->Checked;
			this->saveSettings();
		}
	};
}

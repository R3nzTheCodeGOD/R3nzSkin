#include <Windows.h>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <regex>
#include <msclr/marshal_cppstd.h>

#include "R3nzUI.hpp"
#include "Injector.hpp"
#include "lazy_importer.hpp"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Threading;
using namespace System::Globalization;
using namespace System::Net;

void auto_update()
{
	WebClient^ client = gcnew WebClient();
	client->Headers->Add(L"User-Agent", L"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko/20100101 Firefox/115.0");

	try
	{
		std::string json = msclr::interop::marshal_as<std::string>(client->DownloadString(L"https://api.github.com/repos/R3nzTheCodeGOD/R3nzSkin/releases/latest"));
		std::regex tagnameRegex("\"tag_name\"\\s*:\\s*\"([^\"]+)");
		std::regex urlRegex("\"browser_download_url\"\\s*:\\s*\"([^\"]+)");
		std::regex dateRegex("\"created_at\"\\s*:\\s*\"([^\"]+)");

		std::smatch tagnameMatch, urlMatch, dateMatch;
		if (std::regex_search(json, tagnameMatch, tagnameRegex))
		{
			auto version = gcnew String(tagnameMatch[1].str().c_str());
			if (std::regex_search(json, dateMatch, dateRegex))
			{
				auto date_of_new_release = DateTime::ParseExact(gcnew String(dateMatch[1].str().c_str()), L"yyyy-MM-ddTHH:mm:ssZ", CultureInfo::InvariantCulture).ToString(L"dd.MM.yyyy");
				auto date_of_current_release = System::IO::File::GetLastWriteTime(L"R3nzSkin.dll").ToString(L"dd.MM.yyyy");
				if (date_of_current_release != date_of_new_release)
				{
					auto result = LI_FN(MessageBoxW)(nullptr, L"New version is available on GitHub\nWould you like to download it now?", L"R3nzSkin", MB_YESNO | MB_ICONINFORMATION);
					if (result == IDYES)
					{
						if (std::regex_search(json, urlMatch, urlRegex))
						{
							auto url = gcnew String(urlMatch[1].str().c_str());
							client->DownloadFile(url, String::Format(L"R3nzSkin_{0}.zip", version));
							Environment::Exit(0);
						}
					}
				}
			}
		}
	}
	catch (Exception^ e)
	{
		MessageBox::Show(e->Message, L"R3nzSkin", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
}

int main([[maybe_unused]] array<String^>^ args)
{
	auto_update();
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	Injector::renameExe();
	
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	R3nzSkinInjector::R3nzUI form;

	auto thread{ std::thread(Injector::run) };
	auto screenThread{ gcnew Thread(gcnew ThreadStart(%form, &R3nzSkinInjector::R3nzUI::updateScreen)) };
	screenThread->Start();

	Application::Run(%form);

	thread.detach();
	screenThread->Abort();

	return EXIT_SUCCESS;
}

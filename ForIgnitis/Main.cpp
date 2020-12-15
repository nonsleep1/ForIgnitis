#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>
#include <string>

#include <fcntl.h>
#include <io.h>
#include <sstream>
#include <ctime>


#include <Poco/Zip/Zip.h>
#include <Poco/Path.h>
#include <Poco/Zip/Compress.h>
#include <Poco/File.h>

#include <locale>
 



bool replace(std::string& str, const std::string& from, const std::string& to)
{
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}
std::string ReturnFileNameWithDate(int option)
{
	try
	{
		std::string date = "";
		time_t now = time(0);
		struct tm newtime;
		struct tm newtimeminusonemonth;
		

		localtime_s(&newtime, &now);

		std::cout << "Year:" << 1900 + newtime.tm_year << std::endl;
		std::cout << "Month: " << 1 + newtime.tm_mon << std::endl;
		std::cout << "Day: " << newtime.tm_mday << std::endl;
		std::cout << "Time: " << 1 + newtime.tm_hour << ":";
		std::cout << 1 + newtime.tm_min << ":";
		std::cout << 1 + newtime.tm_sec << std::endl;

		std::cout << "TV_" << 1900 + newtime.tm_year << newtime.tm_mon << ".txt" << std::endl;

		char mbstr[100];
		
		if (std::strftime(mbstr, sizeof(mbstr), "%y%m", &newtime)) {
			std::cout <<" MBSTR: " <<mbstr << '\n';
			newtimeminusonemonth = newtime;
		}
		switch (option)
		{
		default:
			std::cout << "Nepasirinktas option\n";
			return "Error";
			break;
			
		case 1:
			newtimeminusonemonth.tm_mon = newtimeminusonemonth.tm_mon - 1;
			date = "TV_" + std::string(mbstr) + ".txt";

			return date;
		case 0:
			date = "CG_" + std::string(mbstr) + ".zip";

			return date;
		}
		//std::cout << "TV_" << mbstr << ".txt" << std::endl;
		

	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << "\n";
	}
}
bool CreateZip()
{
	try
	{
		std::string ZipName = ReturnFileNameWithDate(0);

		std::ofstream out(ZipName, std::ios::binary);
		Poco::Zip::Compress c(out, true);

		
		Poco::File file(ReturnFileNameWithDate(1));
		Poco::Path failas = file.path();

		std::cout << "File path: " << file.path() << std::endl;
		c.addFile(failas, failas.getFileName());

		c.close(); // MUST be done to finalize the Zip file
		out.close();
		return true;
	}
	catch(const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
		return false;
	}
	
}

std::string trim(const std::string &line, const char &fromthistodelete)
{
	std::string newline = "";
	std::size_t pos = line.find(fromthistodelete);
	if (pos != -1)
	{
		newline = line.substr(0, pos);
	}
	else
	{
		//std::cout << "Klaida!";
	}
	return newline;
}



int main(int argc, char* argv[])
{
	//zip *zipas
	
	if (argc == 1)
	{
		try
		{

			std::cout << "Locale: " << std::locale("").name().c_str() << std::endl;
			std::cout << "CodePage: " << GetConsoleCP() << std::endl;
			SetConsoleCP(1257);
			std::cout << "CodePage: " << GetConsoleCP() << std::endl;
			std::cout << "Enter date, format.: 202012";
			int date=0;
			std::cin >> date;


			std::wstring test = L"irasyk \"CSV failo pavadinima kartu su pletiniu\"\nPvz.: failas.csv";
			std::wcout << test << L"\n";
			std::string line = "";
			std::string filename;
			std::cin >> filename;
			std::ifstream mycsv(filename);
			std::ofstream remade(ReturnFileNameWithDate(1));
			std::vector<std::string>parsedline;
			std::string wstringas = "";
			int nline = 0;
			int countas = 0;
			std::size_t found = 0;
			if (mycsv.is_open())
			{

				while (std::getline(mycsv, line))
				{
					std::stringstream wstr(line);

					while (std::getline(wstr, wstringas, '\t'))
					{
						parsedline.push_back(wstringas);
					}
					found = parsedline[15].find("LT-\r");
					if (found != std::string::npos) {
						std::cout << "first 'needle' found at: " << found << '\n';
						countas++;
						replace(parsedline[15], "LT-\r", "LT-");
					}
					parsedline[4] = trim(parsedline[4], '.');
					parsedline[6] = trim(parsedline[6], '.');
					parsedline[8] = trim(parsedline[8], '.');
					parsedline[10] = trim(parsedline[10], '.');
					std::cout << parsedline[1] << " " << parsedline[2] << " " << parsedline[4] << " " << parsedline[6] << " " << parsedline[8] << " " << parsedline[10] << " " << parsedline[15] << "\n";
					remade << parsedline[1] << "\t" << parsedline[2] << "\t" << parsedline[4] << "\t" << parsedline[6] << "\t" << parsedline[8] << "\t" << parsedline[10] << "\t" << parsedline[15] << "\n";
					//std::cout << line << std::endl;

					parsedline.clear();
				}
				std::cout << "Found: " << countas << std::endl;
				CreateZip();
			}
			else
			{
				std::cout << "Something wrong!" << std::endl;
			}
			std::cout << "CodePage: " << GetConsoleCP() << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}


		return 0;
	}
	if(argc == 2)
	{

	}
	else
	{
		std::cout << "Too much arguments!";
	}
	
	
}

#include"stdafx.h"
#include"GUID_Generator.h"
#include<guiddef.h>
#include<iomanip>



auto GUID_Generator::Generate() -> const uint //생성된 GUID를 HESH방식으로 숫자화
{
	std::hash<std::string> hasher;


	return static_cast<uint>(hasher(GenerateToString()));
}

auto GUID_Generator::GenerateToString() -> const std::string
{
	GUID guid;
	auto hr = CoCreateGuid(&guid); //생성
	
	if (SUCCEEDED(hr))
	{
		std::stringstream sstream;

		sstream
			<< std::hex << std::uppercase
			<< std::setw(8)/*자릿수 맞추기*/ << std::setfill('0')/*공간을 0으로 채워라*/ << guid.Data1 << "-"
			<< std::setw(4) << std::setfill('0') << guid.Data2 << "-"
			<< std::setw(4) << std::setfill('0') << guid.Data3 << "-"
			<< std::setw(2) << std::setfill('0') << static_cast<uint>(guid.Data4[0])
			<< std::setw(2) << std::setfill('0') << static_cast<uint>(guid.Data4[1])
			<< std::setw(2) << std::setfill('0') << static_cast<uint>(guid.Data4[2])
			<< std::setw(2) << std::setfill('0') << static_cast<uint>(guid.Data4[3])
			<< std::setw(2) << std::setfill('0') << static_cast<uint>(guid.Data4[4])
			<< std::setw(2) << std::setfill('0') << static_cast<uint>(guid.Data4[5])
			<< std::setw(2) << std::setfill('0') << static_cast<uint>(guid.Data4[6])
			<< std::setw(2) << std::setfill('0') << static_cast<uint>(guid.Data4[7]);
		return sstream.str();
	}


	assert(false);
	return "N/A";
}

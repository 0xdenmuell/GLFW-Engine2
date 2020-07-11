#pragma once

class UtilityFunc
{
public:

	static char* StringToChar(std::string stringValue) {

		unsigned int stringLength = stringValue.length() + 1;
		char* buffer = new char[stringLength];

		strcpy_s(buffer, stringLength, stringValue.c_str());
		return buffer;
	}
};
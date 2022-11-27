#pragma once

#include "../Utilities/rapidxml-1.13/rapidxml.hpp"
#include <string>
#include <iostream>

using namespace rapidxml;

class AttributeXML
{
public:
	/** Constructor */
	AttributeXML(xml_attribute<>* attr) : attribute(attr) {  }

	bool isValid()
	{
		if (attribute)
			return true;

		std::cout << "ERROR : ATTRIBUTE INVALID\n";
		return false;
	}

	int getInt()
	{
		if (isValid())
			return atoi(attribute->value());
		return -1;
	}

private:
	xml_attribute<>* attribute;
};


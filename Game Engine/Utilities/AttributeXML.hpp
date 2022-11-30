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
		return false;
	}

	int getInt()
	{
		if (isValid())
			return atoi(attribute->value());

		std::cout << "ERROR : getInt() - " << attribute->name() << " ATTRIBUTE INVALID\n";
		return -1;
	}

	std::string getString()
	{
		if (isValid())
			return attribute->value();

		std::cout << "ERROR : getString() - " << attribute->name() << " ATTRIBUTE INVALID\n";
		return "";
	}

private:
	xml_attribute<>* attribute;
};


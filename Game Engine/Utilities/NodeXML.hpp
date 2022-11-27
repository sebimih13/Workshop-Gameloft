#pragma once

#include "../Utilities/rapidxml-1.13/rapidxml.hpp"
#include <string>
#include <iostream>

#include "AttributeXML.hpp"

using namespace rapidxml;

class NodeXML
{
public:
	/** Constructor */
	NodeXML(xml_node<>* n) : node(n) {  }

	bool isValid()
	{
		if (node)
			return true;

		std::cout << "ERROR : NODE INVALID\n";
		return false;
	}

	/** Getters */
	int getInt() 
	{
		if (isValid())
			return atoi(node->value()); 
		return -1;
	}

	std::string getString() 
	{
		if (isValid())
			return node->value(); 
		return "";
	}

	float getFloat() 
	{ 
		if (isValid())
			return float(atof(node->value())); 
		return -1.0f;
	}

	NodeXML getChild(std::string name)
	{
		if (!isValid())
			return NodeXML(nullptr);

		xml_node<>* pChildNode = node->first_node(name.c_str());
		if (!pChildNode)
		{
			std::cout << "ERROR : " << name << " - node not found\n";
			return NodeXML(nullptr);
		}
		return NodeXML(pChildNode);
	}

	NodeXML getNextSibling()
	{
		if (!isValid())
			return NodeXML(nullptr);

		xml_node<>* pSiblingNode = node->next_sibling();
		if (!pSiblingNode)
		{
			std::cout << "ERROR : sibling not found\n";
			return NodeXML(nullptr);
		}
		return NodeXML(pSiblingNode);
	}

	AttributeXML getAttribute(std::string name)
	{
		if (!isValid())
			return AttributeXML(nullptr);

		xml_attribute<>* pAttr = node->first_attribute(name.c_str());
		if (!pAttr)
		{
			std::cout << "ERROR : " << name << " - attr not found\n";
			return AttributeXML(nullptr);
		}
		return AttributeXML(pAttr);
	}

private:
	xml_node<>* node;
};


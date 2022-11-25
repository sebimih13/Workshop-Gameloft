#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#include "rapidxml-1.13/rapidxml-1.13/rapidxml.hpp"

using namespace std;
using namespace rapidxml;

struct obiect
{
	unsigned int ID;
	string nume;

	struct
	{
		string info;
	} model;

	struct
	{
		string folder;
	} textura;
};

vector<obiect> obiecte;

int main()
{
	xml_document<> doc;
	std::ifstream file("exemplu_xml.xml");
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());
	doc.parse<0>(&content[0]);

	xml_node<> *pRoot = doc.first_node();		// <obiecte/> node
	for (xml_node<> *pNode = pRoot->first_node("obiect"); pNode; pNode = pNode->next_sibling())
	{
		/* this loop will walk you through two nodes <obiect/>
			* obiect id = "100"		nume = "zar"
			* obiect id = "27"		nume = "minge"
		*/
		
		// id
		xml_attribute<>* pAttrID = pNode->first_attribute("id");
		unsigned int id = atoi(pAttrID->value());

		// nume
		xml_attribute<>* pAttrNume = pNode->first_attribute("nume");
		string nume = pAttrNume->value();

		// model
		xml_node<>* pNodeModel = pNode->first_node("model");
		xml_attribute<>* pAttrInfo = pNodeModel->first_attribute("info");
		string info = "no";
		info = pAttrInfo->value();


		// textures
		xml_node<>* pNodeTextura = pNode->first_node("textura");
		xml_attribute<>* pAttrTextura = pNodeTextura->first_attribute("folder");
		string folder = "fno";
		folder = pAttrTextura->value();

		obiect ob;
		ob.ID = id;
		ob.nume = nume;
		ob.model.info = info;
		ob.textura.folder = folder;
		obiecte.push_back(ob);

		//cout << id << ' ' << nume << ' ' << info << ' ' << folder;
		//cout << '\n';
	}
	
	int index = 1;
	for (auto& ob : obiecte)
	{
		cout << "Obiectul " << index++ << " : ";
		cout << ob.ID << ' ' << ob.nume << ' ' << ob.model.info << ' ' << ob.textura.folder << '\n';
	}


	return 0;
}


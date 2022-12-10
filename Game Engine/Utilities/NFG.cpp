#include "stdafx.h"
#include "NFG.h"

NFGData* LoadNFG(const char* filePath)
{
	std::ifstream fin(filePath);
	if (fin.good())
	{
		std::cout << '\n' << filePath << " - VALID\n";
	}
	else
	{
		std::cout << '\n' << filePath << " - INVALID\n";
		return nullptr;
	}

	NFGData* data = new NFGData();

	int nrVertices;
	std::string extra;
	fin >> extra >> nrVertices;
	fin.get();

	data->nrVertices = nrVertices;
	
	// TODO : debug
	std::cout << extra << " : " << data->nrVertices << '\n';

	// Read vertices : pos + norm + binorm + tgt + uv
	for (int i = 0; i < nrVertices; i++)
	{
		std::string line;
		getline(fin, line);

		std::stringstream modifiedLine;

		for (char c : line)
			if (('0' <= c && c <= '9') || c == '-' || c == '.' || c == ' ')
				modifiedLine << c;

		NFGVertex v;
		modifiedLine >> v.pos.x >> v.pos.x >> v.pos.y >> v.pos.z;
		modifiedLine >> v.norm.x >> v.norm.y >> v.norm.z;
		modifiedLine >> v.binorm.x >> v.binorm.y >> v.binorm.z;
		modifiedLine >> v.tgt.x >> v.tgt.y >> v.tgt.z;
		modifiedLine >> v.uv.x >> v.uv.y;

		// TODO : DEBUG
		//std::cout << modifiedLine.str() << "\n";
		//std::cout << v.pos.x << ' ' << v.pos.y << ' ' << v.pos.z << "\n";
		//std::cout << v.norm.x << ' ' << v.norm.y << ' ' << v.norm.z << "\n";
		//std::cout << v.binorm.x << ' ' << v.binorm.y << ' ' << v.binorm.z << "\n";
		//std::cout << v.tgt.x << ' ' << v.tgt.y << ' ' << v.tgt.z << "\n";
		//std::cout << v.uv.x << ' ' << v.uv.y << ' ' << "\n\n\n";

		data->verticesData.push_back(v);
	}

	int nrIndices;
	fin >> extra >> nrIndices;
	fin.get();
	
	data->nrIndices = nrIndices;

	// TODO : debug
	std::cout << extra << " : " << data->nrIndices << '\n';

	// Read indices : 1. + 2. + 3.
	for (int i = 0; i < nrIndices / 3; i++)
	{
		std::string line;
		getline(fin, line);

		std::stringstream modifiedLine;

		for (char c : line)
			if (('0' <= c && c <= '9') || c == ' ')
				modifiedLine << c;

		unsigned int ind1, ind2, ind3;
		modifiedLine >> ind1 >> ind1;
		modifiedLine >> ind2;
		modifiedLine >> ind3;

		// TODO : DEBUG
		//std::cout << modifiedLine.str() << '\n';
		//std::cout << ind1 << ' ' << ind2 << ' ' << ind3 << "\n\n";

		data->indices.push_back(ind1);
		data->indices.push_back(ind2);
		data->indices.push_back(ind3);
	}

	return data;
}


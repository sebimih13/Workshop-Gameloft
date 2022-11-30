#pragma once

#include "stdafx.h"
#include "Math.h"

struct NFGVertex
{
	/** Constructor */
	NFGVertex() : pos(0.0f, 0.0f, 0.0f), norm(0.0f, 0.0f, 0.0f), binorm(0.0f, 0.0f, 0.0f), tgt(0.0f, 0.0f, 0.0f), uv(0.0f, 0.0f) {  }

	/** Copy Constructor */
	NFGVertex(const NFGVertex& v) : pos(v.pos), norm(v.norm), binorm(v.binorm), tgt(v.tgt), uv(v.uv) {  }

	Vector3 pos;
	Vector3 norm;
	Vector3 binorm;
	Vector3 tgt;
	Vector2 uv;
};

struct NFGData
{
	int nrVertices;
	std::vector<NFGVertex> verticesData;

	int nrIndices;
	std::vector<unsigned int> indices;
};

NFGData* LoadNFG(const char* filePath);


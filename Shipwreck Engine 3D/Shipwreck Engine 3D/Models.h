#pragma once

#include <iostream>
#include <vector>

#include "Model.h"

class Models
{
public:
	Models(std::vector<Model> _models) :
	modelList(_models)
	{}
	Models() :
	modelList(std::vector<Model>())
	{}

	void AddModel(Model _model) {
		modelList.push_back(_model);
	}

public:
	std::vector<Model> modelList;
};
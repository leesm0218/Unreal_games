// Fill out your copyright notice in the Description page of Project Settings.

#include "Doppel.h"
#include "StageInfo.h"

StageInfo::StageInfo(int width, int height, int num_doppees, TArray<TArray<int>> stage_infos)
	: num_doppees(num_doppees), width(width), height(height)
{
	for (int i = 0; i < height; i++) {
		this->stage_info.Add(TArray<int>());
		for (int j = 0; j < width; j++) {
			this->stage_info[i].Add(stage_infos[i][j]);
		}
	}
}

StageInfo::~StageInfo()
{
}

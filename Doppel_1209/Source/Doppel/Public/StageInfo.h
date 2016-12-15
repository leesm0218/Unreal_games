// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class DOPPEL_API StageInfo
{
public:
	StageInfo(int width, int height, int num_doppees, TArray<TArray<int>> stage_infos);
	~StageInfo();

public:
	int width;
	int height;
	int num_doppees;
	TArray<TArray<int>> stage_info;
};

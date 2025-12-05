#pragma once
#include <unordered_map>
struct FuncNode
{
	int hitCount;
	void* functionPtr;
	std::unordered_map<void*, FuncNode> children;

	FuncNode() : hitCount(0), functionPtr(nullptr) {}
};


#pragma once
#define WIN32_LEAN_AND_MEAN

#include <sstream>
#include <iomanip>
#include "string"
#include <iostream>
#include "stdafx.h"

#define EXPORT __declspec(dllexport)

using namespace std;

EXPORT string GetSystemInformation();
EXPORT string GetMemoryInformation();

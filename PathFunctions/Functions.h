#pragma once
#define PORTABLE_FUNCTION extern "C" __declspec(dllexport) 

PORTABLE_FUNCTION int Function();
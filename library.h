#ifndef ADAPTER_LIBRARY_H
#define ADAPTER_LIBRARY_H

#include <windows.h>
#include <cmath>
#define _hypot hypot;
#include <Python.h>
#include <map>
#include <vector>



#ifdef PYTHON_APTER_EXPORT
#   define PYTHON_ADAPTER_API __declspec(dllexport)
#else
#   define PYTHON_ADAPTER_API __declspec(dllimport)
#endif


static std::map<const char*, PyObject *> python_modules_;
static std::vector<const char*> python_module_names_;

extern "C" BOOL WINAPI  DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

void PYTHON_ADAPTER_API hello();
extern "C" int PYTHON_ADAPTER_API load_module(const char *module_name);
extern "C" char* PYTHON_ADAPTER_API call(int handle, const char *function_name, const char *inputs, int input_length, int *result_length);

extern "C" char* PYTHON_ADAPTER_API convert_to_string(double value);
extern "C" void PYTHON_ADAPTER_API initilize_python(const char* python_home);

static PyObject *msgpack_pack_;
static PyObject *msgpack_unpack_;




#endif
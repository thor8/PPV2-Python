#include "library.h"

#include <iostream>

 void PYTHON_ADAPTER_API hello() {
    std::cout << "Hello, World!" << std::endl;
}

extern "C" BOOL WINAPI  DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch(fdwReason) {
        case DLL_PROCESS_ATTACH:
        case DLL_PROCESS_DETACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        default:
            break;
    }
    return TRUE;
}

extern "C" void PYTHON_ADAPTER_API initilize_python(const char* python_home){
    const size_t cSize = strlen(python_home)+1;
    wchar_t* w_python_home = new wchar_t[cSize];
    mbstowcs (w_python_home, python_home, cSize);
    Py_SetPythonHome(w_python_home);
    Py_Initialize();
    PyObject *py_module = PyImport_ImportModule("msgpack");
    msgpack_pack_ = PyObject_GetAttrString(py_module, "packb");
    msgpack_unpack_ = PyObject_GetAttrString(py_module, "unpackb");
}

extern "C" char* PYTHON_ADAPTER_API convert_to_string(double value){
    PyObject *float_o = PyFloat_FromDouble(value);
    PyObject *tt = PyTuple_New(1);
    PyTuple_SetItem(tt, 0, float_o);
    PyObject *result = PyObject_Call(msgpack_pack_, tt, NULL);
    int result_length = PyBytes_Size(result) + 1;
    char *buffer = new char[result_length];
    strcpy(buffer, PyBytes_AsString(result));
    return buffer;
}

extern "C" int PYTHON_ADAPTER_API load_module(const char *module_name) {
    PyObject *py_module = PyImport_ImportModule(module_name);
    python_modules_[module_name] = py_module;
    python_module_names_.push_back(module_name);
    return python_module_names_.size() - 1;
}


extern "C" char* PYTHON_ADAPTER_API call(int handle, const char *function_name, const char *inputs, int input_length, int *result_length){


    const char* module_name = python_module_names_[handle];
    PyObject *python_module = python_modules_[module_name];
    PyObject *py_function = PyObject_GetAttrString(python_module, function_name);

    // convert input
    PyObject *py_input_bytes = PyBytes_FromStringAndSize(inputs, input_length);
    PyObject *input_tuple = PyTuple_New(1);
    PyTuple_SetItem(input_tuple, 0, py_input_bytes);
    PyObject *py_input_object = PyObject_Call(msgpack_unpack_, input_tuple, NULL);
    double d = PyFloat_AsDouble(py_input_object);
    Py_DECREF(py_input_bytes);
    Py_DECREF(input_tuple);

    //call method
    PyObject *function_input_tuple = PyTuple_New(1);
    PyTuple_SetItem(function_input_tuple, 0, py_input_object);
    PyObject *result_obj = PyObject_Call(py_function, function_input_tuple, NULL);
    Py_DECREF(py_input_object);
    Py_DECREF(function_input_tuple);

    //convert result
    PyObject *result_tuple = PyTuple_New(1);
    PyTuple_SetItem(result_tuple, 0, result_obj);
    PyObject *result = PyObject_Call(msgpack_pack_, result_tuple, NULL);
    Py_DECREF(result_obj);
    Py_DECREF(result_tuple);


    *result_length = PyBytes_Size(result) + 1;
    char *buffer = new char[*result_length];
    strcpy(buffer, PyBytes_AsString(result));
    Py_DECREF(result);
    return buffer;


}
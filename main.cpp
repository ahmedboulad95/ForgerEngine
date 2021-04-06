#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iostream>
#include <string>
#include "media.hpp"

void println(std::string str) {
    std::cout << "[C++] " << str << std::endl;
}

void initPy() {
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("import os");
    PyRun_SimpleString("sys.path.append(os.path.dirname(os.getcwd()) + '/forger/')");
}

int main(int argc, char *argv[]) {
    /*if(argc < 3) {
        fprintf(stderr, "Usage: call pythonfile funcname [args]\n");
        return 1;
    }*/

    initPy();

    const char* scriptName = "myscript";

    Media *media = new Media();
    media->init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);
    if(media == nullptr) {
        println("Failed to init media");
    } else {
        while(media->running()) {
            media->handleEvents();

            PyObject *pName = PyUnicode_DecodeFSDefault(scriptName);

            PyObject *pModule = PyImport_Import(pName);
            Py_DECREF(pName);

            if(pModule != NULL) {
                PyObject *pFunc = PyObject_GetAttrString(pModule, "update");

                if(pFunc && PyCallable_Check(pFunc)) {
                    PyObject *pValue = PyObject_CallObject(pFunc, NULL);

                    if(pValue != NULL) {
                        println("Result of call: " + std::to_string(PyLong_AsLong(pValue)));
                        Py_DECREF(pValue);
                    } else {
                        Py_DECREF(pFunc);
                        Py_DECREF(pModule);
                        PyErr_Print();
                        println("Call failed");
                        return 1;
                    }
                }
            }

            /*if(Py_FinalizeEx() < 0) {
                return 120;
            }*/

            media->render();
        }
    }

    delete media;
    media = nullptr;

    /*initPy();

    PyObject *pName = PyUnicode_DecodeFSDefault(argv[1]);

    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if(pModule != NULL) {
        PyObject *pFunc = PyObject_GetAttrString(pModule, argv[2]);

        if(pFunc && PyCallable_Check(pFunc)) {
            PyObject *pValue;
            PyObject *pArgs = PyTuple_New(argc - 3);
            for(int i = 0; i < argc - 3; ++i) {
                pValue = PyLong_FromLong(atoi(argv[i + 3]));
                if(!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    println("Cannot convert argument");
                    return 1;
                }
                PyTuple_SetItem(pArgs, i, pValue);
            }
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if(pValue != NULL) {
                println("Result of call: " + std::to_string(PyLong_AsLong(pValue)));
                Py_DECREF(pValue);
            } else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                println("Call failed");
                return 1;
            }
        } else {
            if(PyErr_Occurred())
                PyErr_Print();
            println(std::strcat("Cannot find function ", argv[2]));
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        println(std::strcat("Failed to load ", argv[1]));
        return 1;
    }

    if(Py_FinalizeEx() < 0) {
        return 120;
    }
*/


    return 0;
}
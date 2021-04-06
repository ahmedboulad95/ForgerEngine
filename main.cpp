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

    return 0;
}
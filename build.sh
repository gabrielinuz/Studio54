#!/bin/bash
## g++ es el binario del compilador de C++, 
## a continuación se detallan los parámetros usados

##Parámetro 1:
##-std=c++?? donde ?? es el número de versión del estándar de C++
##sirve para ajustarse más a un estándar u otro y también para
##poder utilizar características más avanzadas de C++
##https://stackoverflow.com/questions/44734397/which-c-standard-is-the-default-when-compiling-with-g

##Parámetro 2:
##-Wall -Wextra -Wpedantic -Werror son parámetros para mostrar errores
## y adevertencias varias...
##https://stackoverflow.com/questions/73310403/whats-the-difference-between-wextra-and-pedantic-in-gcc

##Parámetro 3:
##El parámetro -I indica en que directorio buscar las cabeceras
##(headers, archivos .h y .hpp) que yo pueda querer incluir.

##Uso el escape " \" de barra para mejor lectura.
g++ \
-std=c++17 \
-Wextra -Wpedantic -Werror \
-I./include \
src/AppFlow.cpp src/GuestManager.cpp src/Utils.cpp src/main.cpp \
-o studio54.app
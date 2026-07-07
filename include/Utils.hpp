#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

// =========================================================================
// DECLARACIÓN DE FUNCIONES AUXILIARES Y DE PROCESAMIENTO
// =========================================================================

void pauseConsole();
void clearConsole();
std::string trim(const std::string& str);//Quitar espacios de principio y final
std::string sanitize(const std::string& str);//Limpiar , y ; especiales.

#endif // UTILS_HPP
#include "Utils.hpp"
#include <iostream>
#include <cstdlib> // Para system()

using namespace std;

// =========================================================================
// FUNCIONES AUXILIARES Y DE PROCESAMIENTO de CADENAS
// =========================================================================

/**
 * @brief Detiene temporalmente la ejecución para permitir la lectura en consola.
 */
void pauseConsole() {
    cout << "\nPresione Enter para continuar...";
    cin.get();
}

/**
 * @brief Borra los caracteres visibles de la pantalla según el entorno operativo.
 */
void clearConsole() {
    #ifdef __unix__
        system("clear");
    #elif __APPLE__
        system("clear");
    #elif defined(_WIN32) || defined(WIN32) || defined(WIN64)
        system("cls");
    #endif
}

/**
 * @brief Elimina espacios en blanco iniciales y finales de una cadena de texto.
 * @param str Cadena original.
 * @return Cadena procesada sin espacios residuales.
 */
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

/**
 * @brief Sanitiza una cadena eliminando comas para evitar la corrupción del archivo CSV.
 * @param str Cadena de caracteres a limpiar.
 * @return Cadena apta para almacenamiento seguro en formato CSV.
 */
string sanitize(const string& str) {
    string cleanStr = trim(str);
    size_t i = 0; // Se modifica a size_t para machacar el error de comparacion de signo con length()
    // Recorremos la cadena usando un ciclo while clásico
    while (i < cleanStr.length()) {
        if (cleanStr[i] == ',' || cleanStr[i] == ';') {
            cleanStr[i] = ' '; // Reemplaza comas delimitadoras por espacios seguros
        }
        i++;
    }
    return cleanStr;
}
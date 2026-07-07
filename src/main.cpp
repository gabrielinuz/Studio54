/**
 * @file main.cpp
 * @brief Sistema Avanzado de Gestión de Invitaciones para Studio 54 N.Y. 
 * (Versión CSV, Sanitizada y Ordenada)
 * Para aplicar el paso por referencia de forma estricta y nativa en C++, 
 * se ha modificado la firma de las funciones que reciben el arreglo de invitados.
 * En lugar de utilizar la sintaxis clásica de C (Guest guests[]), la cual degrada 
 * implícitamente el arreglo a un puntero ordinario (Guest*), se ha implementado el paso 
 * de arreglos por referencia explícita utilizando la sintaxis Guest (&guests)[MAX_GUESTS]. 
 * Esto garantiza que el arreglo se transmita sin degradación de tipos, manteniendo el control 
 * del tamaño del arreglo en tiempo de compilación y asegurando un paso por referencia real de 
 * toda la colección. En los casos de solo lectura, se aplica const Guest (&guests)[MAX_GUESTS].
 */

#include "Guest.hpp"
#include "AppFlow.hpp"

// =========================================================================
// FUNCIÓN PRINCIPAL
// =========================================================================

int main() {
    Guest guests[MAX_GUESTS]; /**< Arreglo estático de estructuras tipo Guest */
    int guestCount = 0;       /**< Cantidad real de registros activos en memoria */

    initializeApp(guests, guestCount);
    runMenuLoop(guests, guestCount);
    shutdownApp(guests, guestCount);
    
    return 0;
}
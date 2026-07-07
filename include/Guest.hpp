#ifndef GUEST_HPP
#define GUEST_HPP

#include <string>

// =========================================================================
// CONSTANTES Y ESTRUCTURAS
// =========================================================================

const int MAX_GUESTS = 100; /**< Capacidad máxima del arreglo de invitados */

/**
 * @struct Guest
 * @brief Representación estructurada de los datos de un invitado mediante typedef.
 */
typedef struct {
    std::string firstName;    /**< Nombre del invitado (Soporta espacios) */
    std::string lastName;     /**< Apellido del invitado (Soporta espacios) */
    std::string dni;          /**< Documento Nacional de Identidad (Modificado a string para soportar formatos y puntos) */
    std::string rsvpStatus;   /**< Confirmación de asistencia ("si" o "no") */
    int ticketNumber;    /**< Código identificador de ticket único */
} Guest;

#endif // GUEST_HPP
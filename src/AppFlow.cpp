#include "AppFlow.hpp"
#include "GuestManager.hpp"
#include "Utils.hpp"
#include <iostream>

using namespace std;

// =========================================================================
// FUNCIONES DE CONTROL DE FLUJO ALTO NIVEL
// =========================================================================

/**
 * @brief Prepara el estado inicial de la aplicación sincronizando y ordenando los datos.
 * @param guests Arreglo estático de invitados.
 * @param guestCount Referencia al contador de registros.
 */
void initializeApp(Guest (&guests)[MAX_GUESTS], int& guestCount) {
    // Sincroniza la memoria RAM con los datos del archivo CSV
    loadFromFile(guests, guestCount);
    // Asegura que los datos inicien ordenados por número de ticket
    bubbleSortGuests(guests, guestCount);
}

/**
 * @brief Orquesta el ciclo de interacción del menú principal y sus operaciones.
 * @param guests Arreglo estático de invitados.
 * @param guestCount Referencia al contador de registros.
 */
void runMenuLoop(Guest (&guests)[MAX_GUESTS], int& guestCount) {
    int option = 0;           /**< Variable para capturar la opción del menú */

    for (;;) {
        clearConsole();
        displayMenu();
        
        cout << "Ingrese una opcion: ";
        if (!(cin >> option)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore(1000, '\n'); // Limpia el búfer tras leer un entero

        if (option == 6) {
            break; /**< Rompe el ciclo para finalizar la ejecución del programa */
        }

        switch (option) {
            case 1:
                addGuest(guests, guestCount);
                break;
            case 2:
                changeRsvp(guests, guestCount);
                break;
            case 3:
                showGuests(guests, guestCount);
                break;
            case 4:
                editGuest(guests, guestCount);
                break;
            case 5:
                deleteGuest(guests, guestCount);
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo." << endl;
                pauseConsole();
                break;
        }
    }
}

/**
 * @brief Finaliza de forma segura la ejecución guardando los cambios pendientes.
 * @param guests Arreglo estático de invitados.
 * @param guestCount Cantidad de registros en memoria.
 */
void shutdownApp(const Guest (&guests)[MAX_GUESTS], int guestCount) {
    // Persistencia definitiva en el CSV antes de salir
    saveToFile(guests, guestCount);
}

// =========================================================================
// DEFINICIÓN DE FUNCIONES DE INTERFAZ
// =========================================================================

/**
 * @brief Renderiza la interfaz de texto con las opciones funcionales de la APP.
 */
void displayMenu() {
    cout << "=== STUDIO 54 N.Y. - GESTION DE INVITACIONES ===" << endl;
    cout << "1. Agregar Invitado(s)" << endl;
    cout << "2. Cambiar confirmacion de Asistencia (RSVP)" << endl;
    cout << "3. Ver lista de Asistentes" << endl;
    cout << "4. Editar datos de un Invitado" << endl;
    cout << "5. Eliminar un Invitado" << endl;
    cout << "6. Salir" << endl;
    cout << "================================================" << endl;
}
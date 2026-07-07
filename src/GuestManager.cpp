#include "GuestManager.hpp"
#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream> // Para el procesamiento de archivos CSV

using namespace std;

// =========================================================================
// DEFINICIÓN DE FUNCIONES DE NEGOCIO Y OPERACIONES DE INVITADOS
// =========================================================================

/**
 * @brief Función buscadora unificada que localiza un invitado por su número de ticket.
 * @param guests Arreglo de datos.
 * @param count Cantidad lógica de registros.
 * @param ticketNumber Número de ticket a buscar.
 * @return Índice del invitado si se encuentra, o -1 si no existe.
 */
int findGuestByTicket(const Guest (&guests)[MAX_GUESTS], int count, int ticketNumber) {
    for (int i = 0; i < count; i++) {
        if (guests[i].ticketNumber == ticketNumber) {
            return i; // Retorna la posición física en el arreglo
        }
    }
    return -1; // Elemento no localizado
}

/**
 * @brief Ordena el arreglo de invitados por Número de Ticket usando el algoritmo de Burbuja.
 * @param guests Arreglo a ordenar.
 * @param count Cantidad de elementos.
 */
void bubbleSortGuests(Guest (&guests)[MAX_GUESTS], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (guests[j].ticketNumber > guests[j + 1].ticketNumber) {
                // Intercambio de estructuras completas
                Guest temp = guests[j];
                guests[j] = guests[j + 1];
                guests[j + 1] = temp;
            }
        }
    }
}

/**
 * @brief Lee el archivo "invitados.csv" mapeando los campos delimitados por comas.
 * @param guests Arreglo destino.
 * @param count Contador de posiciones llenadas.
 * @return Entero con la cantidad de elementos procesados.
 */
int loadFromFile(Guest (&guests)[MAX_GUESTS], int& count) {
    ifstream inFile("invitados.csv");
    count = 0;
    string line;

    if (inFile.is_open()) {
        // Lee línea por línea para parsear correctamente campos con espacios
        while (getline(inFile, line) && count < MAX_GUESTS) {
            line = trim(line);
            if (line.empty()) continue;

            stringstream ss(line);
            string dniStr, ticketStr;

            // Extrae datos delimitados por comas (formato CSV)
            getline(ss, guests[count].firstName, ',');
            getline(ss, guests[count].lastName, ',');
            getline(ss, dniStr, ',');
            getline(ss, guests[count].rsvpStatus, ',');
            getline(ss, ticketStr, ',');

            // Convierte cadenas numéricas de forma segura mediante flujos
            guests[count].dni = sanitize(dniStr); // El DNI ya no requiere conversión a entero via stringstream
            stringstream(ticketStr) >> guests[count].ticketNumber;

            count++;
        }
        inFile.close();
    }
    return count;
}

/**
 * @brief Guarda la información del arreglo en el archivo "invitados.csv".
 * @param guests Arreglo origen.
 * @param count Elementos lógicos existentes.
 */
void saveToFile(const Guest (&guests)[MAX_GUESTS], int count) {
    ofstream outFile("invitados.csv");

    if (outFile.is_open()) {
        for (int i = 0; i < count; i++) {
            // Se guardan los campos separados estrictamente por comas
            outFile << guests[i].firstName << "," 
                    << guests[i].lastName << "," 
                    << guests[i].dni << "," 
                    << guests[i].rsvpStatus << "," 
                    << guests[i].ticketNumber << endl;
        }
        outFile.close();
    } else {
        cout << "Error critico: No se pudo escribir en el medio de almacenamiento." << endl;
    }
}

/**
 * @brief Captura datos soportando caracteres especiales, sanitiza y los añade a la lista.
 * @param guests Arreglo estático.
 * @param count Puntero lógico de llenado.
 */
void addGuest(Guest (&guests)[MAX_GUESTS], int& count) {
    clearConsole();
    int amountToAdd = 0;
    
    cout << "Cuantos invitados desea agregar? ";
    cin >> amountToAdd;
    cin.ignore(1000, '\n'); // Limpia el residuo de cin

    while (amountToAdd <= 0) {
        cout << "Por favor ingrese una cantidad mayor a 0: ";
        cin >> amountToAdd;
        cin.ignore(1000, '\n');
    }

    // Identificación del ticket más alto para evitar repeticiones por bajas lógicas/físicas
    int maxTicket = 999;
    for (int i = 0; i < count; i++) {
        if (guests[i].ticketNumber > maxTicket) {
            maxTicket = guests[i].ticketNumber;
        }
    }

    for (int added = 0; added < amountToAdd && count < MAX_GUESTS; added++) {
        cout << "\n--- Datos del invitado [" << (count + 1) << "] ---" << endl;
        
        string rawInput;

        cout << "Nombre: ";
        getline(cin, rawInput);
        guests[count].firstName = sanitize(rawInput); // Remueve espacios huérfanos y comas
        
        cout << "Apellido: ";
        getline(cin, rawInput);
        guests[count].lastName = sanitize(rawInput);
        
        cout << "DNI: ";
        getline(cin, rawInput);
        guests[count].dni = sanitize(rawInput); // Lectura directa como cadena de texto sanitizada
        
        cout << "Confirma asistencia? (si/no): ";
        getline(cin, rawInput);
        guests[count].rsvpStatus = sanitize(rawInput);

        // Generación dinámica autoincremental de ticket
        maxTicket++;
        guests[count].ticketNumber = maxTicket; 
        cout << "-> Numero de ticket generado: " << guests[count].ticketNumber << endl;

        count++; 
    }

    bubbleSortGuests(guests, count); // Reordena la lista tras las inserciones
    saveToFile(guests, count);       // Almacena en disco
    cout << "\nRegistros guardados y ordenados correctamente." << endl;
    pauseConsole();
}

/**
 * @brief Recorre la estructura e imprime la totalidad de los datos en pantalla.
 * @param guests Arreglo estático de lectura de datos.
 * @param count Tamaño del segmento con datos válidos.
 */
void showGuests(const Guest (&guests)[MAX_GUESTS], int count) {
    clearConsole();

    if (count == 0) {
        cout << "La lista se encuentra vacia actualmente." << endl;
        pauseConsole();
        return;
    }

    cout << "=== LISTA DE ACCESOS - STUDIO 54 (TOTAL: " << count << ") ===" << endl;
    
    for (int i = 0; i < count; i++) {
        cout << "-----------------------------------------------" << endl;
        cout << "Ticket Nro: " << guests[i].ticketNumber << " | ";
        cout << "Invitado: " << guests[i].lastName << ", " << guests[i].firstName << endl;
        cout << "DNI: " << guests[i].dni << " | ";
        cout << "Asistencia Confirmada: " << guests[i].rsvpStatus << endl;
    }
    
    cout << "-----------------------------------------------" << endl;
    pauseConsole();
}

/**
 * @brief Modifica el RSVP de un usuario utilizando la función buscadora integrada.
 * @param guests Colección indexada de invitados.
 * @param count Cardinalidad del conjunto de datos.
 */
void changeRsvp(Guest (&guests)[MAX_GUESTS], int count) {
    clearConsole();
    
    if (count == 0) {
        cout << "Operacion cancelada: No existen registros cargados." << endl;
        pauseConsole();
        return;
    }

    int searchTicket = 0;
    cout << "Ingrese el nro de Ticket a buscar: ";
    cin >> searchTicket;
    cin.ignore(1000, '\n');

    // Implementación de la función buscadora centralizada
    int index = findGuestByTicket(guests, count, searchTicket);

    if (index != -1) {
        string newRsvp;
        cout << "Registro Localizado: " << guests[index].firstName << " " << guests[index].lastName << endl;
        cout << "Estado RSVP actual: " << guests[index].rsvpStatus << endl;
        cout << "Ingrese el nuevo estado (si/no): ";
        getline(cin, newRsvp);
        guests[index].rsvpStatus = sanitize(newRsvp);
        
        saveToFile(guests, count);
        cout << "Modificacion persistida con exito." << endl;
    } else {
        cout << "No se encontro correspondencia para el ticket ingresado." << endl;
    }

    pauseConsole();
}

/**
 * @brief Permite reescribir los datos personales de un invitado usando la función buscadora.
 * @param guests Colección indexada de invitados.
 * @param count Cardinalidad del conjunto de datos.
 */
void editGuest(Guest (&guests)[MAX_GUESTS], int count) {
    clearConsole();

    if (count == 0) {
        cout << "No hay invitados registrados para editar." << endl;
        pauseConsole();
        return;
    }

    int searchTicket = 0;
    cout << "Ingrese el nro de Ticket del invitado a modificar: ";
    cin >> searchTicket;
    cin.ignore(1000, '\n');

    // Uso de la función buscadora requerida
    int index = findGuestByTicket(guests, count, searchTicket);

    if (index != -1) {
        string rawInput;
        cout << "\n--- Modificando datos para Ticket: " << guests[index].ticketNumber << " ---" << endl;
        
        cout << "Nombre actual [" << guests[index].firstName << "]. Nuevo: ";
        getline(cin, rawInput);
        if (!trim(rawInput).empty()) guests[index].firstName = sanitize(rawInput);

        cout << "Apellido actual [" << guests[index].lastName << "]. Nuevo: ";
        getline(cin, rawInput);
        if (!trim(rawInput).empty()) guests[index].lastName = sanitize(rawInput);

        cout << "DNI actual [" << guests[index].dni << "]. Nuevo: ";
        string dniInput;
        getline(cin, dniInput);
        if (!trim(dniInput).empty()) {
            guests[index].dni = sanitize(dniInput); // Modificado para asignar directamente la cadena sanitizada sin flujos numéricos
        }

        cout << "RSVP actual [" << guests[index].rsvpStatus << "]. Nuevo (si/no): ";
        getline(cin, rawInput);
        if (!trim(rawInput).empty()) guests[index].rsvpStatus = sanitize(rawInput);

        saveToFile(guests, count);
        cout << "\nDatos actualizados con éxito en el archivo CSV." << endl;
    } else {
        cout << "Error: No se encontro ningun asistente asignado al ticket " << searchTicket << "." << endl;
    }

    pauseConsole();
}

/**
 * @brief Aplica una baja lógica/física localizando al objetivo mediante la función buscadora.
 * @param guests Vector de estructuras.
 * @param count Tamaño dinámico interno de control.
 */
void deleteGuest(Guest (&guests)[MAX_GUESTS], int& count) {
    clearConsole();

    if (count == 0) {
        cout << "No existen elementos disponibles para su remocion." << endl;
        pauseConsole();
        return;
    }

    int searchTicket = 0;
    cout << "Ingrese el nro de Ticket del invitado a dar de baja: ";
    cin >> searchTicket;
    cin.ignore(1000, '\n');

    // Llamado estratégico a la función de búsqueda
    int index = findGuestByTicket(guests, count, searchTicket);

    if (index != -1) {
        // Ejecución del algoritmo de borrado por corrimiento a la izquierda
        for (int j = index; j < count - 1; j++) {
            guests[j] = guests[j + 1];
        }
        
        count--; // Disminución de la frontera del arreglo
        saveToFile(guests, count);
        cout << "El invitado ha sido removido del sistema." << endl;
    } else {
        cout << "Operacion fallida: El codigo de ticket no coincide con ningun asistente." << endl;
    }

    pauseConsole();
}
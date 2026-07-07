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

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // Para system()
#include <sstream> // Para el procesamiento de archivos CSV

using namespace std;

// =========================================================================
// CONSTANTES Y ESTRUCTURAS
// =========================================================================

const int MAX_GUESTS = 100; /**< Capacidad máxima del arreglo de invitados */

/**
 * @struct Guest
 * @brief Representación estructurada de los datos de un invitado mediante typedef.
 */
typedef struct {
    string firstName;    /**< Nombre del invitado (Soporta espacios) */
    string lastName;     /**< Apellido del invitado (Soporta espacios) */
    string dni;          /**< Documento Nacional de Identidad (Modificado a string para soportar formatos y puntos) */
    string rsvpStatus;   /**< Confirmación de asistencia ("si" o "no") */
    int ticketNumber;    /**< Código identificador de ticket único */
} Guest;

// =========================================================================
// DECLARACIÓN DE FUNCIONES (PROTOTIPOS)
// =========================================================================

void pauseConsole();
void clearConsole();
string trim(const string& str);//Quitar espacios de principio y final
string sanitize(const string& str);//Limpiar , y ; especiales.
int findGuestByTicket(const Guest (&guests)[MAX_GUESTS], int count, int ticketNumber);
void bubbleSortGuests(Guest (&guests)[MAX_GUESTS], int count);

// El contador solo pasa por referencia si la funcion modifica la cantidad total
int loadFromFile(Guest (&guests)[MAX_GUESTS], int& count);
void saveToFile(const Guest (&guests)[MAX_GUESTS], int count); // Se justifica el paso por valor (lectura)
void displayMenu();
void addGuest(Guest (&guests)[MAX_GUESTS], int& count);
void showGuests(const Guest (&guests)[MAX_GUESTS], int count);
void changeRsvp(Guest (&guests)[MAX_GUESTS], int count);
void editGuest(Guest (&guests)[MAX_GUESTS], int count);
void deleteGuest(Guest (&guests)[MAX_GUESTS], int& count);

// Prototipos de las funciones organizadoras del flujo principal
void initializeApp(Guest (&guests)[MAX_GUESTS], int& guestCount);
void runMenuLoop(Guest (&guests)[MAX_GUESTS], int& guestCount);
void shutdownApp(const Guest (&guests)[MAX_GUESTS], int guestCount); // Se justifica el paso por valor (lectura)

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
    int i = 0;
    // Recorremos la cadena usando un ciclo while clásico
    while (i < cleanStr.length()) {
        if (cleanStr[i] == ',' || cleanStr[i] == ';') {
            cleanStr[i] = ' '; // Reemplaza comas delimitadoras por espacios seguros
        }
        i++;
    }
    return cleanStr;
}

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

// =========================================================================
// DEFINICIÓN DE FUNCIONES DE NEGOCIO
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
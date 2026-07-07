#ifndef GUEST_MANAGER_HPP
#define GUEST_MANAGER_HPP

#include "Guest.hpp"

// =========================================================================
// DECLARACIÓN DE FUNCIONES DE NEGOCIO Y GESTIÓN
// =========================================================================

int findGuestByTicket(const Guest (&guests)[MAX_GUESTS], int count, int ticketNumber);
void bubbleSortGuests(Guest (&guests)[MAX_GUESTS], int count);

// El contador solo pasa por referencia si la funcion modifica la cantidad total
int loadFromFile(Guest (&guests)[MAX_GUESTS], int& count);
void saveToFile(const Guest (&guests)[MAX_GUESTS], int count); // Se justifica el paso por valor (lectura)
void addGuest(Guest (&guests)[MAX_GUESTS], int& count);
void showGuests(const Guest (&guests)[MAX_GUESTS], int count);
void changeRsvp(Guest (&guests)[MAX_GUESTS], int count);
void editGuest(Guest (&guests)[MAX_GUESTS], int count);
void deleteGuest(Guest (&guests)[MAX_GUESTS], int& count);

#endif // GUEST_MANAGER_HPP
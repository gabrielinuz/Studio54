#ifndef APP_FLOW_HPP
#define APP_FLOW_HPP

#include "Guest.hpp"

// =========================================================================
// DECLARACIÓN DE FUNCIONES ORGANIZADORAS DEL FLUJO PRINCIPAL
// =========================================================================

void displayMenu();
void initializeApp(Guest (&guests)[MAX_GUESTS], int& guestCount);
void runMenuLoop(Guest (&guests)[MAX_GUESTS], int& guestCount);
void shutdownApp(const Guest (&guests)[MAX_GUESTS], int guestCount); // Se justifica el paso por valor (lectura)

#endif // APP_FLOW_HPP
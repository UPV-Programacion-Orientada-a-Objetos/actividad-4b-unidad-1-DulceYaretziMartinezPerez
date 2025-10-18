#include "Simulador2D.h"
#include <iostream>

int main() {
    int filas, columnas, numFuentes;

    std::cout << "Filas del grid: "; std::cin >> filas;
    std::cout << "Columnas del grid: "; std::cin >> columnas;

    Simulador2D<float> sim(filas, columnas);

    std::cout << "Cuántas fuentes quieres agregar? "; std::cin >> numFuentes;

    for(int i=0; i<numFuentes; i++){
        float valor;
        std::cout << "Valor de la fuente " << i+1 << ": ";
        std::cin >> valor;
        sim.agregarFuente(valor);

        int f, c;
        std::cout << "Fila y columna donde colocar la fuente " << i+1 << ": ";
        std::cin >> f >> c;
        sim.colocarFuenteEn(f, c, i);
    }

    std::cout << "--- Grid Inicial ---\n";
    sim.mostrarGrid();

    int pasos;
    std::cout << "Cuántos pasos de simulación quieres ejecutar? ";
    std::cin >> pasos;

    for(int i=0; i<pasos; i++){
        sim.simularPaso();
        std::cout << "--- Grid Paso " << i+1 << " ---\n";
        sim.mostrarGrid();
    }

    return 0;
}

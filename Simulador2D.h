#include <iostream>
#include <string>
#include <new> // Para try/catch std::bad_alloc

template <typename T>
class Simulador2D {
private:
    T **_grid;
    int _filas;
    int _columnas;

    T *_fuentes;
    int _numFuentes;
    int _capacidadFuentes;

    float _constantes[3]; // [0]=Difusión, [1]=DeltaX, [2]=DeltaT

public:
    // Constructor
    Simulador2D(int filas, int columnas) : _filas(filas), _columnas(columnas), _numFuentes(0), _capacidadFuentes(2) {
        try {
            _grid = new T*[_filas];
            for(int i=0; i<_filas; i++){
                _grid[i] = new T[_columnas];
                for(int j=0; j<_columnas; j++) _grid[i][j] = 0;
            }

            _fuentes = new T[_capacidadFuentes];
            for(int i=0; i<_capacidadFuentes; i++) _fuentes[i] = 0;

            _constantes[0] = 0.1f; // Coef. Difusión
            _constantes[1] = 1.0f; // DeltaX
            _constantes[2] = 1.0f; // DeltaT
        } catch(std::bad_alloc &e){
            std::cout << "Error de memoria: " << e.what() << std::endl;
            exit(1);
        }
    }

    // Destructor
    ~Simulador2D(){
        for(int i=0; i<_filas; i++) delete[] _grid[i];
        delete[] _grid;
        delete[] _fuentes;
    }

    // Redimensionar la matriz
    void redimensionarGrid(int nuevaF, int nuevaC){
        T **nuevoGrid = nullptr;
        try {
            nuevoGrid = new T*[nuevaF];
            for(int i=0; i<nuevaF; i++){
                nuevoGrid[i] = new T[nuevaC];
                for(int j=0; j<nuevaC; j++) nuevoGrid[i][j] = 0;
            }

            for(int i=0; i<std::min(_filas, nuevaF); i++)
                for(int j=0; j<std::min(_columnas, nuevaC); j++)
                    nuevoGrid[i][j] = _grid[i][j];

            for(int i=0; i<_filas; i++) delete[] _grid[i];
            delete[] _grid;

            _grid = nuevoGrid;
            _filas = nuevaF;
            _columnas = nuevaC;

            std::cout << "Grid redimensionado a " << _filas << "x" << _columnas << ". Datos copiados.\n";
        } catch(std::bad_alloc &e){
            std::cout << "Error al redimensionar: " << e.what() << std::endl;
            exit(1);
        }
    }

    // Agregar fuente
    void agregarFuente(T valor){
        if(_numFuentes >= _capacidadFuentes){
            T *nuevo = nullptr;
            try {
                nuevo = new T[_capacidadFuentes*2];
                for(int i=0; i<_capacidadFuentes; i++) nuevo[i] = _fuentes[i];
                for(int i=_capacidadFuentes; i<_capacidadFuentes*2; i++) nuevo[i] = 0;
                delete[] _fuentes;
                _fuentes = nuevo;
                _capacidadFuentes *= 2;
            } catch(std::bad_alloc &e){
                std::cout << "Error al expandir vector de fuentes: " << e.what() << std::endl;
                exit(1);
            }
        }
        _fuentes[_numFuentes++] = valor;
        std::cout << "Fuente agregada (" << valor << ") -> Éxito.\n";
    }

    // Simular un paso
    void simularPaso(){
        T **temp = nullptr;
        try {
            temp = new T*[_filas];
            for(int i=0; i<_filas; i++){
                temp[i] = new T[_columnas];
                for(int j=0; j<_columnas; j++) temp[i][j] = _grid[i][j];
            }
        } catch(std::bad_alloc &e){
            std::cout << "Error al crear matriz temporal: " << e.what() << std::endl;
            exit(1);
        }

        for(int i=1; i<_filas-1; i++){
            for(int j=1; j<_columnas-1; j++){
                temp[i][j] = (_grid[i-1][j] + _grid[i+1][j] + _grid[i][j-1] + _grid[i][j+1]) / 4.0;
            }
        }

        for(int i=0; i<_filas; i++)
            for(int j=0; j<_columnas; j++)
                _grid[i][j] = temp[i][j];

        for(int i=0; i<_filas; i++) delete[] temp[i];
        delete[] temp;

        std::cout << "Paso de simulación aplicado.\n";
    }

    // Mostrar grid
    void mostrarGrid(){
        for(int i=0; i<_filas; i++){
            std::cout << "| ";
            for(int j=0; j<_columnas; j++){
                std::cout << _grid[i][j] << " | ";
            }
            std::cout << "\n";
        }
    }

    // Colocar fuente manualmente
    void colocarFuenteEn(int fila, int col, int idxFuente){
        if(fila>=0 && fila<_filas && col>=0 && col<_columnas && idxFuente<_numFuentes)
            _grid[fila][col] = _fuentes[idxFuente];
    }
};

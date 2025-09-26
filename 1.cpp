#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "rwstring.hpp"

using namespace std;

//1. ESTRUCTURAS 

struct Producto {
    string sku;         
    string descripcion; 
    float costoFijo;
};
const int lsku = 10;
const int ldesc = 20;

string tipoProductoDesc(int tipo) {
    if(tipo==0) return "Electrónico";
    if(tipo==1) return "Mecánico";
    if(tipo==2) return "Mecatrónico";
    return "Desconocido";
}

struct Reparacion {
    string cliente;     
    int tipoProducto;   
    string sku;         
    float costoDirecto;
    float presupuestado;
};
const int lcli = 15;

//2. LECTURA DE ARCHIVOS BINARIOS Y CARGA DE DATOS

fstream& operator>>(fstream &fs, Producto &p) {
    p.sku = readstring(fs, lsku);
    p.descripcion = readstring(fs, ldesc);
    fs.read(reinterpret_cast<char*>(&p.costoFijo), sizeof(p.costoFijo));
    return fs;
}

fstream& operator>>(fstream &fs, Reparacion &r) {
    r.cliente = readstring(fs, lcli);
    fs.read(reinterpret_cast<char*>(&r.tipoProducto), sizeof(r.tipoProducto));
    r.sku = readstring(fs, lsku);
    fs.read(reinterpret_cast<char*>(&r.costoDirecto), sizeof(r.costoDirecto));
    fs.read(reinterpret_cast<char*>(&r.presupuestado), sizeof(r.presupuestado));
    return fs;
}

vector<Producto> leerProductos() {
    vector<Producto> productos;
    fstream archivo("productos.bin", ios::binary | ios::in);
    if (!archivo) {
        cerr << "No se pudo abrir productos.bin" << endl;
        return productos;
    }
    Producto p;
    while (archivo >> p) {
        if (!archivo) break;
        productos.push_back(p);
    }
    archivo.close();
    return productos;
}

vector<Reparacion> leerReparacion(){
    vector<Reparacion> reparaciones;
    fstream archivo("reparaciones.bin", ios::binary | ios::in);
    if(!archivo){
        cerr << "No se pudo abrir reparaciones.bin" << endl;
        return reparaciones;
    }
    Reparacion r;
    while(archivo >> r){
        if(!archivo) break;
        reparaciones.push_back(r);
    }
    archivo.close();
    return reparaciones;
}

template <typename T>
void ordenar(vector<T> &vec, int (*criterio)(const T&, const T&)) {
    int dim = vec.size();
    for (int i = 1; i < dim; i++) {
        T temp = vec[i];
        int j = i - 1;
        while (j >= 0 && criterio(vec[j], temp) > 0) {
            vec[j + 1] = vec[j];
            j--;
        }
        vec[j + 1] = temp;
    }
}

int criterio_reparacion(const Reparacion &a, const Reparacion &b) {
    if (a.cliente < b.cliente) return -1;
    if (a.cliente > b.cliente) return 1;

    if (a.tipoProducto < b.tipoProducto) return -1;
    if (a.tipoProducto > b.tipoProducto) return 1;

    if (a.sku < b.sku) return -1;
    if (a.sku > b.sku) return 1;

    return 0; 
}

int main() {
    vector<Producto> productos = leerProductos();
    vector<Reparacion> reparaciones = leerReparacion();

    string cliente;
    cout << "Ingrese el nombre del cliente (FIN para salir): ";
    while (getline(cin, cliente) && cliente != "FIN") {
        bool encontrado = false;
        float gananciaTotal = 0;
    
        for (int i = 0; i < reparaciones.size(); i++) {
            Reparacion r = reparaciones[i];

            if (r.cliente == cliente) {
                Producto* p = NULL;
                for (int j = 0; j < productos.size(); j++) {
                    if (productos[j].sku == r.sku) {
                        p = &productos[j];
                        break;
                    }
                }

                if (p == NULL) continue;

                float ganancia = r.presupuestado - (p->costoFijo + r.costoDirecto);
                gananciaTotal += ganancia;

                cout << "Cliente: " << r.cliente << "\n";
                cout << "Tipo: " << tipoProductoDesc(r.tipoProducto) << "\n";
                cout << "SKU: " << r.sku << "\n";
                cout << "Producto: " << p->descripcion << "\n";
                cout << "Costo Fijo: $" << p->costoFijo << "\n";
                cout << "Costo Directo: $" << r.costoDirecto << "\n";
                cout << "Presupuestado: $" << r.presupuestado << "\n";

                encontrado = true;
            }
        }

        if (encontrado)
            cout << "Ganancia total con " << cliente << ": $" << gananciaTotal << "\n";
        else
            cout << "No se encontraron reparaciones para " << cliente << "\n";

        cout << "\nIngrese el nombre del cliente (FIN para salir): ";
    }

    return 0;
}


    

    
    
    
    
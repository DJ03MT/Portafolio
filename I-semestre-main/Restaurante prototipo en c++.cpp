#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <limits>
#include <fstream>
#include <cctype>

using namespace std;

struct Comida {
    string nombre;
    string descripcion;
};

struct Menu {
    string nombre;
    string descripcion;
    double precio;
};

vector<Menu> menus;

struct Empleado {
    string nombre;
    double salario;
    string trabajo;
    int ventasMes;
};

bool esNumeroPositivo(double numero) {
    return numero >= 0;
}

const string PASSWORD_PERSONAL = "123456789";

void MenuPrincipal() {
    cout << "----- MENU PRINCIPAL -----\n";
    cout << "La Esquina del sabor\n";
    cout << "1. Acceder como Cliente\n";
    cout << "2. Acceder como Personal\n";
    cout << "Ingrese una opcion: ";
}

void menuPrincipalPersonal() {
    cout << "----- MENU PRINCIPAL (PERSONAL) -----" << endl;
    cout << "1. Ver Combos Disponibles" << endl;
    cout << "2. Agregar empleado" << endl;
    cout << "3. Dar de baja empleado" << endl;
    cout << "4. Ver empleados" << endl;
    cout << "5. Registrar entrada de empleado" << endl;
    cout << "6. Registrar salida de empleado" << endl;
    cout << "0. Volver al Menu Principal" << endl;
    cout << "Ingrese una opcion: ";
}

void MenuCliente() {
    cout << "----- MENU CLIENTE -----" << endl;
    for (int i = 0; i < menus.size(); i++) {
        cout << "Menu " << (i + 1) << ": " << menus[i].nombre << endl;
        cout << "Descripcion: " << menus[i].descripcion << endl;
        cout << "Precio: $" << menus[i].precio << endl;
        cout << endl;
    }
    cout << "0. Volver al Menu Principal" << endl;
}

string obtenerFechaHoraActual() {
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    stringstream ss;
    ss << put_time(localtime(&currentTime), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

bool esAlfabetic(string texto) {
    for (char c : texto) {
        if (!isalpha(c)) {
            return false;
        }
    }
    return true;
}

void mostrarMensajeError() {
    cout << "Opcion invalida. Intente nuevamente.\n";
    std::this_thread::sleep_for(chrono::seconds(3));
}

void agregarMenu(const string& nombre, const string& descripcion, double precio) {
    Menu menu = {nombre, descripcion, precio};
    menus.push_back(menu);
}

void mostrarMenusDisponibles() {
    cout << "----- MENUS DISPONIBLES -----\n";
    for (int i = 0; i < menus.size(); i++) {
        cout << "Menu " << (i + 1) << ": " << menus[i].nombre << endl;
        cout << "Descripcion: " << menus[i].descripcion << endl;
        cout << "Precio: $" << menus[i].precio << endl;
        cout << endl;
    }
}

void generarFactura(const string& nombreEmpleado, const string& nombreCliente, int menuSeleccionado) {
    // Obtener el menú seleccionado
    Menu menu = menus[menuSeleccionado - 1];

    // Generar factura con la hora actual
    string fechaHoraActual = obtenerFechaHoraActual();

    // Guardar la factura en un archivo de texto
    ofstream archivo("facturas.txt", ios::app);
    if (archivo.is_open()) {
        archivo << "----- FACTURA -----\n";
        archivo << "Fecha y hora: " << fechaHoraActual << "\n";
        archivo << "Empleado: " << nombreEmpleado << "\n";
        archivo << "Cliente: " << nombreCliente << "\n";
        archivo << "Menu: " << menu.nombre << "\n";
        archivo << "Descripcion: " << menu.descripcion << "\n";
        archivo << "Precio: $" << menu.precio << "\n";
        archivo << " Gracias por visitar La Esquina Del Sabor\n";
        archivo << "\n";
        archivo.close();
        

        cout << "Factura generada exitosamente. Se ha guardado en el archivo facturas.txt\n";
        cout << "-----------------------------------\n";
        cout << "Fecha y hora: " << fechaHoraActual << "\n";
        cout << "Empleado: " << nombreEmpleado << "\n";
        cout << "Cliente: " << nombreCliente << "\n";
        cout << "Menu: " << menu.nombre << "\n";
        cout << "Descripcion: " << menu.descripcion << "\n";
        cout << "Precio: $" << menu.precio << "\n";
        cout << "-----------------------------------\n";
    } else {
        cout << "No se pudo abrir el archivo para guardar la factura.\n";
    }
}
void verEmpleados(const vector<Empleado>& empleados) {
    cout << "----- EMPLEADOS -----" << endl;
    for (int i = 0; i < empleados.size(); i++) {
        cout << "Empleado " << (i + 1) << ":" << endl;
        cout << "Nombre: " << empleados[i].nombre << endl;
        cout << "Salario: $" << empleados[i].salario << endl;
        cout << "Trabajo: " << empleados[i].trabajo << endl;
        cout << "Ventas en el mes: " << empleados[i].ventasMes << endl;
        cout << endl;
    }

    if (empleados.empty()) {
        cout << "No hay empleados registrados." << endl;
    }
}
void agregarEmpleado(vector<Empleado>& empleados) {
    Empleado empleado;
    cout << "Ingrese el nombre del empleado: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, empleado.nombre);

    cout << "Ingrese el salario del empleado: ";
    cin >> empleado.salario;
    while (cin.fail() || !esNumeroPositivo(empleado.salario)) {
        cout << "Salario invalido. Ingrese un numero valido: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> empleado.salario;
    }

    cout << "Ingrese el trabajo del empleado: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, empleado.trabajo);

    cout << "Ingrese el numero de ventas del empleado en el mes: ";
    cin >> empleado.ventasMes;
    while (cin.fail() || !esNumeroPositivo(empleado.ventasMes)) {
        cout << "Numero de ventas invalido. Ingrese un numero valido: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> empleado.ventasMes;
    }

    empleados.push_back(empleado);
}

void darDeBajaEmpleado(vector<Empleado>& empleados) {
    cout << "----- EMPLEADOS -----" << endl;
    for (int i = 0; i < empleados.size(); i++) {
        cout << "Empleado " << (i + 1) << ":" << endl;
        cout << "Nombre: " << empleados[i].nombre << endl;
        cout << "Salario: $" << empleados[i].salario << endl;
        cout << "Trabajo: " << empleados[i].trabajo << endl;
        cout << "Ventas en el mes: " << empleados[i].ventasMes << endl;
        cout << endl;
    }

    if (empleados.empty()) {
        cout << "No hay empleados registrados." << endl;
        return;
    }

    int indice;
    cout << "Ingrese el número de empleado que desea dar de baja: ";
    cin >> indice;
    while (cin.fail() || indice < 1 || indice > empleados.size()) {
        mostrarMensajeError();
        cin.clear(); // Limpiar el estado de error del flujo de entrada
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorar caracteres inválidos en el flujo de entrada
        cout << "Ingrese el número de empleado que desea dar de baja: ";
        cin >> indice;
    }

    empleados.erase(empleados.begin() + indice - 1);
    cout << "Empleado dado de baja exitosamente." << endl;
}


void registrarEntradaSalidaEmpleado(const string& nombreEmpleado, bool esEntrada) {
    // Obtener la hora actual
    string fechaHoraActual = obtenerFechaHoraActual();

    // Guardar la entrada o salida del empleado en un archivo de texto
    ofstream archivo("registro_empleados.txt", ios::app);
    if (archivo.is_open()) {
        archivo << (esEntrada ? "ENTRADA" : "SALIDA") << " - " << nombreEmpleado << " - " << fechaHoraActual << "\n";
        archivo.close();

        cout << "Registro realizado exitosamente. Se ha guardado en el archivo registro_empleados.txt\n";
        cout << "-----------------------------------\n";
        cout << (esEntrada ? "ENTRADA" : "SALIDA") << " - " << nombreEmpleado << "\n";
        cout << "Fecha y hora: " << fechaHoraActual << "\n";
        cout << "-----------------------------------\n";
    } else {
        cout << "No se pudo abrir el archivo para guardar el registro.\n";
    }
}

int main() {
    ofstream archivo("empleados.txt", ios::app);

    if (!archivo) {
        cerr << "No se pudo abrir el archivo." << endl;
        return 1;
    }

    agregarMenu("El Toro", "Alitas picantes; papas fritas, queso amarillo derretido; Coca cola de 1.5L", 9.99);
    agregarMenu("Capital", "Churrasco; Ensalada de tomates, Salsas, Tajadas; Coca cola, Cerveza, Te (500ML)", 12.99);
    agregarMenu("La Redonda", "Pizza de 18`; Palitroque, Salsa; Coca Cola de 1.5L", 10.99);
    agregarMenu("VIP", "Carne Kobe; Chocolate derretido, Nachos, Postre; Vino, Whisky", 14.99);
    agregarMenu("La Vikinga", "Hamburguesa 3 carnes; Papas fritas, Salsa picante, Palitroque; Jugos, Coca Cola, Tona de 500ML", 8.99);

    int opcionMenu;
    string password;

    int opc_Principal, opc_Personal;
    string nombreEmpleado;
    int numeroCombo;
    string nombreCliente;
    int opcionCombo;
    vector<Empleado> empleados;

    do {
        MenuPrincipal();
        cin >> opc_Principal;

        if (cin.fail() || opc_Principal < 1 || opc_Principal > 2) {
            system("cls");
            cout << "Opcion invalida. Ingrese un numero valido del 1 al 2.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            MenuPrincipal();
            cin >> opc_Principal;
            continue;
        } else {
            system("cls");

            switch (opc_Principal) {
                case 1:
                    // Cliente
                    cout << "Ingrese el nombre del cliente: ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    getline(cin, nombreCliente);
                    while (!esAlfabetic(nombreCliente)) {
                        cout << "Nombre invalido. Ingrese un nombre valido: ";
                        getline(cin, nombreCliente);
                    }

                    MenuCliente();
                    cout << "Ingrese el numero del combo que desea: ";
                    cin >> numeroCombo;

                    while (cin.fail() || numeroCombo < 0 || numeroCombo > menus.size()) {
                        cout << "Número de combo invalido. Ingrese un número valido: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cin >> numeroCombo;
                    }

                    if (numeroCombo == 0) {
                        break;
                    }

                    // Generar factura
                    system("cls");
                    generarFactura(nombreEmpleado, nombreCliente, numeroCombo);
                    break;
                case 2:
                    // Personal
                    do {
                        menuPrincipalPersonal();
                        cin >> opc_Personal;

                        if (cin.fail() || opc_Personal < 0 || opc_Personal > 6) {
                            system("cls");
                            cout << "Opcion invalida. Ingrese un numero valido del 1 al 6.\n";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            menuPrincipalPersonal();
                            cin >> opc_Personal;
                        } else {
                            system("cls");

                            switch (opc_Personal) {
                                case 1:
                                    mostrarMenusDisponibles();
                                    break;
                                case 2:
                                     agregarEmpleado(empleados);
                                    break;
                                case 3:
                                    darDeBajaEmpleado(empleados);
                                    break;
                                case 4:
                                    verEmpleados(empleados);
                                    break;
                                case 5:
                                    // Registrar entrada de empleado
                                    cout << "Ingrese el nombre del empleado: ";
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    getline(cin, nombreEmpleado);
                                    while (!esAlfabetic(nombreEmpleado)) {
                                        cout << "Nombre invalido. Ingrese un nombre valido: ";
                                        getline(cin, nombreEmpleado);
                                    }
                                    registrarEntradaSalidaEmpleado(nombreEmpleado, true);
                                    break;
                                case 6:
                                    // Registrar salida de empleado
                                    cout << "Ingrese el nombre del empleado: ";
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    getline(cin, nombreEmpleado);
                                    while (!esAlfabetic(nombreEmpleado)) {
                                        cout << "Nombre invalido. Ingrese un nombre valido: ";
                                        getline(cin, nombreEmpleado);
                                    }
                                    registrarEntradaSalidaEmpleado(nombreEmpleado, false);
                                    break;
                                case 0:
                                    // Volver al Menu Principal
                                    cout << " Volviendo al Menu Principal\n";
                                    break;
                            }
                        }
                    } while (opc_Personal != 0);
                    break;
            }
        }

    } while (opc_Principal != 0);

    return 0;
}

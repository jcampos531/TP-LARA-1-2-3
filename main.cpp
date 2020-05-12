
#include <iostream>
using namespace std;
#include <clocale>
#include <ctype.h>
#include <ctime>
#include "includes/rlutil.h"
using namespace rlutil;

#include "includes/func.h"
#include "includes/platos.h"
#include "includes/clientes.h"
#include "includes/pedidos.h"

int main() {
    int opc;

    setlocale(LC_CTYPE, "Spanish");
    saveDefaultColor();

    if (verificarArchivo(ARCHIVO_PLATOS) == -1) return 0;
    if (verificarArchivo(ARCHIVO_CLIENTES) == -1) return 0;
    if (verificarArchivo(ARCHIVO_PEDIDOS) == -1) return 0;

    while (true){
        opc = mostrarMenu(MENU_PRINCIPAL, 6, false);

        switch (opc){
            case 1:
                do{ /**MENU PLATOS**/
                    opc = mostrarMenu(MENU_PLATOS, 7);
                    switch (opc){
                        case 1:/**NUEVO PLATO**/
                            if (crearPlato()) mensajePlato(PLATO_GUARDADO);
                        break;
                        case 2:/**MODIFICAR PLATO**/
                            if (modificarPlato()) mensajePlato(PLATO_MODIFICADO);
                        break;
                        case 3:/**LISTAR PLATO**/
                            listarPlato_x_ID();
                        break;
                        case 4:/**LISTAR PLATOS POR RESTAURANTE**/
                            listarPlatos_x_Rest();
                        break;
                        case 5:/**LISTAR TODOS**/
                            listarTodosPlatos();
                        break;
                        case 6: /**ELIMINAR PLATO**/
                            if (eliminarPlato()) mensajePlato(PLATO_ELIMINADO);
                        break;
                        case 7:
                        break;
                    }
                } while(opc != 7);
            break;
            case 2:
                do{ /**MENU CLIENTES**/
                    opc = mostrarMenu(MENU_CLIENTES, 6);
                    switch (opc){
                        case 1: /**NUEVO CLIENTE**/
                            if (crearCliente()) mensajeCliente(CLIENTE_GUARDADO);
                        break;
                        case 2: /**MODIFICAR CLIENTE**/
                            if (modificarCliente()) mensajeCliente(CLIENTE_MODIFICADO);
                        break;
                        case 3: /**LISTAR CLIENTE POR ID**/
                            listarCliente_x_ID();
                        break;
                        case 4: /**LISTAR TODOS**/
                            listarTodosClientes();
                        break;
                        case 5: /**ELIMINAR CLIENTE**/
                            if (eliminarCliente()) mensajeCliente(CLIENTE_ELIMINADO);
                        break;
                        case 6: /**VOLVER**/
                        break;
                    }
                } while (opc != 6);
            break;
            case 3:
                do{ /**MENU CLIENTES**/
                    opc = mostrarMenu(MENU_PEDIDOS, 5);
                    switch (opc){
                        case 1: /**NUEVO PEDIDO**/
                            if (crearPedido()) mensajePedido(PEDIDO_GUARDADO);
                        break;
                        case 2: /**MODIFICAR PEDIDO**/
                            if (modificarPedido()) mensajePedido(PEDIDO_MODIFICADO);
                        break;
                        case 3: /**LISTAR PEDIDO POR ID**/
                            listarPedido_x_ID();
                        break;
                        case 4: /**LISTAR TODOS LOS PEDIDOS**/
                            listarTodosPedidos();
                        break;
                        case 5: /**VOLVER**/
                        break;
                    }
                } while (opc != 5);
            break;
            case 5:
                char c;
                do{ /**MENU CONFIGURACION**/
                    opc = mostrarMenu(MENU_CONFIGURACION, 3);
                    switch (opc){
                        case 1: /**REALIZAR COPIA DE SEGURIDAD**/
                            showcursor();
                            cout << endl << "¿Desea realizar una copia de seguridad de los ficheros? (y/n): ";
                            fflush(stdin);
                            cin >> c;
                            if (c == 'y' || c == 'Y'){
                                if (copiarFicheroPlatos(ARCHIVO_PLATOS, BACKUP_PLATOS)) mensajeFile(ARCHIVO_PLATOS, BACKUP_GENERADO);
                                if (copiarFicheroClientes(ARCHIVO_CLIENTES, BACKUP_CLIENTES)) mensajeFile(ARCHIVO_CLIENTES, BACKUP_GENERADO);
                                if (copiarFicheroPedidos(ARCHIVO_PEDIDOS, BACKUP_PEDIDOS)) mensajeFile(ARCHIVO_PEDIDOS, BACKUP_GENERADO);
                            }
                        break;
                        case 2: /**RESTAURAR COPIA DE SEGURIDAD**/
                            showcursor();
                            cout << endl << "¿Desea restaurar la copia de seguridad de los ficheros? (y/n): ";
                            fflush(stdin);
                            cin >> c;
                            if (c == 'y' || c == 'Y'){
                                if (copiarFicheroPlatos(BACKUP_PLATOS, ARCHIVO_PLATOS)) mensajeFile(BACKUP_PLATOS, BACKUP_RECUPERADO);
                                if (copiarFicheroClientes(BACKUP_CLIENTES, ARCHIVO_CLIENTES)) mensajeFile(BACKUP_CLIENTES, BACKUP_RECUPERADO);
                                if (copiarFicheroPedidos(BACKUP_PEDIDOS, ARCHIVO_PEDIDOS)) mensajeFile(BACKUP_PEDIDOS, BACKUP_RECUPERADO);
                            }
                        break;
                        case 3: /**VOLVER**/
                        break;
                    }
                }while (opc != 3);
            break;
            case 6:
                return 0; /**SALIR DEL PROGRAMA**/
            break;
        }
    }
}





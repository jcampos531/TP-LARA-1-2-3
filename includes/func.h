#ifndef FUNC_H_INCLUDED
#define FUNC_H_INCLUDED
//ESTRUCTURAS DE USO COMUN
struct Fecha{
    int dia, mes, anio;
};

//MENSAJES DE NOTIFICACION AL USUARIO

const int BACKUP_GENERADO = 0;
const int BACKUP_RECUPERADO = 1;

const char SUCCES_FILE[][80] = {
    {"Se generó con éxito el Backup del archivo: "},
    {"Se recuperó con éxito el archivo: "}
};

//Errores de FILE
 const int FILE_OPEN_ERROR = 0;
 const int FILE_READ_ERROR = 1;
 const int FILE_WRITE_ERROR = 2;
 const int MEM_INSUF_ERROR = 3;
 const int ARCHIVO_VACIO_ERROR = 4;

 const char ERROR_FILE[][50]{
    {"Error al abrir el archivo: "},
    {"Error al leer del archivo: "},
    {"Error al escribir en el archivo: "},
    {"Memoria insuficiente para cargar el archivo: "},
    {"No se encontraron registros en el archivo: "}
 };

//MENUES
 const char  MENU_PRINCIPAL[][30] = {
    {" MENÚ PRINCIPAL "},
    {" -------------- "},
    {"-> PLATOS "},
    {"-> CLIENTES "},
    {"-> PEDIDOS "},
    {"-> REPORTES "},
    {"-> CONFIGURACIÓN "},
    {"-> SALIR "}
};
 const char  MENU_PLATOS[][30] = {
    {" PLATOS "},
    {" -------------- "},
    {"-> NUEVO PLATO "},
    {"-> MODIFICAR PLATO "},
    {"-> LISTAR PLATO POR ID "},
    {"-> PLATOS POR RESTAURANT "},
    {"-> LISTAR TODOS LOS PLATOS "},
    {"-> ELIMINAR PLATO "},
    {"-> VOLVER AL MENÚ PRINCIPAL"}
};
const char MENU_CLIENTES[][30] = {
    {" MENÚ CLIENTES "},
    {" -------------- "},
    {"-> NUEVO CLIENTE "},
    {"-> MODIFICAR CLIENTE "},
    {"-> LISTAR CLIENTE POR ID "},
    {"-> LISTAR TODOS LOS CLIENTES "},
    {"-> ELIMINAR CLIENTE "},
    {"-> VOLVER AL MENÚ PRINCIPAL "}
};
const char MENU_PEDIDOS[][30] = {
    {" MENÚ PEDIDOS "},
    {" -------------- "},
    {"-> NUEVO PEDIDO "},
    {"-> MODIFICAR PEDIDO "},
    {"-> LISTAR PEDIDO POR ID "},
    {"-> LISTAR TODOS LOS PEDIDOS "},
    {"-> VOLVER AL MENÚ PRINCIPAL "}
};
const char MENU_CONFIGURACION[][30] = {
    {" MENÚ CONFIGURACION "},
    {" ------------------ "},
    {"-> REALIZAR COPIA DE SEG. "},
    {"-> RESTAURAR COPIA DE SEG. "},
    {"-> VOLVER AL MENÚ PRINCIPAL "}
};

//ARCHIVOS
const char ARCHIVO_PLATOS [] = "ARCHIVOS/platos.dat";
const char ARCHIVO_CLIENTES [] = "ARCHIVOS/clientes.dat";
const char ARCHIVO_PEDIDOS [] = "ARCHIVOS/pedidos.dat";
const char BACKUP_PLATOS [] = "BACKUP/platos.bkp";
const char BACKUP_CLIENTES [] = "BACKUP/clientes.bkp";
const char BACKUP_PEDIDOS [] = "BACKUP/pedidos.bkp";

//PROTOTIPOS
int mostrarMenu(char (*)[30], int, bool);
void errorFile(const char [][50], const char*, int);
void mensajeFile(const char, int);
int verificarArchivo(const char *);
int contarRegistros(const char *, size_t);
bool verificarFecha(int, int, int);

//DEFINICIONES
int verificarArchivo(const char *archivo){
    /** Verifica la existencia de fichero en la ruta indicada.
        ARGUMENTOS: const char *archivo --> la ruta del fichero.
        RETORNO: Un valor entero -1 si no se pudo abrir el archivo, 1 si el archivo se pudo abrir */
    FILE *pArchivo;

    pArchivo = fopen(archivo, "rb");
    if (pArchivo == NULL){
        setBackgroundColor(RED);
        cout << endl << "Falta el archivo '" << archivo <<"'." << "Contactese con el administrador" << endl;
        resetColor();
        return -1;
    }
    fclose(pArchivo);
    return 1;
}
int mostrarMenu(const char (*menu)[30], int cantOpc, bool esc = true){
    /** Carga un menú en pantalla.
        ARGUMENTOS: const char menu[][30] --> El menú a cargar
                    int cantOpc --> La cantidad de opciones seleccionables del menú
                    bool esc --> Indica si el menú acepta la tecla ESC como retorno al menú anterior qu elo llamó
        RETORNO: Un valor entero que indica la opción seleccionada por el usuario */

    int opc = 1, i;
    int teclaPulsada;

    hidecursor();

    do{
        cls();
        for (i=0;i<cantOpc+2;i++){
            if (i == opc + 1){ //Opción que tiene el foco
              setBackgroundColor(GREY);
              setColor(BLACK);
              cout << menu[i] << endl;
              resetColor();
            }
            else cout << menu[i] << endl;
        }
        if (esc == true) cout << endl << endl << "   --->    Esc: Volver   <---        ";

        teclaPulsada = getkey();

        switch (teclaPulsada){
            case KEY_DOWN:
                if(opc < cantOpc)opc++;
            break;
            case KEY_UP:
                if(opc > 1)opc--;
            break;
            case KEY_ESCAPE:
                if(esc == true) return cantOpc; /** Si se permite el Modo Escape, entonces
                                la tecla ESC retorna al menú anterior (cantOpc = 'ultima opción del menú')*/
            break;
        };
    } while (teclaPulsada != KEY_ENTER);

    return opc;
}
void errorFile(const char *fileName ,int nError){
    /** Muestra por pantalla el mensaje de error que se generó al trabajar con un fichero determinado.
        ARGUMENTOS: const char *fileName --> la ruta del fichero.
                    int nError --> Un valor que indica el número de error.
        RETORNO: Void */
    setBackgroundColor(RED);
    cout << endl << ERROR_FILE[nError] << " '" << fileName << "'";
    resetColor();
    getch();
}

void mensajeFile(const char *fileName ,int nMensaje){
    /** Muestra por pantalla un mensaje informativo relativo a una operación en un fichero determinado en *fileName.
        ARGUMENTOS: const char *fileName --> la ruta del fichero.
                    int nMensaje --> Un valor que indica el número de mensaje.
        RETORNO: Void */
    setBackgroundColor(GREEN);
    cout << endl << SUCCES_FILE[nMensaje] << " '" << fileName << "'";
    resetColor();
    getch();
}

int contarRegistros(const char *fileName, size_t tamReg){
    /** Cuenta los registros de una determinada estructura dentro de un fichero.
        ARGUMENTOS: const char *fileName --> la ruta del fichero.
                    size_t tamReg --> Tamaño en bytes del fichero.
        RETORNO: Un entero con la cantidad de registros */
    FILE *pArchivo;
    int tamFile = 0;

    pArchivo = fopen(fileName, "rb");
    if (pArchivo){
        fseek(pArchivo, 0, 2);
        tamFile = ftell(pArchivo);
        fclose(pArchivo);
    }
    fclose(pArchivo);
    return (tamFile/tamReg);
}

bool verificarFecha(int dia, int mes, int anio){
    /** Verifica que la fecha exista dentro del calendario.
        ARGUMENTOS: 3 enteros con el dia, mes y anio.
        RETORNO: bool  */
    bool bisiesto = false;

    if (mes > 12 || mes < 1) return false;
    if (dia < 1 || dia > 31) return false;

    if (mes == 4 || mes == 6 || mes == 9 ||mes == 11){
        if (dia > 30) return false;
    }
    if (mes == 2){
        if (anio%400 == 0) bisiesto = true;
        if (anio%4 == 0 && anio%100 != 0) bisiesto = true;
        if (bisiesto){
            if (dia > 29) return false;
        } else {
            if (dia > 28) return false;
        }
    }

    return true;
}



#endif // FUNC_H_INCLUDED

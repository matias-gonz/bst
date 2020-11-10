Matias Ignacio Gonzalez
TDA ARBOL BINARIO BUSQUEDA

Se presentan:
    >> Implementacion de TDA Arbol
    >> Programa de testeo para el TDA Arbol presentado
    >> Biblioteca de testeo

Programa de testeo para el TDA Arbol:
    En el archivo .zip tambien se incluye un programa para testear la implementacion del TDA. Al compilar y ejecutar el programa se ejecutaran una serie de pruebas
sobre el TDA, testeando cada funcion por separado y testeando el funcionamiento en casos bordes. Se brinda una breve descripcion de cada conjunto de pruebas
por encima de cada funcion principal. En cada funcion especifica hay diferentes mini-tests, cada uno con un nombre descriptivo.
    Los resultados de las pruebas seran impresos por pantalla con un formato vistoso y facil de leer.
    Codigo de colores:
        Verde = Test exitoso
        Rojo = Test fallido
        Amarillo = Advertencia


POSIBLES FALLAS DEL TESTEO:
Si el usuario no tiene permisos para asignar memoria en el heap el programa se comportara de forma indeterminada.
Si la memoria disponible no es suficiente el programa se comportara de forma indeterminada.
El programa de testeo no esta disenado para la modificacion intensiva de sus pruebas ya que se verifica de forma muy "artesanal" algunos tests, en especial
los tests de caja blanca.


Utilizacion del TDA:
    Para utilizar las funciones del TDA basta con agregar en su codigo la linea:
        #include "abb.h"
    Y al compilar el codigo se encuentren en el mismo directorio los archivos abb.h y abb.c

Compilacion - Ejecucion Testeo:
    Para compilar el programa de testeo debe escribirse en la terminal (mientras este seleccionado el directorio donde estan los archivos):
           gcc *.c -o "nombre_programa"

    El nombre del programa puede ser a eleccion. Si no tiene preferencia en el nombre puede compilarlo con el siguiente comando:
           gcc *.c
    En este caso el programa se llamara por predeterminado "a.out"

    Para ejecutar el programa es necesario estar en el directorio donde estan los archivos en la terminal e introducir lo siguiente:
            ./"nombre_programa"

Ejecucion con Valgrind:
    Valgrind es una herramienta para comprobar que no existan leaks de memoria en el programa.
    Si se desea, se puede ejecutar el programa con valgrind de la sigueinte manera:
            valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./"nombre_programa"


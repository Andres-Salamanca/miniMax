#include <algorithm>
#include <iostream>
#include <limits>
#include <ostream>
#include <utility>
using namespace std;
// se puede solo usar una funcion en vez de tener aux y la normal
// toca mejorar la funcion heuristica
const int  profundidad= 2;
// Función para imprimir el tablero
void imprimirTablero(char tablero[3][3]) {
    cout << "  0 1 2" << endl;
    for (int i = 0; i < 3; i++) {
        cout << i << " ";
        for (int j = 0; j < 3; j++) {
            cout << tablero[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}




// Función para verificar si alguien ha ganado
bool verificarGanador(char tablero[3][3], char jugador) {
    // Verificar filas y columnas
    for (int i = 0; i < 3; i++) {
        if (tablero[i][0] == jugador && tablero[i][1] == jugador && tablero[i][2] == jugador) {
            return true; // Ganador en la fila i
        }
        if (tablero[0][i] == jugador && tablero[1][i] == jugador && tablero[2][i] == jugador) {
            return true; // Ganador en la columna i
        }
    }

    // Verificar diagonales
    if (tablero[0][0] == jugador && tablero[1][1] == jugador && tablero[2][2] == jugador) {
        return true; // Ganador en diagonal principal
    }
    if (tablero[0][2] == jugador && tablero[1][1] == jugador && tablero[2][0] == jugador) {
        return true; // Ganador en diagonal secundaria
    }

    return false; // Nadie ha ganado todavía
}
//verificar estas funciones para mejorar la funcion euristica
int evalLine(int countX, int countO) {
    if (countX == 3) {
        return 100;
    } else if (countO == 3) {
        return -100;
    } else if (countX == 2 && countO == 0) {
        return 10;
    } else if (countX == 0 && countO == 2) {
        return -10;
    } else {
        return 0;
    }
}
int funcionheuristica(char tablero[][3],char jugador,int depth){
 
  if (verificarGanador(tablero, 'X')) {
        return 100 - depth;
    } else if (verificarGanador(tablero, 'O')) {
        return depth - 100;
    }

    // Evalúa el tablero según la cantidad de símbolos del jugador en filas, columnas y diagonales.
    int score = 0;

    //contar columnas y filas 
    for (int i = 0; i < 3; i++) {
        int rowCountX = 0, colCountX = 0, rowCountO = 0, colCountO = 0;
        for (int j = 0; j < 3; j++) {
            if (tablero[i][j] == 'X') {
                rowCountX++;
            } else if (tablero[i][j] == 'O') {
                rowCountO++;
            }

            if (tablero[j][i] == 'X') {
                colCountX++;
            } else if (tablero[j][i] == 'O') {
                colCountO++;
            }
        }

        score += evalLine(rowCountX, rowCountO);
        score += evalLine(colCountX, colCountO);
    }

    //contar diagonales 
    int diagCountX1 = 0, diagCountO1 = 0, diagCountX2 = 0, diagCountO2 = 0;
    for (int i = 0; i < 3; i++) {
        if (tablero[i][i] == 'X') {
            diagCountX1++;
        } else if (tablero[i][i] == 'O') {
            diagCountO1++;
        }

        if (tablero[i][2 - i] == 'X') {
            diagCountX2++;
        } else if (tablero[i][2 - i] == 'O') {
            diagCountO2++;
        }
    }

    score += evalLine(diagCountX1, diagCountO1);
    score += evalLine(diagCountX2, diagCountO2);

    if (jugador == 'O') {
        return -score;
    }

    return score;


}
//HASTA AQUI TOCA MEJOAR



int minmaxAux(char tablero[][3] , int depth, bool maximizar, std::pair<int,int> position,char jugador){


  if(depth == 0 || verificarGanador(tablero, jugador)){
    //std::cout<<"la matriz en caso recursivo es"<<"\n";
    // imprimirTablero(tablero);
    return funcionheuristica(tablero, jugador, depth); 
  }


  if (maximizar){
    int mejorHeuri = std::numeric_limits<int>::min();
    
    for(int fila = 0;fila < 3;fila++){


      for(int columna = 0 ;columna<3;columna++){
        if(tablero[fila][columna] == ' '){

          tablero[fila][columna] = 'X';
          int heuri = minmaxAux(tablero,depth-1,false,pair(fila,columna),'X');
          mejorHeuri = std::max(mejorHeuri,heuri);
          tablero[fila][columna] = ' ';
        }

      }

    }

      return mejorHeuri;

  }
  else {

    int mejorHeuri = std::numeric_limits<int>::max();

    for(int fila = 0;fila < 3;fila++){


      for(int columna = 0 ;columna<3;columna++){

        if(tablero[fila][columna] == ' '){

          tablero[fila][columna] = 'O';
          int heuri = minmaxAux(tablero,depth-1,true,pair(fila,columna),'O');
          mejorHeuri = std::min(mejorHeuri,heuri);
          tablero[fila][columna] = ' ';

        }

      }

    }

    return  mejorHeuri;
  }

  
}

pair<int, int> mejorJugada(char tablero[3][3], char jugador) {
    int mejorHeuristica = (jugador == 'X') ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    pair<int, int> mejorMovimiento = {-1, -1};

    for (int fila = 0; fila < 3; fila++) {
        for (int columna = 0; columna < 3; columna++) {
            if (tablero[fila][columna] == ' ') {
                tablero[fila][columna] = jugador;
                int heuristica = minmaxAux(tablero, profundidad - 1, (jugador == 'X' ? false : true), pair(fila, columna), jugador);

                if ((jugador == 'X' && heuristica > mejorHeuristica) || (jugador == 'O' && heuristica < mejorHeuristica)) {
                    mejorHeuristica = heuristica;
                    mejorMovimiento = {fila, columna};
                }

                tablero[fila][columna] = ' ';
            }
        }
    }

    return mejorMovimiento;
}

// Función principal del juego
int main() {
    char tablero[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
    char jugadorActual = 'X';
    int fila, columna;

    cout << "¡Bienvenido al juego de Triqui!" << endl;

    for (int turnos = 0; turnos < 9; turnos++) {
      // Imprimir tablero
      imprimirTablero(tablero);

      if(turnos %2 ==0){
                    // Computer's turn (X)
            cout << "Turno del jugador " << jugadorActual << "\n";
            pair<int, int> mejorJugad = mejorJugada(tablero, jugadorActual);
            fila = mejorJugad.first;
            columna = mejorJugad.second;
      }
      else{
        // Solicitar movimiento al jugador
        cout << "Turno del jugador " << jugadorActual << ". Ingresa fila y columna (ejemplo: 1 2): ";
        cin >> fila >> columna;


      }
                // Verificar si la posición está ocupada
        if (tablero[fila][columna] != ' ') {
            cout << "¡Posición ocupada! Intenta de nuevo." << endl;
            turnos--; // Repetir el turno
            continue;
        }

        // Realizar el movimiento
        tablero[fila][columna] = jugadorActual;

        // Verificar si el jugador actual ha ganado
        if (verificarGanador(tablero, jugadorActual)) {
            imprimirTablero(tablero);
            cout << "¡El jugador " << jugadorActual << " ha ganado!" << endl;
            return 0; // Terminar el juego
        }

        // Cambiar al siguiente jugador
        jugadorActual = (jugadorActual == 'X') ? 'O' : 'X';
    }

    // Si llegamos aquí y no hay ganador, es un empate
    imprimirTablero(tablero);
    cout << "¡Es un empate!" << endl;

    return 0;
}


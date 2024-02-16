#include <algorithm>
#include <iostream>
#include <limits>
#include <utility>
using namespace std;

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

int funcionheuristica(char tablero[][3],char jugador,int depth){
  if(verificarGanador(tablero, 'X')){
    return 100 + depth;

  }else if (verificarGanador(tablero, 'O')) {
    return depth-100;
  }
  else {
   // Evaluate the board based on the number of player's symbols in rows, columns, and diagonals
        int score = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (tablero[i][j] == jugador) {
                    score += 1;
                } else if (tablero[i][j] != ' ') {
                    score -= 1;
                }
            }
        }
        if(jugador == '0'){
          return -1*score;
    }
        return score;
  }
}




int minmaxAux(char tablero[][3] , int depth, bool maximizar, std::pair<int,int> position,char jugador){


  if(depth == 0 || verificarGanador(tablero, jugador)){
    return funcionheuristica(tablero, jugador, depth); 
  }


  if (maximizar){
    int mejorHeuri = std::numeric_limits<int>::min();
    
    for(int fila = 0;fila < 3;fila++){


      for(int columna = 0 ;columna<3;columna++){
        if(tablero[fila][columna] == ' '){

          tablero[fila][columna] = 'O';
          int heuri = minmaxAux(tablero,depth-1,false,pair(fila,columna),'O');
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

          tablero[fila][columna] = 'X';
          int heuri = minmaxAux(tablero,depth-1,true,pair(fila,columna),'X');
          mejorHeuri = std::min(mejorHeuri,heuri);
          tablero[fila][columna] = ' ';

        }

      }

    }

    return  mejorHeuri;
  }

  
}


std::pair<int, int> miniMax(char tablero[3][3] , int depth, bool maximizar){

  std::pair<int,int> bestMove = pair(-1,-1); 
  int mejorHeuristica = std::numeric_limits<int>::min();

  //encontrar jugadas posibles
  for(int fila = 0 ; fila < 3 ;fila ++){

    for(int columna = 0;columna<3;columna++){
      if(tablero[fila][columna]== ' '){

        tablero[fila][columna] = 'X';
        //maybe cambiar el depth -1 por ahora no se
        int pHeuristica = minmaxAux(tablero,depth-1,maximizar,pair(fila,columna),'X');
        tablero[fila][columna] = ' ';

        if(pHeuristica>mejorHeuristica){
          mejorHeuristica = pHeuristica;
          bestMove.first = fila;
          bestMove.second = columna;
        }

      }
    }

  }

 
  return bestMove;
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
        
        pair mejorJugada = miniMax(tablero,2,true);
        fila = mejorJugada.first;
        columna = mejorJugada.second;
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


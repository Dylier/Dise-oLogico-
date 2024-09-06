#include <Arduino.h>

#define MAX_NUMEROS 100

// Clase Numero simplificada, integrando la funcionalidad de la clase Digito.
class Numero {
private:
  String valorBinario;

public:
  Numero() {
    valorBinario = "";
  }

  void setValor(String binario) {
    valorBinario = binario;
  }

  String getValor() const {
    return valorBinario;
  }

  int obtenerLongitud() const {
    return valorBinario.length();
  }

  double binarioaDecimal() const {
    double decimal = 0;
    int len = valorBinario.length();
    for (int i = 0; i < len; i++) {
      if (valorBinario[len - i - 1] == '1') {
        decimal += pow(2, i);
      }
    }
    return decimal;
  }
};

// Clase para manejar la lista de números binarios.
class ListaNumeros {
private:
  Numero numeros[MAX_NUMEROS];
  int conteo;

public:
  ListaNumeros() {
    conteo = 0;
  }

  void agregarNumero(String binario) {
    if (conteo < MAX_NUMEROS) {
      numeros[conteo].setValor(binario);
      conteo++;
    } else {
      Serial.println("Error: Lista llena.");
    }
  }

  void imprimirLista() const {
    for (int i = 0; i < conteo; i++) {
      Serial.print(i);
      Serial.print(": ");
      Serial.println(numeros[i].getValor());
    }
  }

  Numero obtenerNumeroPorIndice(int indice) const {
    if (indice >= 0 && indice < conteo) {
      return numeros[indice];
    } else {
      Serial.println("Error: Índice fuera de rango.");
      return Numero();
    }
  }

  Numero encontrarNumeroMasLargo() const {
    Numero numMasLargo;
    for (int i = 0; i < conteo; i++) {
      if (numeros[i].obtenerLongitud() > numMasLargo.obtenerLongitud()) {
        numMasLargo = numeros[i];
      }
    }
    return numMasLargo;
  }

  // Método para agregar desde un vector de datos iniciales
  void agregarDesdeVector(const char* vector[], int tamano) {
    for (int i = 0; i < tamano; i++) {
      agregarNumero(String(vector[i])); // Convertir char* a String y agregarlo
    }
  }
};

// Variables globales.
ListaNumeros listaGlobal;

// Vector de datos iniciales
const char* lista_datos[] = {"1010", "1100", "1111", "1001"};

// Funciones
const int MAX_BITS = 50;

String suma_binaria(String sublista[], int n) {
  int max_len = 0;
  
  for (int i = 0; i < n; i++) {
    if (sublista[i].length() > max_len) {
      max_len = sublista[i].length();
    }
  }

  char resultado[MAX_BITS] = {0}; 
  int acarreo = 0;

  for (int posicion = 0; posicion < max_len; posicion++) {
    int suma = acarreo;

    for (int i = 0; i < n; i++) {
      if (posicion < sublista[i].length()) {
        suma += sublista[i].charAt(sublista[i].length() - 1 - posicion) - '0'; 
      }
    }

    int bit_resultante = suma % 2;
    acarreo = suma / 2;

    if (posicion < MAX_BITS) {
      resultado[posicion] = (char)(bit_resultante + '0');
    }
  }

  int pos = max_len;
  while (acarreo > 0) {
    if (pos < MAX_BITS) {
      resultado[pos++] = (char)((acarreo % 2) + '0');
      acarreo /= 2;
    } else {
      break;
    }
  }

  String resultado_str = "";
  for (int i = pos - 1; i >= 0; i--) {
    resultado_str += resultado[i];
  }
  
  Serial.print("\nEl resultado es:");   
  Serial.print(resultado_str);
  Serial.print("\n");

  return resultado_str;
}

void mostrar_menu() {
  Serial.println("\nMenú de opciones:");
  Serial.println("1. Guardar datos binarios");
  Serial.println("2. Convertir binario a decimal");
  Serial.println("3. Suma de binarios");
  Serial.println("4. Mostrar número más largo");
  Serial.println("5. Mostrar todos los números");
  Serial.println("0. Salir");
  Serial.println("Ingrese su opción:");
}

void setup() {
  Serial.begin(9600);

  // Llamada para agregar todos los datos del vector a la lista de números al iniciar el programa.
  listaGlobal.agregarDesdeVector(lista_datos, sizeof(lista_datos) / sizeof(lista_datos[0]));
  
  mostrar_menu();
}

void loop() {
  if (Serial.available()) {
    String opcion = Serial.readStringUntil('\n');
    opcion.trim();

    if (opcion == "1") {
      Serial.println("Ingrese el número binario:");
      while (!Serial.available());
      String valor_binario = Serial.readStringUntil('\n');
      valor_binario.trim();
      listaGlobal.agregarNumero(valor_binario);

    } else if (opcion == "2") {
      Serial.println("Ingrese el número binario:");
      while (!Serial.available());
      String num_binario = Serial.readStringUntil('\n');
      num_binario.trim();

      Numero numero;
      numero.setValor(num_binario);
      double decimal = numero.binarioaDecimal();
      Serial.print("El número decimal es: ");
      Serial.println(decimal);

    } else if (opcion == "3") {
      listaGlobal.imprimirLista();
      Serial.println("Ingrese los índices de los números que desea sumar separados por coma (ejemplo: 0,2,3):");
      while (!Serial.available());
      String indices = Serial.readStringUntil('\n');
      indices.trim();

      String sublista[MAX_BITS];
      int n = 0;
      int pos = 0;
      while ((pos = indices.indexOf(',')) != -1) {
        int index = indices.substring(0, pos).toInt();
        sublista[n] = listaGlobal.obtenerNumeroPorIndice(index).getValor();
        indices = indices.substring(pos + 1);
        n++;
      }
      if (indices.length() > 0) {
        int index = indices.toInt();
        sublista[n] = listaGlobal.obtenerNumeroPorIndice(index).getValor();
        n++;
      }

      String resultado = suma_binaria(sublista, n);
      Serial.print("\nEl resultado de la suma binaria es: ");
      Serial.println(resultado);

    } else if (opcion == "4") {
      Numero numMasLargo = listaGlobal.encontrarNumeroMasLargo();
      Serial.print("El número más largo es: ");
      Serial.println(numMasLargo.getValor());

    } else if (opcion == "5") {
      listaGlobal.imprimirLista();

    } else if (opcion == "0") {
      Serial.println("¡Hasta luego!");
      while (true);  // Detener el programa.
    } else {
      Serial.println("Opción no válida. Intente nuevamente.");
    }
    mostrar_menu();
  }
}

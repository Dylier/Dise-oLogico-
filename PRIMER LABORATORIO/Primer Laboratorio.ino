#include <Arduino.h>

// Define el pin del botón
const int botonPin = 10;

// Variables globales para manejar el estado del botón y la opción actual
int opcionActual = 0;

// Definir la lista de datos inicial
String lista_datos[] = {
  "111111111111111111001111",
  "111111111111111111111000",
  "111111111111111111111100",
  "111111111111111000111111",
  "111111111111111111110111",
  "1111111111110000",
  "1111111111111110",
  "1111111111000111",
  "1111111111111000",
  "11111000",
  "11111100",
  "11111110"
};

String prueba1[] = {
  "111111111111111111001111",
  "111111111111111111111000",
  "111111111111111111111100",
  "111111111111111000111111",
  "111111111111111111110111",
  "1111111111110000",
  "1111111111111110",
  "1111111111000111",
  "1111111111111000",
  "11111000",
  "11111100",
  "11111110"
};

String prueba2[] = {
  "111111111111111000111111",
  "111111111111111111110111",
  "1111111111110000",
  "1111111111111110",
  "11111000",
  "11111100",
  "11111110"
};

String prueba3[] = {
  "111111111111111000111111",
  "111111111111111111110111",
  "1111111111000111",
  "1111111111111000",
  "11111100",
  "11111110"
};

String prueba4[] = {
  "111111111111111111001111",
  "111111111111111111111000",
  "111111111111111111111100",
  "111111111111111000111111",
  "111111111111111111110111",
  "1111111111110000",
  "11111000"
};



void guardar_datos(String nuevo_dato) {
  int index = -1;
  
  for (int i = 12; i < 20; i++) {  // Buscar el primer espacio vacío
    if (lista_datos[i] == "") {
      index = i;
      break;
    }
  }

  if (index != -1) {
    lista_datos[index] = nuevo_dato;
    Serial.print("Dato guardado en la posición: ");
    Serial.println(index + 1);
  } else {
    Serial.println("No hay espacio disponible para guardar el nuevo dato.");
  }
}


unsigned long binarioaDecimal(String* num) {
  unsigned long resultado = 0;
  unsigned long base = 1; // Potencia inicial de 2
  Serial.println(*num);
  for (int i = num->length()-1; i >= 0; i--) {
    if (num->charAt(i) == '1') {
      Serial.print("   1 * ");
      Serial.print(base);
      Serial.print("(2^");
      Serial.print(num->length() - i - 1);
      Serial.print(") = ");
      Serial.print(resultado);
      Serial.print(" + ");
      Serial.print(base);
      resultado = resultado + base;
      Serial.print("\n");
    } else {
      Serial.print("   0 * ");
      Serial.print(base);
      Serial.print("(2^");
      Serial.print(num->length() - i - 1);
      Serial.print(") = ");
      Serial.print(resultado);
      Serial.print("+0\n");
    }
    base *= 2; 
  }

  return resultado;
}


const int MAX_BITS = 50;

String suma_binaria(String sublista[], int n) {
  int max_len = 0;

  // Encuentra la longitud máxima entre los números binarios
  for (int i = 0; i < n; i++) {
    if (sublista[i].length() > max_len) {
      max_len = sublista[i].length();
    }
  }

  // Imprime todos los números binarios
  Serial.println("Los números binarios son:");
  for (int i = 0; i < n; i++) {
    // Rellenar con ceros a la izquierda si es necesario
    String num_con_ceros = "";
    for (int j = 0; j < (max_len - sublista[i].length()); j++) {
      num_con_ceros += "0";
    }
    num_con_ceros += sublista[i];
    Serial.println(num_con_ceros); // Imprimir cada número con formato
  }
  Serial.println("-----");

  // Array para el resultado y variables para la suma
  char resultado[MAX_BITS] = {0}; 
  int acarreo = 0;

  // Proceso de la suma
  for (int posicion = 0; posicion < max_len; posicion++) {
    int suma = acarreo;

    // Sumamos cada columna de bits
    for (int i = 0; i < n; i++) {
      if (posicion < sublista[i].length()) {
        suma += sublista[i].charAt(sublista[i].length() - 1 - posicion) - '0'; 
      }
    }

    int bit_resultante = suma % 2;
    acarreo = suma / 2;

    // Guardamos el bit resultante en la posición correspondiente
    if (posicion < MAX_BITS) {
      resultado[posicion] = (char)(bit_resultante + '0');
    }
  }

  // Procesamos el acarreo adicional
  int pos = max_len;
  while (acarreo > 0) {
    if (pos < MAX_BITS) {
      resultado[pos++] = (char)((acarreo % 2) + '0');
      acarreo /= 2;
    } else {
      break;
    }
  }

  // Convertimos el resultado a String
  String resultado_str = "";
  for (int i = pos - 1; i >= 0; i--) {
    resultado_str += resultado[i];
  }
  
  // Imprimimos el resultado
  Serial.println(resultado_str);


  return resultado_str;
}


void validarsumabinaria(String sublista[], int n) {
  unsigned long suma_manual = 0;

  for (int i = 0; i < n; i++) {
    unsigned long decimal = binarioaDecimal(&sublista[i]);
    suma_manual += decimal;
  }

  String resultado_str = suma_binaria(sublista, n);
  unsigned long resultado_decimal = binarioaDecimal(&resultado_str);

  Serial.print("Suma binaria = ");
  Serial.print(resultado_str);
  Serial.print(" (en decimal: ");
  Serial.print(resultado_decimal);
  Serial.print(")\n");

  Serial.print("Suma decimal = ");
  Serial.print(suma_manual);
  Serial.print("\n");

  if (suma_manual == resultado_decimal) {
    Serial.println("Suma correcta");
  } else {
    Serial.println("Suma incorrecta");
  }
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  int n = sizeof(prueba1) / sizeof(prueba1[0]);
  validarsumabinaria(prueba1, n);
  delay(10000);
  n = sizeof(prueba2) / sizeof(prueba2[0]);
  validarsumabinaria(prueba2, n);
  n = sizeof(prueba3) / sizeof(prueba3[0]);
  validarsumabinaria(prueba3, n);
  delay(10000);
   n = sizeof(prueba4) / sizeof(prueba4[0]);
  validarsumabinaria(prueba4, n);
  delay(10000);
}

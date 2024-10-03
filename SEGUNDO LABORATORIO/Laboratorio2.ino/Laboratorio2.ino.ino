const int MAX_LENGTH = 25;
const int MAX_VARS = 5;

struct Variable {
  char nombre;
  char valor[MAX_LENGTH];
};

Variable variables[MAX_VARS];
int cuenta_var = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Calculadora Binaria para Arduino");
}

void loop() {
  Serial.println("\nMenu:");
  Serial.println("1. Almacenar valor");
  Serial.println("2. Realizar operacion");
  Serial.println("3. Mostrar valores almacenados");
  Serial.println("4. Salir");
  while (!Serial.available()) {}
  char eleccion = Serial.read();
  
  switch (eleccion) {
    case '1':
      almacenarValor();
      break;
    case '2':
      realizarOperacion();
      break;
    case '3':
      mostrarValoresAlmacenados();
      break;
    case '4':
      Serial.println("Saliendo...");
      while(1) {} // Bucle infinito para detener la ejecucion
    default:
      Serial.println("Eleccion invalida. Intenta de nuevo.");
  }
}

void almacenarValor() {
  if (cuenta_var >= MAX_VARS) {
    Serial.println("Numero maximo de variables alcanzado.");
    return;
  }
  
  Serial.println("Ingresa el nombre de la variable (A-E):");
  while (!Serial.available()) {}
  char nombre = Serial.read();
  if (nombre < 'A' || nombre > 'E') {
    Serial.println("Nombre de variable invalido. Usa A-E.");
    return;
  }
  
  Serial.println("Ingresa el valor binario:");
  while (!Serial.available()) {}
  String valor = Serial.readStringUntil('\n');
  valor.trim();
  
  if (valor.length() > MAX_LENGTH - 1) {
    Serial.println("Valor demasiado largo. La longitud maxima es de 24 bits.");
    return;
  }
  
  for (int i = 0; i < valor.length(); i++) {
    if (valor.charAt(i) != '0' && valor.charAt(i) != '1') {
      Serial.println("Valor binario invalido. Usa solo 0 y 1.");
      return;
    }
  }
  
  variables[cuenta_var].nombre = nombre;
  strcpy(variables[cuenta_var].valor, valor.c_str());
  cuenta_var++;
  
  Serial.print("Almacenado ");
  Serial.print(nombre);
  Serial.print(": ");
  Serial.println(valor);
}

void mostrarValoresAlmacenados() {
  if (cuenta_var == 0) {
    Serial.println("No hay valores almacenados.");
    return;
  }
  
  Serial.println("Valores almacenados:");
  for (int i = 0; i < cuenta_var; i++) {
    Serial.print(variables[i].nombre);
    Serial.print(": ");
    Serial.print(variables[i].valor);
    Serial.print(" (decimal: ");
    Serial.print(binarioADecimal(variables[i].valor));
    Serial.println(")");
  }
}

long binarioADecimal(const char* binario) {
  long resultado = 0;
  while (*binario) {
    resultado = (resultado << 1) + (*binario++ - '0');
  }
  return resultado;
}

void realizarOperacion() {
  if (cuenta_var < 2) {
    Serial.println("Se necesitan al menos dos variables para realizar la operacion.");
    return;
  }
  
  Serial.println("Ingresa la operacion (por ejemplo, A+B):");
  while (!Serial.available()) {}
  String operacion = Serial.readStringUntil('\n');
  operacion.trim();
  
  if (operacion.length() != 3 || (operacion[1] != '+' && operacion[1] != '-' && operacion[1] != '*' && operacion[1] != '/')) {
    Serial.println("Operacion invalida. Usa el formato: A+B, A-B, A*B, o A/B");
    return;
  }
  
  char op1 = operacion[0];
  char op = operacion[1];
  char op2 = operacion[2];
  
  const char* val1 = nullptr;
  const char* val2 = nullptr;
  
  for (int i = 0; i < cuenta_var; i++) {
    if (variables[i].nombre == op1) val1 = variables[i].valor;
    if (variables[i].nombre == op2) val2 = variables[i].valor;
  }
  
  if (!val1 || !val2) {
    Serial.println("Una o ambas variables no encontradas.");
    return;
  }
  
  char resultado[MAX_LENGTH];
  
  switch (op) {
    case '+':
      sumar(val1, val2, resultado);
      break;
    case '-':
      restar(val1, val2, resultado);
      break;
    case '*':
      multiplicar(val1, val2, resultado);
      break;
    case '/':
      dividir(val1, val2, resultado);
      break;
  }
  
  Serial.print("Resultado (binario): ");
  Serial.println(resultado);
  Serial.print("Resultado (decimal): ");
  Serial.println(binarioADecimal(resultado));
}

void sumar(const char* a, const char* b, char* resultado) {
  int acarreo = 0;
  int i = strlen(a) - 1;
  int j = strlen(b) - 1;
  int k = MAX_LENGTH - 2;
  resultado[k + 1] = '\0';
  
  Serial.println("Suma:");
  Serial.print("  ");
  Serial.println(a);
  Serial.print("+ ");
  Serial.println(b);
  
  while (i >= 0 || j >= 0 || acarreo) {
    int suma = acarreo;
    if (i >= 0) suma += a[i] - '0';
    if (j >= 0) suma += b[j] - '0';
    resultado[k] = (suma % 2) + '0';
    acarreo = suma / 2;
    i--; j--; k--;
  }
  
  while (k >= 0) {
    resultado[k] = '0';
    k--;
  }
  
  // Eliminar ceros a la izquierda
  while (resultado[0] == '0' && resultado[1] != '\0') {
    for (int m = 0; resultado[m]; m++) {
      resultado[m] = resultado[m + 1];
    }
  }
  
  Serial.print("= ");
  Serial.println(resultado);
}

void restar(const char* a, const char* b, char* resultado) {
  int prestamo = 0;
  int i = strlen(a) - 1;
  int j = strlen(b) - 1;
  int k = MAX_LENGTH - 2;
  resultado[k + 1] = '\0';
  
  Serial.println("Resta:");
  Serial.print("  ");
  Serial.println(a);
  Serial.print("- ");
  Serial.println(b);
  
  while (i >= 0 || j >= 0) {
    int diferencia = prestamo;
    if (i >= 0) diferencia += a[i] - '0';
    if (j >= 0) diferencia -= b[j] - '0';
    if (diferencia < 0) {
      diferencia += 2;
      prestamo = -1;
    } else {
      prestamo = 0;
    }
    resultado[k] = diferencia + '0';
    i--; j--; k--;
  }
  
  while (k >= 0) {
    resultado[k] = '0';
    k--;
  }
  
  // Eliminar ceros a la izquierda
  while (resultado[0] == '0' && resultado[1] != '\0') {
    for (int m = 0; resultado[m]; m++) {
      resultado[m] = resultado[m + 1];
    }
  }
  
  Serial.print("= ");
  Serial.println(resultado);
}

void multiplicar(const char* a, const char* b, char* resultado) {
  char temp[MAX_LENGTH];
  strcpy(resultado, "0");
  
  Serial.println("Multiplicacion:");
  Serial.print("  ");
  Serial.println(a);
  Serial.print("* ");
  Serial.println(b);
  
  for (int i = strlen(b) - 1; i >= 0; i--) {
    if (b[i] == '1') {
      strcpy(temp, a);
      for (int j = 0; j < strlen(b) - 1 - i; j++) {
        strcat(temp, "0");
      }
      char suma[MAX_LENGTH];
      sumar(resultado, temp, suma);
      strcpy(resultado, suma);
    }
  }
  
  Serial.print("= ");
  Serial.println(resultado);
}

void dividir(const char* a, const char* b, char* resultado) {
  char dividendo[MAX_LENGTH];
  char divisor[MAX_LENGTH];
  char cociente[MAX_LENGTH] = "";
  char temp[MAX_LENGTH];
  bool puntoDecimal = false; // Para verificar si ya se agregó el punto decimal

  strcpy(dividendo, a);
  strcpy(divisor, b);
  
  Serial.println("División:");
  Serial.print("Dividendo: ");
  Serial.println(dividendo);
  Serial.print("Divisor: ");
  Serial.println(divisor);
  
  while (strlen(dividendo) > 0) {
    // Comparar el dividendo con el divisor
    if (comparar(dividendo, divisor) >= 0) {
      restar(dividendo, divisor, temp);
      strcpy(dividendo, temp);
      strcat(cociente, "1");
    } else {
      strcat(cociente, "0");
    }

    // Verificar si debemos agregar un punto decimal
    if (strlen(dividendo) < strlen(divisor) && !puntoDecimal) {
      strcat(cociente, ".");
      puntoDecimal = true; // Ya agregamos el punto decimal
    }

    // Añadir un 0 al dividendo si es necesario después del punto
    if (puntoDecimal && strlen(dividendo) < strlen(divisor)) {
      strcat(dividendo, "0");
    } else {
      // Desplazar dividendo en otros casos
      for (int i = 0; i < strlen(dividendo) - 1; i++) {
        dividendo[i] = dividendo[i + 1];
      }
      dividendo[strlen(dividendo) - 1] = '\0';
    }
  }

  strcpy(resultado, cociente);
  Serial.print("= ");
  Serial.println(resultado);
}

int comparar(const char* a, const char* b) {
  if (strlen(a) > strlen(b)) return 1;
  if (strlen(a) < strlen(b)) return -1;
  
  for (int i = 0; i < strlen(a); i++) {
    if (a[i] > b[i]) return 1;
    if (a[i] < b[i]) return -1;
  }
  
  return 0;
}
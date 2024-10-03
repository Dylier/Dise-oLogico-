import re

bits_permitidos = [8, 16, 24]
valores_guardados = {}

pruebas = {
    1: {"A": "111100001111000011110000", "B": "00110011", "C": "111100001111000011110000", "E":"00000011", "D": "1010101011010101"},
    2: {"A": "111100001111000011110000", "B": "0011001110110010", "C": "111100001111000011110000", "E":"10100011", "D":"1010101011010101"},
    3: {"A": "111100001111000011110000", "B": "00110011", "C": "1111000011110000", "E":"10100011", "D": "1010101011010101"},
    4: {"A": "10110011", "B": "00110011", "C": "1111000011110000", "E":"111100001111000011110000", "D": "1010101011010101"}
}

operaciones_prueba = {
    1: "((A*B)+(C-D))/E",
    2: "((A+B)*(C-D))/E",
    3: "((A*B)-(C+D))/E",
    4: "(A*B*C*D)/E"
}

def binario_a_decimal(num):
    if "." in num:
        parte_entera, parte_decimal = num.split(".")
    else:
        parte_entera, parte_decimal = num, ""

    parte_entera_invertida = parte_entera[::-1]
    resultado_entero = 0
    for expo, n in enumerate(parte_entera_invertida):
        resultado_entero += int(n) * (2 ** expo)
    resultado_decimal = 0
    for expo, n in enumerate(parte_decimal, 1):
        resultado_decimal += int(n) * (2 ** -expo)
    resultado_total = resultado_entero + resultado_decimal
    return resultado_total

def mostrar_resultado(operacion, pasos, resultado_binario, resultado_decimal):
    print("\n" + "=" * 50)
    print(f"Operación: {operacion}")
    print("\nPasos intermedios:")
    for i, paso in enumerate(pasos, 1):
        print(f"Paso {i}:")
        for linea in paso.split('\n'):
            print(f"  {linea}")
    print(f"\nResultado binario final: {resultado_binario}")
    print(f"Resultado decimal final: {resultado_decimal}")

    operacion_decimal = operacion
    for var, valor in valores_guardados.items():
        operacion_decimal = operacion_decimal.replace(var, str(binario_a_decimal(valor)))
    resultado_decimal_natural = eval(operacion_decimal)
    print(f"\nOperación decimal natural: {operacion_decimal}")
    print(f"Resultado decimal natural: {resultado_decimal_natural:.2f}")

    print("\nComparación:")
    if round(resultado_decimal,0) <= round(resultado_decimal_natural,0):
        print("Los resultados son iguales.")
    else:
        print("Los resultados son diferentes.")
    print("=" * 50 + "\n")

def guardar_valor():
    variable = input("Ingrese el nombre de la variable (A-Z): ").upper()
    if not re.match(r'^[A-Z]$', variable):
        print("Error: El nombre de la variable debe ser una sola letra mayúscula.")
        return

    while True:
        valor = input(f"Ingrese el valor binario para '{variable}' (solo 0 y 1): ")
        if not re.match(r'^[01]+$', valor):
            print("Error: Solo se permiten caracteres binarios (0 y 1).")
            continue

        bits = int(input(f"Ingrese la cantidad de bits para '{variable}' (8, 16, 24): "))
        if bits not in bits_permitidos:
            print("Error: La cantidad de bits debe ser 8, 16 o 24.")
            continue

        if len(valor) > bits:
            print(f"Error: El valor excede {bits} bits.")
            continue

        valores_guardados[variable] = valor.zfill(bits)
        print(f"Variable '{variable}' guardada: {valores_guardados[variable]}")
        break

def realizar_operacion(operacion=None):
    if not valores_guardados:
        print("No hay valores guardados. Por favor, guarde algunos valores antes de realizar una operación.")
        return

    if operacion is None:
        print("\nValores disponibles:", ", ".join(valores_guardados.keys()))
        operacion = input("Ingrese la operación (ejemplo: A+B): ").strip()

    try:
        resultado_binario, pasos = calcular_operacion(jerarquia(operacion), valores_guardados)
        if resultado_binario:
            resultado_decimal = binario_a_decimal(resultado_binario)
            mostrar_resultado(operacion, pasos, resultado_binario, resultado_decimal)
    except Exception as e:
        print(f"Error al realizar la operación: {e}")

def jerarquia(operacion):
    prioridades = {'+': 1, '-': 1, '*': 2, '/': 2}
    salida = []
    pila = []
    numero = ''
    for char in operacion:
        if char.isalnum():
            numero += char
        else:
            if numero:
                salida.append(numero)
                numero = ''
            if char == '(':
                pila.append(char)
            elif char == ')':
                while pila and pila[-1] != '(':
                    salida.append(pila.pop())
                pila.pop()  # Eliminar el '('
            elif char in prioridades:
                while pila and pila[-1] != '(' and prioridades.get(pila[-1], 0) >= prioridades[char]:
                    salida.append(pila.pop())
                pila.append(char)
    if numero:
        salida.append(numero)
    while pila:
        salida.append(pila.pop())
    return salida

def calcular_operacion(tokens, valores):
    pila = []
    pasos = []
    for token in tokens:
        if token in valores:
            pila.append(valores[token])
        elif token in ['+', '-', '*', '/']:
            b, a = pila.pop(), pila.pop()
            if token == '+':
                resultado, paso = sumar(a, b)
            elif token == '-':
                resultado, paso = restar(a, b)
            elif token == '*':
                resultado, paso = multiplicar(a, b)
            elif token == '/':
                resultado, paso = dividir(a, b)
            pasos.append(paso)
            pila.append(resultado)
        else:
            pila.append(token)
    return pila[0], pasos

def sumar(a, b):
    max_len = max(len(a), len(b))
    a = a.zfill(max_len)
    b = b.zfill(max_len)
    resultado = ''
    acarreo = 0
    paso = f"  {a}\n+ {b}\n"
    acarreos = ''

    for i in range(max_len - 1, -1, -1):
        suma = int(a[i]) + int(b[i]) + acarreo
        resultado = str(suma % 2) + resultado
        acarreo = suma // 2
        acarreos = str(acarreo) + acarreos

    if acarreo:
        resultado = '1' + resultado

    paso += f"  {acarreos}\n"
    paso += "=" * (max_len + 2) + "\n"
    paso += f"  {resultado}"

    return resultado, f"Suma:\n{paso}"

def restar(a, b):
    max_len = max(len(a), len(b))
    a = a.zfill(max_len)
    b = b.zfill(max_len)
    resultado = ''
    prestamo = 0
    paso = f"  {a}\n- {b}\n"
    prestamos = ''

    for i in range(max_len - 1, -1, -1):
        resta = int(a[i]) - int(b[i]) - prestamo
        if resta < 0:
            resta += 2
            prestamo = 1
        else:
            prestamo = 0
        resultado = str(resta) + resultado
        prestamos = str(prestamo) + prestamos

    paso += f"  {prestamos}\n"
    paso += "=" * (max_len + 2) + "\n"
    paso += f"  {resultado}"

    return resultado.lstrip('0') or '0', f"Resta:\n{paso}"

def multiplicar(a, b):
    resultado = '0'
    paso = f"  {a}\n* {b}\n"
    productos_parciales = []

    for i, bit in enumerate(b[::-1]):
        if bit == '1':
            producto_parcial = (a + '0' * i).zfill(len(a) + len(b))
            productos_parciales.append(producto_parcial)
            paso += f"  {producto_parcial}\n"
        else:
            paso += f"  {'0' * (len(a) + len(b))}\n"

    paso += "=" * (len(a) + len(b) + 2) + "\n"

    for producto in productos_parciales:
        resultado, suma_paso = sumar(resultado, producto)

    paso += f"  {resultado}"

    return resultado, f"Multiplicación:\n{paso}"

def dividir(a, b):
    cociente = ""
    residuo = 0
    paso = f"Dividendo: {a}\nDivisor:  {b}\n\nProceso:\n"

    for bit in a:
        residuo = (residuo << 1) + int(bit)
        if residuo >= int(b, 2):
            cociente += '1'
            residuo -= int(b, 2)
        else:
            cociente += '0'
        paso += f"Residuo actual: {bin(residuo)[2:].zfill(len(b))}, Cociente actual: {cociente}\n"

    resultado = cociente.lstrip('0') or '0'
    paso += f"\nCociente final: {resultado}\nResiduo final: {bin(residuo)[2:].zfill(len(b))}"

    if residuo != 0:
        resultado += '.'
        contador_decimales = 0
        while residuo != 0 and contador_decimales < 10:
            residuo <<= 1
            if residuo >= int(b, 2):
                resultado += '1'
                residuo -= int(b, 2)
            else:
                resultado += '0'
            contador_decimales += 1
            paso += f"Residuo actual: {bin(residuo)[2:].zfill(len(b))}, Cociente actual: {resultado}\n"

    return resultado, f"División:\n{paso}"

def mostrar_valores_guardados():
    if valores_guardados:
        print("\nValores guardados:")
        for var, valor in valores_guardados.items():
            print(f"{var}: {valor} (decimal: {binario_a_decimal(valor)})")
    else:
        print("No hay valores guardados.")

def realizar_prueba():
    global valores_guardados
    num_prueba = int(input("Ingrese el número de prueba (1-4): "))
    if num_prueba not in pruebas:
        print("Número de prueba inválido.")
        return

    valores_guardados = pruebas[num_prueba].copy()
    print(f"\nPRUEBA NUMERO {num_prueba}")
    print("DATOS:")
    mostrar_valores_guardados()
    print(f"OPERACION: {operaciones_prueba[num_prueba]}")
    realizar_operacion(operaciones_prueba[num_prueba])

def menu():
    while True:
        print("\nMenú:")
        print("1. Guardar valor")
        print("2. Realizar operación")
        print("3. Mostrar valores guardados")
        print("4. Realizar prueba")
        print("5. Salir")

        opcion = input("Seleccione una opción: ")

        if opcion == "1":
            guardar_valor()
        elif opcion == "2":
            realizar_operacion()
        elif opcion == "3":
            mostrar_valores_guardados()
        elif opcion == "4":
            realizar_prueba()
        elif opcion == "5":
            print("Saliendo del programa.")
            break
        else:
            print("Opción no válida. Intente nuevamente.")

if __name__ == "__main__":
    menu()
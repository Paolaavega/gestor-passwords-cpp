# Gestor de Contraseñas en C++

Proyecto escolar en C++ que guarda credenciales (sitio web, usuario, contraseña y notas) desde la consola, usando **programación orientada a objetos** y un cifrado simple por sustitución.

> ⚠️ **Aviso de seguridad:** este proyecto es solo con fines educativos. El cifrado usado es muy básico y **no es seguro**. No lo uses para guardar contraseñas reales.

## ¿Qué hace?

Al ejecutarlo, muestra un menú con estas opciones:

| Opción | Acción |
|--------|--------|
| 1 | Agregar una nueva entrada (URL, usuario, contraseña y notas) |
| 2 | Mostrar todas las entradas con la contraseña encriptada |
| 3 | Mostrar todas las entradas con la contraseña desencriptada |
| 4 | Buscar entradas por URL (o parte de ella) |
| 5 | Salir y guardar |

Las entradas se guardan en un archivo llamado `passwords.dat` y se cargan automáticamente la próxima vez que se abre el programa.

## ¿Cómo funciona?

El programa está dividido en tres clases:

| Clase | Responsabilidad |
|-------|-----------------|
| `Encriptador` | Cifra y descifra texto sustituyendo cada letra por otra según un alfabeto de reemplazo. Los números y símbolos no se modifican. |
| `PasswordEntry` | Representa una entrada individual y sabe mostrarse en pantalla y convertirse a una línea de texto para el archivo. |
| `PasswordManager` | Administra la lista de entradas, las carga y guarda en el archivo, y permite agregar y buscar. |

Cada línea del archivo `passwords.dat` tiene este formato:

```
url|password_encriptada|usuario|notas
```

## Conceptos que se aplican

- Clases, objetos y encapsulamiento
- Constructores y destructores (el destructor guarda el archivo al cerrar)
- `vector` de la STL
- Lectura y escritura de archivos con `fstream`
- Manejo de cadenas y búsqueda sin distinguir mayúsculas
- Validación de entradas del usuario

## Tecnologías

- C++
- Librería estándar (no requiere instalar nada extra)

## Cómo ejecutarlo

Necesitas un compilador como `g++`. Desde la terminal:

```bash
g++ gestor.cpp -o gestor
./gestor
```

También puedes abrir el repositorio en **GitHub Codespaces** (botón verde *Code* → pestaña *Codespaces*) y ejecutar los mismos comandos sin instalar nada.

## Ejemplo de uso

```
Gestor de Passwords
1. Agregar nueva entrada
2. Mostrar todas las entradas (encriptadas)
3. Mostrar contrasenas desencriptadas
4. Buscar por URL
5. Salir
Seleccione una opcion: 1
URL: github.com
Username: paola
Password: hola123
Notas: mi cuenta
Entrada agregada correctamente.
```

## Limitaciones

- El cifrado por sustitución es fácil de romper y la clave está escrita en el código. Un gestor real usaría algoritmos como AES y una contraseña maestra.
- Solo cifra letras; los números y símbolos quedan igual.
- No permite editar ni eliminar entradas.
- Los campos no pueden contener el carácter `|`, porque se usa como separador en el archivo.

## Autor

**Paola Vega** · Estudiante de Ingeniería en Sistemas Computacionales, UACJ

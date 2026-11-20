# my-regex

Biblioteca experimental en C++17 para construir expresiones regulares y representar el resultado como NFA, DFA o tabla de transiciones.

> El proyecto está en desarrollo. Algunas características pueden cambiar antes de alcanzar una API estable.

## Características

- Construcción de expresiones regulares.
- Conversión de NFA a DFA.
- Conversión de DFA a tabla de transiciones.
- Operadores básicos: concatenación, alternancia, clases y cuantificadores.
- Admite `char` y `wchar_t`.
- Compatible con CMake y como objetivo de cabecera.

## Requisitos

- CMake 3.14 o posterior.
- Un compilador compatible con C++17.

## Compilar

Desde la raíz del proyecto:

```sh
cmake -S . -B build
cmake --build build
```

Con MinGW, en PowerShell:

```powershell
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
```

## Ejecutar las pruebas

```sh
ctest --test-dir build --output-on-failure
```

Las pruebas disponibles son:

- `regex_basic`: construcción y verificación con NFA, DFA y tabla.
- `converter_unicode`: conversiones de cadenas y casos Unicode.

## Uso básico

```cpp
#include <myregex/myregex.hpp>
#include <iostream>

int main()
{
    myregex::regex<size_t> regex = myregex::builder<size_t>({
        {1, "a+"},
        {2, "b(c|d)"}
    }).convert_to_dfa()
      .convert_to_table()
      .build();

    for (const char* input : {"aaaa", "bc", "bd", "xx"})
    {
        std::cout << input << " -> "
                  << (regex.verification(input) ? "válido" : "inválido")
                  << '\n';
    }
}

// Compilar con CMake:
// target_link_libraries(mi_programa PRIVATE myregex::myregex)
```

También se puede obtener la coincidencia detectada:

```cpp
auto result = regex.match<myregex::constants::match_maximun>("bcd");
std::cout << result.str() << ':' << result.id() << '\n';
```

## Sintaxis admitida

- Concatenación: `ab`
- Alternancia: `a|b`
- Grupos: `(ab)+`
- Cero o más: `a*`
- Uno o más: `a+`
- Opcional: `a?`
- Rangos: `a{2}`, `a{2,4}`
- Clases: `[a-z]`, `[^0-9]`
- Escapes compatibles con el analizador incorporado.

## Instalar

```sh
cmake --install build --prefix install
```

El paquete exportado permite consumir la biblioteca desde otro proyecto:

```cmake
find_package(myregex CONFIG REQUIRED)
target_link_libraries(mi_programa PRIVATE myregex::myregex)
```

## Licencia

Este proyecto se distribuye bajo la licencia MIT. Consulta el archivo `LICENSE`.

MIT es una licencia permisiva apropiada para esta biblioteca: permite usar, modificar y distribuir el proyecto con pocas restricciones y exige conservar el aviso de copyright y la licencia.

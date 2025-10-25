-colapartido.cpp:
la logica estaba muy bien, pero estaba usando structs en vez de llamando las clases que ya existen en los otros archivos. cambié eso y puse algunos elementos en colapartidos.h para poder llamarlos por separado (archivos .h son muy buenos cuando estas trabajando con varios
archivos pq puedes declarar cosas en el y luego definir como funciona en el .cpp, y es mucho mas eficiente con memoria)

-arbol.cpp:
tambien hace falta integrar las clases que ya hicimos. La logica está muy bien, nada mas lo separé entre .h y .cpp como la otra para poder usar los otros archivos. Usa entradas y salidas de consola, no está mal para ir viendo como funciona, pero yo he estado usando esto en
main.cpp para no enredarnos al momento de cambiar esto de consola a GUI

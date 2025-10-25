-colapartido.cpp:
la logica estaba muy bien, pero estaba usando structs en vez de llamando las clases que ya existen en los otros archivos. cambié eso y puse algunos elementos en colapartidos.h para poder llamarlos por separado (archivos .h son muy buenos cuando estas trabajando con varios
archivos pq puedes declarar cosas en el y luego definir como funciona en el .cpp, y es mucho mas eficiente con memoria)

-arbol.cpp:
tambien hace falta integrar las clases que ya hicimos. La logica está muy bien, nada mas lo separé entre .h y .cpp como la otra para poder usar los otros archivos. Usa entradas y salidas de consola, no está mal para ir viendo como funciona, pero yo he estado usando esto en
main.cpp para no enredarnos al momento de cambiar esto de consola a GUI

lo que estas haciendo es basicamente escribiendo codigo que funciona como su propio entidad en vez de usar el resto del proyecto. Si tu compu no logra recorrer todos los archivos juntos, no pasa nada, pero la ventaja de c++ es que es muy eficiente con espacio y memoria.
asi que si puedes recorrer todos los archivos juntos con el main.cpp, creo que te daria un mejor idea de como funcionan juntos los archivos. Pero si no, igual la logica que estas haciendo esta muy bien, yo puedo seguir integrandolo como hice con los otros :)

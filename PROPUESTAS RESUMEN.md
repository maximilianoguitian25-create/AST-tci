# Propuestas de AF

---

## Propuesta 1 — Delta como Lista de Transiciones

De esta propuesta entiendo que cada transición se almacena de manera independiente mediante la estructura `Transition`. El campo `Tdata to` permite almacenar tanto un único estado destino para un AFD como un conjunto de estados destino para un AFND.

Observo que `TransitionNode` implementa una lista enlazada simple de transiciones y que la estructura `Automata` contiene explícitamente los componentes principales de un autómata: conjunto de estados, alfabeto, transiciones, estado inicial y estados de aceptación. Además, incorpora un campo adicional para indicar si el autómata es determinista o no.

Como ventaja principal encuentro que es una solución simple y fácil de implementar. También resulta sencilla de comprender y mantener.

Como desventaja, noto que la función delta debe recorrer toda la lista de transiciones para encontrar la combinación correspondiente de estado y símbolo. Por este motivo, el rendimiento puede disminuir cuando el número de transiciones aumenta.

Respecto a los grafos, interpreto que cada estado representa un nodo y cada transición una arista. Sin embargo, el grafo no está representado de manera explícita sino mediante una lista de transiciones.

---

## Propuesta 2 — Delta Indexado por Estado

En esta propuesta observo que cada estado almacena directamente sus transiciones mediante una lista enlazada propia.

Entiendo que el autómata queda representado como una lista de estados, donde cada estado posee sus transiciones asociadas. Esto permite acceder más rápidamente a las transiciones de un estado determinado sin tener que recorrer todas las transiciones existentes.

Como ventaja destaco que la búsqueda de transiciones es más eficiente que en la propuesta anterior y que la representación resulta muy natural para modelar un autómata.

Como desventaja, observo que parte de la información que antes estaba explícita, como el conjunto de estados o el alfabeto, queda distribuida dentro de la estructura y puede requerir recorridos adicionales para reconstruirse.

Respecto a los grafos, considero que es la propuesta que mejor representa un grafo real, ya que cada estado funciona como un nodo y sus transiciones como aristas salientes.

---

## Propuesta 3 — Delta como Matriz Dispersa

Entiendo que esta propuesta busca representar la función de transición mediante una matriz indexada por estados y símbolos. Al tratarse de una matriz dispersa, únicamente se almacenan las transiciones existentes y no las posiciones vacías.

Observo que surge la necesidad de asignar índices numéricos a estados y símbolos. Como los conjuntos de estados y símbolos se almacenan como SET, no poseen un orden natural, por lo que sería necesario generar previamente una numeración para poder trabajar con índices.

De la estructura `TransitionEntry` interpreto que el campo `from` almacena el índice del estado origen y `symbol` el índice del símbolo correspondiente. El campo `to` mantiene la posibilidad de representar tanto AFD como AFND.

También noto que el estado inicial se almacena como un entero y no como un STR, lo que confirma que toda la representación se basa en índices. Además, el campo `deltaSize` permite conocer la cantidad de transiciones almacenadas.

Como ventaja principal destaco la eficiencia en el acceso a las transiciones. Una vez realizada la numeración, las consultas pueden realizarse de manera muy rápida. También me parece una representación utilizada frecuentemente en analizadores léxicos y compiladores.

Como desventaja, observo que requiere una etapa previa de indexación y un mantenimiento de la correspondencia entre índices y estados reales.

Respecto a los grafos, entiendo que los estados continúan representando nodos y las transiciones aristas, aunque dichas relaciones quedan almacenadas dentro de la matriz en lugar de utilizar punteros o listas enlazadas.

---

## Propuesta 4 — Delta Funcional

Observo que esta propuesta intenta representar la función delta de una forma muy cercana a la definición matemática formal.

La estructura `DeltaEntry` almacena explícitamente un estado origen, un símbolo y un conjunto de destinos. Esto permite expresar directamente relaciones del tipo δ(q,a).

Entiendo que la estructura `Automata` mantiene explícitamente los conjuntos de estados, alfabeto y estados finales. Además, la presencia de `delta` junto con `deltaCount` me lleva a interpretar que las transiciones podrían almacenarse mediante un arreglo dinámico.

Como ventaja encuentro que es una propuesta muy clara desde el punto de vista teórico y que se corresponde directamente con la definición matemática de autómata.

Como desventaja noto que para obtener el resultado de una transición es necesario recorrer todas las entradas hasta encontrar la combinación adecuada de estado y símbolo.

Respecto a los grafos, las aristas quedan representadas mediante las aplicaciones de la función delta, aunque el grafo no se almacena explícitamente.

---

## Propuesta 5 — Estados Compuestos para Determinización

Entiendo que esta propuesta está orientada específicamente al proceso de determinización de autómatas finitos no deterministas.

Observo que cada estado está compuesto por un subconjunto de estados originales. Por este motivo, un único estado del nuevo autómata puede representar varios estados del autómata original.

La estructura `CompositeState` almacena estos subconjuntos y las transiciones se realizan entre estados compuestos. La estructura final representa directamente el AFD resultante de la construcción por subconjuntos.

Como ventaja destaco que se encuentra muy alineada con la teoría de autómatas y facilita la implementación del algoritmo de determinización.

Como desventaja observo que el número de estados puede crecer considerablemente debido a la generación de subconjuntos.

Respecto a los grafos, cada nodo representa un conjunto de estados y las aristas representan las transiciones entre dichos conjuntos.

---

## Justificación de la Elección de la Propuesta 3

Después de analizar las distintas propuestas, elegí la Propuesta 3 porque considero que es la alternativa más eficiente para representar la función de transición. La utilización de índices numéricos para estados y símbolos permite acceder rápidamente a las transiciones sin necesidad de recorrer listas completas, lo que mejora el rendimiento general del autómata.

Además, me parece una estructura organizada y fácil de interpretar, ya que las transiciones pueden visualizarse de manera similar a una tabla. También considero que este tipo de representación es muy utilizada en implementaciones reales relacionadas con compiladores y analizadores léxicos, donde la velocidad de acceso a las transiciones es un aspecto importante.

Si bien requiere una etapa previa para asignar índices a los estados y símbolos, considero que esta complejidad adicional se ve compensada por las ventajas obtenidas en eficiencia y organización. Por estos motivos, la Propuesta 3 fue la que me resultó más adecuada entre todas las alternativas analizadas.

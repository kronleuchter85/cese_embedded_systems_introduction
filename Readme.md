
## Objetivo y descripción del proyecto

El objetivo del proyecto es implementar un sistema embebido para un Smart Meter. Las funcionalidades básicas son:
- por medio de una entrada analogica poder determinar el nivel de consumo hogareño
- permitir la carga de tarifas por parte de la empresa proveedora de energía eléctrica
- en base al consumo y la tarifa aplicada calcular el costo y presentarlo, tanto al usuario como a la empresa de energía.


## Registro del consumo

Para implementar este sistema, se simula la medición del consumo del medidor mediante una entrada analógica que se puede variar con un potenciómetro. Considerando un valor estándar de consumo de 300 kWh por mes se puede asumir, solo a fin de fijar un valor, que un máximo podría ser 400 kWh.
De esta forma, multiplicando la lectura del potenciómetro (valores entre 0 y 1) por el valor máximo esperable se puede simular la regulación en el consumo energético del hogar.
En realidad, para poder hacerlo más realista, durante un intervalo de tiempo se realizan varias lecturas y por medio de una función se calcula un estimado del valor consumido en una hora y medido en kWh. Una vez estimado el consumo por hora, para calcular el consumo diario y mensual solo se acumulan estos valores.
El proceso de muestreo para la estimación del consumo por hora se realiza marcado por una interrupción.


## Ingreso y activación de las tarifas

Usando la interfaz UART se utiliza la terminal serie como medio para que el usuario de la empresa de energía realice operaciones:
- listar las tarifas existentes
- agregar una nueva tarifa
- activar una tarifa
- imprimir el consumo y facturación actual para la vivienda 


## Presentacion del consumo y facturación en el Smart Meter

Usando un display de 4 líneas y 14 caracteres se presenta en tiempo real el estado del medidor y el costo que se aplicará en la factura con diferentes niveles de agregacion:
- Consumo estimado de la hora
- Consumo estimado del dia
- Consumo estimado del mes
- Consumo total
- Tarifa vigente aplicada


## Diagramas

### Diagrama en bloques de alto nivel
![alt text](https://github.com/kronleuchter85/embedded_systems_introduction/blob/master/IdCE-TPFinal.jpg )

### Modelado de Software
![alt text](https://github.com/kronleuchter85/embedded_systems_introduction/blob/master/IdCE-TPFinal-SW.jpg )

### Conexionado fisico
![alt text](https://github.com/kronleuchter85/embedded_systems_introduction/blob/master/IdCE-TPFinal-HW.jpg )

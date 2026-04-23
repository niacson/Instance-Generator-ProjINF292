# Compilacion:
g++ -Wall -Wextra instance_generator.cpp -o instance_generator

# Modo de uso:
./instance_generator \<num de instancias a generar\> \<tamaño de las instancias\>  
**tamaño de instancia:** Debe ser un caracter s, m y b para pequeño, medio y grande respectivamente

# Extras:
Para sistemas linux se puede automatizar la creacion de 5 instancias de todos los tamaños mediante el archivo **runner.sh**, para poder hacer uso de este es importante
primero dar permisos de ejecucion al archivo con el siguiente comando "*chmod +x runner.sh*" y luego ejecutarlo mediante "*./runner.sh*"

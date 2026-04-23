g++ -Wall -Wextra instance_generator.cpp -o instance_generator
if ! ./instance_generator 5 s; then
    echo "Hubo un error en la ejecucion."
    exit 1
fi
if ! ./instance_generator 5 m; then
    echo "Hubo un error en la ejecucion."
    exit 1
fi
if ! ./instance_generator 5 b; then
    echo "Hubo un error en la ejecucion"
    exit 1
fi
echo "Instancias generadas correctamente"

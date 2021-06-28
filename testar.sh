#Metam na pasta do campista, os ficheiros .camp numa pasta testfilesAeB, os .tents do professor numa chamada testfilesAeB_solution.
#Na primeira execução fazer chmod +x testar.sh
#Para correr fazer ./testar.sh

#Ele imprime FICHEIRO NOME_DO_FICHEIRO e de seguida imprime as diferenças entre o gerado e o do professor
#Apenas imprimir os nomes dos ficheiros é porque todos funcionam bem

for file in testfilesAeB/*; do
    filename="$(basename "$file")"
    if [ ${file: -5} == ".camp" ]
    then

        filename_without_extension="${filename%.*}"
        echo FICHEIRO: $filename_without_extension

	./campista testfilesAeB/$filename
  #valgrind --leak-check=summary ./campista testfilesAeB/$filename
  diff testfilesAeB/$filename_without_extension.tents testfilesAeB_solution/$filename_without_extension.check;
    fi
done

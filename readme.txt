Esse projeto utiliza a versão 11 do C++. O Makefile compila o programa corretamente.
O executável é gerado na pasta bin.

Utilização do comando:

tsp arquivo.tsp [arquivo.tsp] ...

O programa recebe como argumentos um ou mais arquivos de onde ler as entradas

Métodos de cálculo de lower bound:
- Soma das arestas da árvore geradora mínima
- 1-Tree Lower Bound Method

Heurísticas construtivas utilizadas:
- Vizinho mais próximo
- Caminho euleriano na árvore mínima dupla

Heurísticas de busca local utilizadas:
- 2-Opt
- 3-Opt

Metaheurísticas utilizadas:
- Variable Neighborhood Descent utilizando 2-Opt e 3-Opt
- Simulated annealing usando soluções 2-Opt aleatórias

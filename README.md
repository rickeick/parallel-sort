## Laboratório de Algoritmos de Ordenação Paralelo

### Equipe
- Rick Eick Vieira Dos Santos
- Victor José Beltrão Almajano Martinez
- Juan Carlos Melo do Nascimento

### Objetivo
Possibilitar a experimentação e o aprendizado de conceitos relacionados a computação paralela, bem como implementação de algoritmos de ordenação em máquinas paralelas de memória compartilhada, com atenção para a dependência de dados.

### Etapas
1. O discente deve providenciar os seguintes algoritmos de ordenação
   - Implementação sequencial do mergesort
   - Implementação paralela de memória compartilhada do mergesort
   - Implementação paralela de memória distribuída do mergesort. [(REF)](https://digitalcommons.chapman.edu/cgi/viewcontent.cgi?article=1017&context=scs_books)
   - Versão paralela do Odd-Even usando OpenMP (disponível no Livro do Pacheco: Program 5.5: Second OpenMP implementation of odd-even sort, Pag 236.)
   - Versão paralela do Odd-Even mergesort [(REF1)](https://webpages.charlotte.edu/abw/coit-grid01.uncc.edu/ITCS4145F12/slides10.ppt) [(REF2)](https://hwlang.de/algorithmen/sortieren/networks/oemen.htm)
2. Realizar um experimento de comparação das implementações, aferindo tempo de processamento médio em 10 execuções
3. Usar os dados derivados de vetores inteiros com as seguintes quantidades de elementos:
    - 10 milhões (Half)
    - 100 milhões (Original)
    - 1 bilhões (Double)
4. Para cada item anterior testar a seguinte escala de número de processos/threads
    - sequencial: 1 processo/thread
    - paralelo 2: 2 processos/threads
    - paralelo 3: 3 processos/threads
    - paralelo 4: 4 processos/threads
    - outras, caso o equipamento permita
5. Usar as versões sequenciais para medir a eficiência das versões em paralelo, montando tabelas similares à tabela (Table 2.5 Speedups and Efficiencies of a Parallel Program on Different Problem Sizes), comparando as quantidades de dados Half, Original e Double, para cada algoritmo.
6. Discutir e concluir o trabalho na forma de um relatório a ser enviado via SIGAA.

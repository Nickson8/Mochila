#include <stdlib.h>
#include <stdio.h>

typedef unsigned char uc;


/**
 * Função que gera e analisa todas as combinações de items possíveis
 * 
 * @param w -> Vetor que contém os pesos dos items
 * @param v -> Vetor que contém os valores dos items
 * @param n -> Número de items na mochila
 * @param W -> Carga máxima da mochila
 * @param melhor -> Melhor combinação de items
 */
void forca(int *w, int *v, int n, int W, uc *melhor){
    //Variável que guarda a melhor soma de valores até o momento
    int max = 0;

    //Loop de tamanho 2^n que monta todas as combinações de items,
    //que são interpretados como uma bit mask, representada pelo
    //"subset_mask" do loop
    for(int subset_mask=0; subset_mask< (1 << n); subset_mask++){
        //Variáveis para guardar o peso e o valor da combinação atual
        int valor_at = 0, peso_at = 0;

        //Declaração do vetor para guardar o subset que será anali-
        //sado, usando unsigned char por questão de economia na me-
        //mória
        uc subset[n];

        //Loop que passa por todos os "n" items
        for(int j=0; j<n; j++){
            //Analisando se o item j está na lista a partir de uma
            //operação de AND bit a bit com a bit mask e o j em bi-
            //nário
            if(subset_mask & (1 << j)){
                peso_at += w[j];
                valor_at += v[j];
                subset[j] = 1;
            } else{
                subset[j] = 0;
            }
        }

        //Verificando se o valor do subset atual supera o máximo até
        //então e se não ultrapassa o limite de peso
        if (valor_at > max && peso_at <= W){
            //Atualizando as variáveis
            max = valor_at; 
            for (int i = 0; i < n; i++){
                (melhor)[i] = subset[i]; 
            }
        }
    }
}

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void swapF(float *a, float *b){
    float temp = *a;
    *a = *b;
    *b = temp;
}

void quick_sort(float v[], int v2[], int ini, int fim){
    int pivo = v[(ini+fim)/2];
    int i=ini, j=fim;

    do{
        while(v[i] > pivo) i++;
        while(v[j] < pivo) j--;

        if(i<=j){
            swap(&v2[i], &v2[j]);
            swapF(&v[i], &v[j]);
            i++;
            j--;
        }

    } while(i<j);

    if(j>ini) quick_sort(v, v2, ini, j);
    if(i<fim) quick_sort(v, v2, i, fim);
}

/**
 * Função que analisa a razão valor/peso para decidir a melhor combi-
 * nação de items
 * 
 * @param w -> Vetor que contém os pesos dos items
 * @param v -> Vetor que contém os valores dos items
 * @param n -> Número de items na mochila
 * @param W -> Carga máxima da mochila
 * @param melhor -> Melhor combinação de items
 */
void guloso(int *w, int *v, int n, int W, uc *melhor){
    //Zerando o vetor "melhor"
    for (int i = 0; i < n; i++){
        melhor[i] = 0;
    }

    //Vetor que contém as razões valor/peso
    float razoes[n];
    //Vetor com os índices dos items
    int indices[n]; 

    //Loop para preencher "razões" e "índices"
    for (int i = 0; i < n; i++){
        razoes[i] = (float)v[i] / (float)w[i]; 
        indices[i] = i; 
    }

    //Quick Sort para ordenar, em ordem decrescente, a partir de "ra-
    //zões", o vetor "razões" e o vetor "índices"
    quick_sort(razoes, indices, 0, n-1);

    //Montagem da combinação de items a ser atribuída ao "melhor"
    int peso_at = 0; 
    for (int i = 0; i < n; i++){
        peso_at += w[indices[i]]; 
        if (peso_at > W){
            break; 
        } 
        (melhor)[indices[i]] = 1; 
    }
    printf("\n");
}


int max(int a, int b){
    if(a >= b) return a;
    if(a < b) return b;
}

void dynamic(int *w, int *v, int n, int W, uc *melhor){
    //Zerando o vetor "melhor"
    for (int i = 0; i < n; i++){
        melhor[i] = 0;
    }
    
    //Tabela que guarda os melhores valores dependendo de um peso máximo
    int tab[n+1][W+1];

    //Preenchendo "tab"
    for(int i=0; i<n+1; i++){
        for(int p=0; p<W+1; p++){
            //Número de items é 0 ou peso máximo atual é 0
            if(i==0 || p==0){
                tab[i][p] = 0;
            }
            //Item atual cabe no peso máximo atual "p"
            else if(w[i-1] <= p){
                // Decide se o melhor valor para o peso "p" com "i" items é
                // o já calculado anteriormente (não adiciona o item atual)
                // ou se é melhor adicionar o item atual, que para fazer isso
                // adicionamos o valor dele ao máximo de valor que pode ser
                // colocado com o peso que sobra, o que já foi calculado e está
                // em "tab"
                tab[i][p] = max(tab[i-1][p], v[i-1]+tab[i-1][p-w[i-1]]);
            }
            //Nenhuma das opções anteriores
            else {
                //Recebe a melhor solução anterior para um mesmo peso "p"
                tab[i][p] = tab[i-1][p];
            }
        }
    }
    printf("\n%d\n", tab[n][W]);
}


int main(void){
    //Declaração do número de cidades ("n") e o peso máximo ("W")
    int n, W;

    //Leitura dos dados
    printf("Valores de n e W: ");
    scanf("%d %d", &n, &W);

    //Declarando e alocando memória para os vetores
    int *w = malloc(n*sizeof(int));
    int *v = malloc(n*sizeof(int));
    uc *melhor = malloc(n*sizeof(uc));

    //Lendo os valores de para preencher "v" e "w"
    for(int i=0; i<n; i++){
        printf("Valor e peso do item %d: ", i+1);
        scanf("%d %d", &v[i], &w[i]);
    }

    //Printando o resultado do método da força bruta
    printf("\n\n"); 
    forca(w, v, n, W, melhor);
    for(int i=0; i<n; i++){
        if(melhor[i] == 1){
            printf("Item %d: valor: %d, peso: %d\n", i+1, v[i], w[i]);
        }
    }


    //Printando o resultado do método guloso
    printf("\n\n"); 
    guloso(w, v, n, W, melhor);
    for(int i=0; i<n; i++){
        if(melhor[i] == 1){
            printf("Item %d: valor: %d, peso: %d\n", i+1, v[i], w[i]);
        }
    }


    //Printando o resultado do método guloso
    printf("\n\n"); 
    //dynamic(w, v, n, W, melhor);
    /*for(int i=0; i<n; i++){
        if(melhor[i] == 1){
            printf("Item %d: valor: %d, peso: %d\n", i+1, v[i], w[i]);
        }
    }*/

    free(w);
    free(v);
    free(melhor);
}
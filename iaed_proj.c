/*
* Fcheiro: proj1.c
* Autor: Daniel Nunes ist1103095
* Ambito: 2º Semestre Introdução aos Algoritmos e Estruturas de Dados
* Descricao: Sistema de gestão de voos
*/

/* Bibliotecas */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Definição de constantes */
#define DIM_ID          4 
#define DIM_PAIS        31
#define DIM_CIDADE      51
#define DIM_AEROPORTOS  40
#define DIM_CODIGO      7
#define DIM_VOOS        30000
#define DIM_VOO         45
#define CAPACIDADE_MAX  100
#define CAPACIDADE_MIN  10
#define DURACAO_MAX     12
#define MIN_MAX         59
#define HORAS_MAX       23
#define MES_MAX         12

/* Definição de Estruturas de Dados */
typedef struct {
    char id[DIM_ID]; /* vetor do id do aeroporto */
    char pais[DIM_PAIS]; /* vetor do pais do aeroporto */
    char cidade[DIM_CIDADE]; /* vetor da cidade do aeroporto */
} Aeroporto;

typedef struct {
    char codigo[DIM_CODIGO]; /* vetor do codigo do voo */
    char id_partida[DIM_ID]; /* vetor do id do aeroporto de partida do voo */
    char id_chegada[DIM_ID]; /* vetor do id do aeroporto de chegada do voo */
    int dia, mes, ano;
    int horas_p, minutos_p;
    int horas_d, minutos_d; 
    int capacidade;
} Voo;

/* variaveis globais */
/* as variaveis numero_aeroportos e numero_voos são aumentadas em um à medida 
que se criam aeroportos e voos, respetivamente */
int numero_aeroportos, numero_voos;
int dia = 1, mes = 1, ano = 2022; 
/* O vetor dia_do_mes serve para definir o dia maximo de cada mês */
int dia_do_mes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

Aeroporto aeroportos[DIM_AEROPORTOS];
Voo voos[DIM_VOOS];
Voo voos_aux[DIM_VOOS];
Voo voos_aux2[DIM_VOOS];

/* Prototipos das funcoes */
void adiciona_aeroporto(char id[], char pais[], char cidade[]);
void lista_aeroportos(char filtro[]);
void avanca_data();
void cria_voo();
void cria_voo2();
void lista_voos_p(char id_p[]);
void lista_voos_c(char id_c[]);


int main() {

    int i;
    char caracter;
    char comando;
    char id[DIM_ID] = {0};
    char pais[DIM_PAIS] = {0};
    char cidade[DIM_CIDADE];
    char filtro[DIM_ID * DIM_AEROPORTOS]; /* Vetor que serve para verificar se 
    ha alguma instrucao depois do comando */ 
    char id_p[DIM_ID]; /* Vetor ID de Partida */
    char id_c[DIM_ID]; /* Vetor ID de Chegada */
    


    scanf("%c", &comando);
    while(comando != 'q') {
        switch(comando) {
            case 'q': /* Comando que termina o programa */
                break;
            
            case 'a': /* Comando que adiciona um novo aeroporto ao sistema */
                i = 0;
    
                scanf("%s%s ", id, pais);
                while((caracter = getchar()) != '\n'){
                    cidade[i] = caracter;
                    i++;
                }
                cidade[i] = '\0';
                adiciona_aeroporto(id, pais, cidade);
                break;
                
            case 'l': /* Comando que lista os aeroportos */
                i = 0;
                
                while((caracter = getchar()) != '\n' &&
                i < DIM_ID * DIM_AEROPORTOS - 1) {
                    if(caracter != ' ') {
                        filtro[i] = caracter;
                        i++;
                    }
                } 
                filtro[i] = '\0';
                lista_aeroportos(filtro);
                break;
            
            case 'v': /* Comando que adiciona um voo ou lista todos os voos */

                caracter = getchar();
                if(caracter != '\n')
                    cria_voo();
                else 
                    cria_voo2();
                break;

            case 'p': /* Comando que lista os voos com partida de um aeroporto*/
                scanf("%s", id_p);
                lista_voos_p(id_p);
                break;

            case 'c': /* Comando que lista os voos com chegada a um aeroporto*/
                scanf("%s", id_c);
                lista_voos_c(id_c);
                break;
            
            case 't': /* Comando que avança a data do sistema */
                avanca_data();
                break;
        
        }
        scanf("%c", &comando);
    }
    return 0;
}


/* Função adiciona_aeroporto(char id[], char pais[], char cidade[]), 
correspondente ao comando a. Recebe um id, um pais e uma cidade e adiciona um
novo aeroporto ao sistema. */
void adiciona_aeroporto(char id[], char pais[], char cidade[]) {
    
    int i = 0;
    
    /* Verificar se o id do aeroporto é ou não uma string com maiusculas*/
    for(i = 0; i < DIM_ID; i++) 
        if(islower(id[i]) != 0) {
            printf("invalid airport ID\n");
            return;
        }
    
    /* Verifica se o aeroporto criado excede o limite de aeroportos */
    if(numero_aeroportos > DIM_AEROPORTOS - 1) {
        printf("too many airports\n");
        return;
    }

    /* Verifica se ja ha um aeroporto com o mesmo id */
    for(i = 0; i < numero_aeroportos; i++) 
        if(strcmp(id, aeroportos[i].id) == 0) {
            printf("duplicate airport\n");
            return;
        }

    strcpy(aeroportos[numero_aeroportos].id, id);
    strcpy(aeroportos[numero_aeroportos].pais, pais);
    strcpy(aeroportos[numero_aeroportos].cidade, cidade);
    
    printf("airport %s\n", aeroportos[numero_aeroportos].id);
    numero_aeroportos++;
     
}



/* Função lista_aeroportos(char filtro[]), correspondente ao comando l. Recebe 
ou não, um ou mais IDs de aeroportos e se não receber nenhum ID lista todos os 
aeroportos adicionados ao programa por ordem alfabetica, um por linha,
se receber, lista apenas os aeroportos que tenham os IDs recebidos. */
void lista_aeroportos(char filtro[]) {

    int i, j, voos = 0, contador_1 = 0, contador_erro = 0;
    int aux, dim_filtro;
    int lista_ordenada[DIM_AEROPORTOS];
    char id[DIM_AEROPORTOS][DIM_ID];

    
    dim_filtro = strlen(filtro);
    if(dim_filtro > 0) {

        for(i = 0; i < dim_filtro/3; i++)
            lista_ordenada[i] = i;

        for(i = 0; i < dim_filtro/3; i++)  {
            for(j = 0; j < DIM_ID-1; j++) {
                id[i][j] = filtro[contador_1];
                contador_1++;
            }
            id[i][j] = '\0';
        }

        /* Verifica se o aeroporto indicado existe ou não */   
        for(i = 0; i < dim_filtro/3; i++) {
            contador_erro = 0;
            for(j = 0; j < numero_aeroportos; j++) 
                if(strcmp(id[i], aeroportos[j].id) == 0) {
                    printf("%s %s %s %d\n", aeroportos[j].id,
                    aeroportos[j].cidade,
                    aeroportos[j].pais, voos);
                    contador_erro++;
                }   
            if(contador_erro == 0) 
                printf("%s: no such airport ID\n", id[i]);
                
        }
    }


    else {

        /* em cada posição do  vetor lista_ordenada estará o numero
        correspondente ao indice da posição do vetor */ 
        for(i = 0; i < numero_aeroportos; i++) 
            lista_ordenada[i] = i;
            
        for(i = 0; i < numero_aeroportos; i++) 
            for(j = 0; j < numero_aeroportos; j++)
                if((strcmp(aeroportos[lista_ordenada[i]].id, 
                aeroportos[lista_ordenada[j]].id)) < 0) {
                    aux = lista_ordenada[i];
                    lista_ordenada[i] = lista_ordenada[j];
                    lista_ordenada[j] = aux;
                }              

        /* Como se trocou o conteudo das posicoes da lista_ordenada apenas é
        preciso fazer print de cada posição da lista_ordenada nos vetores
        correspondentes ao id , cidade e pais */  
        for(i = 0; i < numero_aeroportos; i++)         
            printf("%s %s %s %d\n", aeroportos[lista_ordenada[i]].id,
            aeroportos[lista_ordenada[i]].cidade,
            aeroportos[lista_ordenada[i]].pais, voos);
    }

}



/*funcao cria_voo(), correspondente ao comando v. Cria um novo voo e aiciona-o
ao sistema.*/
void cria_voo() {

    char codigo[DIM_CODIGO];
    char id_partida[DIM_ID];
    char id_chegada[DIM_ID];
    int dia2, mes2, ano2;
    int horas_p, minutos_p, horas_d, minutos_d;
    int capacidade;
    int i, contador_erro1 = 0, soma = 0, dim, contador_erro2p = 0,
    contador_erro2d = 0;

    scanf("%s %s %s %d-%d-%d %d:%d %d:%d %d", codigo, id_partida, 
    id_chegada, &dia2, &mes2, &ano2, &horas_p, &minutos_p, &horas_d, 
    &minutos_d, &capacidade);

    for(i = 2; i < DIM_CODIGO; i++)
        if(codigo[i] == '\0' || codigo[i] == '\t') {
            dim = i;
            break;
        }

    for(i = 0; i < 2; i++) 
        if(islower(codigo[i]) != 0) 
            contador_erro1++;

    for(i = 2; i < dim; i++)
        soma = soma + codigo[i] - '0';
    
    /* Verifica se o codigo do voo é ou nao uma string com duas maiusculas e 
    dois digitos */
    if(soma < 1 || soma > 36) 
        contador_erro1++;
    if(contador_erro1 != 0) {
        printf("invalid flight code\n");
        return;
    }

    /* Verifica se o voo criado tem o mesmo codigo que algum dos voos que ja 
    tenham sido criados para o mesmo dia */
    for(i = 0; i < numero_voos; i++) 
        if((strcmp(codigo, voos[i].codigo)) == 0 && dia2 == voos[i].dia) {
            printf("flight already exists\n");
            return;
        }

    /* Verifica se o aeroporto existe ou não independentemente de ser um 
    aeroporto de partida ou de chegada */
    for(i = 0; i < DIM_AEROPORTOS; i++)
        if((strcmp(id_partida, aeroportos[i].id)) == 0) 
            contador_erro2p++;    

    if(contador_erro2p == 0) {
        printf("%s: no such airport ID\n", id_partida);
        return;
    }
    
    for(i = 0; i < DIM_AEROPORTOS; i++)   
        if ((strcmp(id_chegada, aeroportos[i].id)) == 0 ) 
            contador_erro2d++;
    
    if(contador_erro2d == 0) {
        printf("%s: no such airport ID\n", id_chegada);
        return;
    }

    /* Verifica se o voo criado excede o limite de voos */
    if(numero_voos > DIM_VOOS) {
        printf("too many flights\n");
        return;
    }

    /* Verifica se a data está no passado ou a mais de um ano no futuro */
    if(ano2 - ano < 0 || ano2 - ano > 1) {
        printf("invalid date\n");
        return;
    }
    if(ano2 == ano && mes2 - mes < 0) {
        printf("invalid date\n");
        return;
    }
    if(ano2 == ano && mes2 == mes && dia2 - dia < 0) {
        printf("invalid date\n");
        return;
    }
    if(ano2 - ano == 1 && mes2 - mes > 0) {
        printf("invalid date\n");
        return;
    }
    if(ano2 - ano == 1 && mes2 ==  mes && dia2 - dia > 0) {
        printf("invalid date\n");
        return;
    }
    
    /* Verifica se o voo tem ou não uma duração superior a 24 horas */
    if(horas_d > DURACAO_MAX) {
        printf("invalid duration\n");
        return;
    }
    if(horas_d == DURACAO_MAX && minutos_d > 0) {
        printf("invalid duration\n");
        return;
    }

    /* Verifica se a capacidade do voo está entre 10 e 100 */
    if(capacidade < CAPACIDADE_MIN || capacidade > CAPACIDADE_MAX) {
        printf("invalid capacity\n");
        return;
    }

    strcpy(voos[numero_voos].codigo, codigo);
    strcpy(voos[numero_voos].id_partida, id_partida);
    strcpy(voos[numero_voos].id_chegada, id_chegada);
    voos[numero_voos].dia = dia2;
    voos[numero_voos].mes = mes2;
    voos[numero_voos].ano = ano2;
    voos[numero_voos].horas_p = horas_p;
    voos[numero_voos].minutos_p = minutos_p;
    voos[numero_voos].horas_d = horas_d;
    voos[numero_voos].minutos_d = minutos_d;
    voos[numero_voos].capacidade = capacidade;

    numero_voos++;

}

/*funcao cria_voo2(), correspondente ao comando v. Lista os voos adicionados 
pela sua ordem de criação. */
void cria_voo2() {
    int i;

    for(i = 0; i < numero_voos; i++) 
        printf("%s %s %s %02d-%02d-%d %02d:%02d\n", voos[i].codigo,
        voos[i].id_partida, voos[i].id_chegada, 
        voos[i].dia, voos[i].mes, voos[i].ano,
        voos[i].horas_p, voos[i].minutos_p);

}


/*funcao lista_voos_p(char id_p[]), correspondente ao comando p. Recebe um ID de
um aeroporto e lista os voos com partida nesse aeroporto um por linha ordenados
pela data e hora de partida.*/
void lista_voos_p(char id_p[]) {
    int i, j,k = 0, contador_erro = 0, aux, data1, data2, hora1, hora2;
    int lista_ordenada[DIM_VOOS];

    /* Verifica se o aeroporto indicado existe ou não */
    for(i = 0; i < numero_aeroportos; i++)   
        if((strcmp(id_p, aeroportos[i].id)) == 0) {
            contador_erro++;
            break;
        }
    if(contador_erro == 0) {
        printf("%s: no such airport ID\n", id_p);
        return;
    }
    
    for(i = 0; i < numero_voos; i++) 
        if((strcmp(id_p, voos[i].id_partida)) == 0) {
            strcpy(voos_aux2[k].codigo,voos[i].codigo);
            strcpy(voos_aux2[k].id_partida,voos[i].id_partida);
            strcpy(voos_aux2[k].id_chegada,voos[i].id_chegada);
            voos_aux2[k].dia = voos[i].dia;
            voos_aux2[k].mes = voos[i].mes;
            voos_aux2[k].ano = voos[i].ano;
            voos_aux2[k].horas_p = voos[i].horas_p;
            voos_aux2[k].minutos_p = voos[i].minutos_p;
            k++;
        }

    /* em cada posição do  vetor lista_ordenada estará o numero
    correspondente ao indice da posição do vetor */
    for(i = 0; i < k; i++)
        lista_ordenada[i] = i;

    /* multiplicação do ano por 10000, do mes por 100 e soma do dia para ficar 
    com uma data do tipo AAAAMMDD e multiplicação das horas por 100 e soma dos 
    minutos para ficar com o tempo do tipo HHMM */       
    for(i = 0; i < k; i++) 
        for(j = 0; j < k; j++) {
            data1 = voos_aux2[lista_ordenada[i]].ano *10000 + 
            voos_aux2[lista_ordenada[i]].mes *100 + 
            voos_aux2[lista_ordenada[i]].dia;

            hora1 = voos_aux2[lista_ordenada[i]].horas_p *100 + 
            voos_aux2[lista_ordenada[i]].minutos_p;

            data2 = voos_aux2[lista_ordenada[j]].ano *10000 + 
            voos_aux2[lista_ordenada[j]].mes *100 + 
            voos_aux2[lista_ordenada[j]].dia;

            hora2 = voos_aux2[lista_ordenada[j]].horas_p *100 + 
            voos_aux2[lista_ordenada[j]].minutos_p;

            if(data1 < data2) {
                aux = lista_ordenada[i];
                lista_ordenada[i] = lista_ordenada[j];
                lista_ordenada[j] = aux;
            }
            if(data1 == data2) 
                if(hora1 < hora2) {
                    aux = lista_ordenada[i];
                    lista_ordenada[i] = lista_ordenada[j];
                    lista_ordenada[j] = aux;
                        
                }  
        }

    /* Como se trocou o conteudo das posicoes da lista_ordenada apenas é
    preciso fazer print de cada posição da lista_ordenada nos vetores da 
    estrutura voos_aux2 que sendo assim apareceram ordenados */
    for(i = 0; i < k; i++)  
        printf("%s %s %02d-%02d-%d %02d:%02d\n", 
        voos_aux2[lista_ordenada[i]].codigo, 
        voos_aux2[lista_ordenada[i]].id_chegada,
        voos_aux2[lista_ordenada[i]].dia, voos_aux2[lista_ordenada[i]].mes,
        voos_aux2[lista_ordenada[i]].ano, voos_aux2[lista_ordenada[i]].horas_p,
        voos_aux2[lista_ordenada[i]].minutos_p);
        
}


/*funcao lista_voos_c(char id_c[]), correspondente ao comando c. Recebe um ID de
um aeroporto e lista os voos com chegada a esse aeroporto um por linha ordenados
pela data e hora de chegada, ou seja a hora de partida mais a duração do voo.*/
void lista_voos_c(char id_c[]) {
    int i, j, k=0,  contador_erro = 0, aux, data1, data2, hora1, hora2;
    int lista_ordenada[DIM_VOOS];
    int horas_novo, minutos_novo;

    

    for(i = 0; i < numero_voos; i++) 
        if((strcmp(id_c, voos[i].id_chegada)) == 0) {
            strcpy(voos_aux[k].codigo,voos[i].codigo);
            strcpy(voos_aux[k].id_partida,voos[i].id_partida);
            strcpy(voos_aux[k].id_chegada,voos[i].id_chegada);
            voos_aux[k].dia = voos[i].dia;
            voos_aux[k].mes = voos[i].mes;
            voos_aux[k].ano = voos[i].ano;
            voos_aux[k].horas_p = voos[i].horas_p;
            voos_aux[k].minutos_p = voos[i].minutos_p;
            voos_aux[k].horas_d = voos[i].horas_d;
            voos_aux[k].minutos_d = voos[i].minutos_d;
            k++;
        }

    /* Verifica se o aeroporto indicado existe ou não */
    for(i = 0; i < numero_aeroportos; i++)   
        if((strcmp(id_c, aeroportos[i].id)) == 0) {
            contador_erro++;
            break;
        }
    if(contador_erro == 0) {
        printf("%s: no such airport ID\n", id_c);
        return;
    }

    /* soma das horas e minutos aumentando o dia em um quando necessario */
    for(i = 0; i < k; i++) {
        horas_novo = voos_aux[i].horas_p + voos_aux[i].horas_d;
        minutos_novo = voos_aux[i].minutos_p + voos_aux[i].minutos_d;
        if(minutos_novo > MIN_MAX) {
            horas_novo++;
            minutos_novo = minutos_novo - (MIN_MAX +1);
        }
        if(horas_novo > HORAS_MAX) {
            voos_aux[i].dia++;
            horas_novo = horas_novo - (HORAS_MAX +1);
        }
        voos_aux[i].horas_p = horas_novo;
        voos_aux[i].minutos_p = minutos_novo;
    }

    /* correção do dia, mes e ano quando necessario */
    for(i = 0; i < k; i++) {
        if(voos_aux[i].mes != MES_MAX && voos_aux[i].dia > 
        dia_do_mes[voos_aux[i].mes -1 ]) {
            voos_aux[i].dia = 1;
            voos_aux[i].mes++;
        }
        else if(voos_aux[i].mes == MES_MAX && voos_aux[i].dia > 
        dia_do_mes[MES_MAX -1 ] ) {
            
            voos_aux[i].dia = 1;
            voos_aux[i].mes = 1;
            voos_aux[i].ano++;
            
        }
    }

    /* em cada posição do  vetor lista_ordenada estará o numero
    correspondente ao indice da posição do vetor */
    for(i = 0; i < k; i++)
        lista_ordenada[i] = i;

    /* multiplicação do ano por 10000, do mes por 100 e soma do dia para ficar 
    com uma data do tipo AAAAMMDD e multiplicação das horas por 100 e soma dos 
    minutos para ficar com o tempo do tipo HHMM */        
    for(i = 0; i < k; i++) 
        for(j = 0; j < k; j++) {
            data1 = voos_aux[lista_ordenada[i]].ano *10000 + 
            voos_aux[lista_ordenada[i]].mes *100 + 
            voos_aux[lista_ordenada[i]].dia;

            hora1 = voos_aux[lista_ordenada[i]].horas_p *100 + 
            voos_aux[lista_ordenada[i]].minutos_p;

            data2 = voos_aux[lista_ordenada[j]].ano *10000 + 
            voos_aux[lista_ordenada[j]].mes *100 + 
            voos_aux[lista_ordenada[j]].dia;

            hora2 = voos_aux[lista_ordenada[j]].horas_p *100 + 
            voos_aux[lista_ordenada[j]].minutos_p;
        
            if(data1 < data2) {
                aux = lista_ordenada[i];
                lista_ordenada[i] = lista_ordenada[j];
                lista_ordenada[j] = aux;
            }
            else if(data1 == data2) 
                if(hora1 < hora2) {
                    aux = lista_ordenada[i];
                    lista_ordenada[i] = lista_ordenada[j];
                    lista_ordenada[j] = aux;         
                }  
        }         

    for(i = 0; i < k; i++) {
        printf("%s %s %02d-%02d-%d %02d:%02d\n", 
        voos_aux[lista_ordenada[i]].codigo, 
        voos_aux[lista_ordenada[i]].id_partida,
        voos_aux[lista_ordenada[i]].dia, voos_aux[lista_ordenada[i]].mes,
        voos_aux[lista_ordenada[i]].ano, voos_aux[lista_ordenada[i]].horas_p,
        voos_aux[lista_ordenada[i]].minutos_p);
    }
        
}

/*funcao avanca_data(), correspondente ao comando t. Recebe uma nova data e 
atualiza o sistema para a nova data passando a ser a data atual.*/
void avanca_data() {
    int diat, mest, anot;
    scanf("%d-%d-%d", &diat, &mest, &anot);
    /* a data nao pode ser do passado ou esatr a mais de um ano no futuro */
    if(anot - ano < 0 || anot - ano > 1) {
        printf("invalid date\n");
        return;
    }
    if(anot == ano && mest - mes < 0) {
        printf("invalid date\n");
        return;
    }
    if(anot == ano && mest == mes && diat - dia < 0) {
        printf("invalid date\n");
        return;
    }
    if(anot - ano == 1 && mest - mes > 0) {
        printf("invalid date\n");
        return;
    }
    if(anot - ano == 1 && mest ==  mes && diat - dia > 0) {
        printf("invalid date\n");
        return;
    }

    else {
        ano = anot;
        mes = mest;
        dia = diat;
        printf("%02d-%02d-%d\n", dia, mes, ano);
    }

}
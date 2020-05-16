/*
  Universidade de Brasilia 
  Instituto de Ciencias Exatas
  Departamento de Ciencia da Computacao

  Algoritmos e Programacao de Computadores

  Aluno: Lucas Correa Lemos
  Matricula: 17/0149412
  Turma: A
  Versao do compilador: gcc 5.4.0 20160609
*/

/* bibliotecas */
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<ctype.h>
#include<string.h>

/* defines */
#define RED "\033[0;31m"
#define NONE   "\033[0m"
#define CYAN  "\033[1;36m"
#define GREEN "\033[0;32m"

/* clear de acordo com o sistema operacional */
#ifdef _WIN32
#define CLEAR  system("cls");
#else
#define CLEAR  system("clear");
#endif

/* struct para as coordenadas dos inimigosX */
typedef struct{
    int i,j;
}inimigoX;

/* struct para a avaliacao do melhor movimento para X */
typedef struct{
    int i,j,avaliacao;
}tipoMovimentoX;

/* struct para as coordenadas dos inimigosB */
typedef struct{
    int i,j;
}inimigoB;

/* struct para gravar o jogador corrente no ranking de jogadores */
typedef struct {
    char nome[7];
    int pontos;
    int movimentos;
}player;

/*variaveis globais*/

char campo[50][50]; /* grid do jogo */

inimigoX posicaoX[100]; /* vetor do tipo inimigoX, para guardar o par de coordenadas de cada X no mapa */
inimigoB posicaoB[100]; /* vetor do tipo inimigoB, para guardar o par de coordenadas de cada B no mapa */

int quantO=0,quantB=0,quantX=0; /* qtd de objetos que tera o mapa */
int tamanhoB=0,tamanhoQ=0; /* tamanho do rastro de B e tamanho da explosao de Q */
int largura=0,altura=0; /* dimensoes da grid */

char personagem = 'C'; /*personagem */
char inimigoPonto = 'O'; /*inimigo inimigoPonto */
char inimigoPassivo = 'B'; /*inimigo passivo */
char inimigoBomba = 'Q'; /*inimigo bomba */
char inimigoAgressivo = 'X'; /* inimigo agressivo */

int r=0,s=0; /*coordenadas do inimigo bomba */
int x=0,y=0; /*coordenadas do personagem */
int countB=0; /*contador para o guardar o numero de inimigosB´s no mapa*/
int nivelDeJogo=2; /*dificuldade do jogo: 1 - facil 2 - medio 3 - dificil. por padrao, o jogo inicia no medio*/
int rankeado=0; /*0 - rankeado desativado 1 - rankeado ativado*/
player jogador[10]; /*vetor de jogadores para gerar o ranking, no caso um top10 */
player aux1; /*auxiliar para gravar o jogador corrente no modo rankeado*/

/*assinaturas das funcoes */
void mostraCampo(int *movimentos,int *pontos);
void criaCampo();
void partida();
int moverPersonagem(char move,int *morreuX,int *pontos);
void moverB();
void moverXseguindo(int *morreuX);
void moverXaleatorio(int *morreuX);
void bombaColocada(int*bomba,int *tempoB);
void fim(int *morreuX,int*morreuB,int *movimentos,int *pontos);
void bombaExplode(int *bomba,int *morreuB);
void limpaBomba();
void instrucoes();
void gravaRanking();
void ranking();
void leArquivo();
void configuracoes();
void verificaArquivo();
int arquivoVazio(const char* file_name);

/*menu do jogo*/
int menuMain(){
    
    int opcao;
    CLEAR;
    
    printf("%sSnakeBomber APC\n\n",CYAN);
    
    /*menu com as opcoes do jogo */
    printf("%s1 - Jogar\n",NONE);
    printf("2 - Configuracoes\n");
    printf("3 - Ranking\n");
    printf("4 - Instrucoes\n");
    printf("5 - Sair\n\n");

    /*escolhe uma opcao disponivel no menu*/
    printf("Escolha uma opcao:");
    scanf("%d",&opcao);
    
    /*um retorno para cada opcao escolhida*/
    if(opcao==1){
        
        return opcao;

    }else if(opcao==2){
        
        return opcao;

    }else if(opcao==3){
        
        return opcao;
    
    }else if(opcao==4){
        
        return opcao;

    }else{
        
        return opcao;
        
    }

}

/*funcao que verifica se um arquivo esta vazio ou nao*/
int arquivoVazio(const char* file_name){
    
    FILE *file = fopen(file_name, "r");

    if(file == NULL)
        return 0;

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fclose(file);

    return size;
}

/*funcao que grava um jogador do ranking de jogadores.
Caso ele não entre no top10, não será incluído no ranking.*/
void gravaRanking(){

    int i,j,achou;
    int auxPontos=0,auxMovimentos=0;
    char auxNome[7];

    FILE *fp_ranking;

    /*teste se o arquivo nao esta vazio*/
    /*caso NAO esteja vazio, entao verifica se pode ou nao inserir um novo jogador no arquivo ranking */
    if(arquivoVazio ("ranking.bin") > 0){

            /*abre o arquivo de ranking*/
            fp_ranking=fopen("ranking.bin","r+b");

            i=0;
        
            /*le o arquivo ranking, armazenando no vetor jogador*/
            while((fread(&jogador[i], sizeof(jogador[i]), 1, fp_ranking) != 0)){
               
                i++;
            }
            
            fclose(fp_ranking);

            /*procura um espaco vago no vetor*/
            i=0; achou=0;
            do{
        
                if(jogador[i].pontos==-1){
        
                   strcpy(jogador[i].nome,aux1.nome);
                   jogador[i].pontos=aux1.pontos;
                   jogador[i].movimentos=aux1.movimentos;
                   
                   achou=1;
        
                }
            i++;
        
            }while(achou == 0 && i < 10);

            /*se nao achou espaco vago, entao testa com o ultimo indice*/
            if(achou==0){

                /*caso os pontos do novo registro seja igual aos pontos do ultimo no vetor*/
                if(aux1.pontos == jogador[9].pontos){
                    
                    /*grava no vetor o jogador com mais movimentos restantes (criterio de desempate)*/
                    /*caso os movimentos tambem empatem, o novo jogador nao entra no ranking*/
                    if(aux1.movimentos > jogador[9].movimentos){

                        strcpy(jogador[9].nome,aux1.nome);
                        jogador[9].pontos=aux1.pontos;
                        jogador[9].movimentos=aux1.movimentos;
                        
        
                    }
        
                }
                /*caso a pontuacao do novo registro seja maior que a pontuacao do ultimo no vetor
                entao o novo registro entra no ranking*/
                else if(aux1.pontos > jogador[9].pontos){
                        
                        strcpy(jogador[9].nome,aux1.nome);
                        jogador[9].pontos=aux1.pontos;
                        jogador[9].movimentos=aux1.movimentos;
                        

                }
            
            }
            
            /*ordena o vetor jogador em ordem decrescente, para depois gravar no arquivo ranking*/
            for( i = 0; i < 10; i++ ){

              for( j = i + 1; j < 10; j++ ) {
                
                /*ordenando por quem tem maior pontuacao*/
                if ( jogador[j].pontos > jogador[i].pontos ){

                   /*mudando os pontos*/
                   auxPontos = jogador[j].pontos;

                   jogador[j].pontos = jogador[i].pontos;

                   jogador[i].pontos = auxPontos;
                    
                   /*mudando os movimentos*/
                   auxMovimentos = jogador[j].movimentos;

                   jogador[j].movimentos =  jogador[i]. movimentos;

                   jogador[i].movimentos=auxMovimentos;

                   /*mudando o nome*/
                   strcpy(auxNome,jogador[j].nome);
            
                   strcpy(jogador[j].nome,jogador[i].nome);
                   
                   strcpy(jogador[i].nome,auxNome);

                }

                /*ordenando caso os pontos sejam iguais. neste caso, a quantidade de movimentos restantes sera o desempate*/
                /*caso persista o empate, o novo registro nao entra no vetor*/
                else if ( jogador[j].pontos == jogador[i].pontos ){
                    
                    if(jogador[j].movimentos > jogador[i].movimentos){
                    
                                       /*mudando os pontos*/
                                       auxPontos = jogador[j].pontos;
                    
                                       jogador[j].pontos = jogador[i].pontos;
                    
                                       jogador[i].pontos = auxPontos;
                    
                    
                                       /*mudando os movimentos*/
                                       auxMovimentos= jogador[j].movimentos;
                    
                                       jogador[j].movimentos =  jogador[i]. movimentos;
                    
                                       jogador[i].movimentos=auxMovimentos;
                    
                    
                                       /*mudando o nome*/
                                       strcpy(auxNome,jogador[j].nome);
                    
                                       strcpy(jogador[j].nome,jogador[i].nome);
                    
                                       strcpy(jogador[i].nome,auxNome);
                }
            }
        }
    }


            /*grava no arquivo o vetor de jogadores, referente ao ranking*/
            fp_ranking = fopen("ranking.bin", "r+b");
            
                        for(i = 0; i < 10; i++){
                            fwrite(&jogador[i], sizeof(jogador[i]), 1, fp_ranking);
                        }
                        fclose(fp_ranking);


        /*caso o arquivo ranking esteja vazio, apenas grava no primeiro lugar*/
        }else{

            i=0;
                    
            strcpy(jogador[i].nome,aux1.nome);
            jogador[i].pontos=aux1.pontos;
            jogador[i].movimentos=aux1.movimentos;

            fp_ranking = fopen("ranking.bin", "r+b");
                        
            fwrite(&jogador[i], sizeof(jogador[i]), 1, fp_ranking);
                        
            fclose(fp_ranking);

        }

}

/*funcao que mostra o ranking*/
void ranking(){
        
    FILE *fp_ranking;
    int i=0;
    CLEAR;

        /*abre o arquivo ranking*/
        fp_ranking=fopen("ranking.bin","r+b");

        printf("%sRanking SnakeBomber APC\n\n",CYAN);
        printf("%s",NONE);

        printf("%sPosicao\tNome\tPontos\tJogadas\n\n",RED);

        printf("%s",NONE);

            /*le o arquivo ranking*/
            while( (fread(&jogador[i], sizeof(jogador[i]), 1, fp_ranking) != 0) ){

            /*imprime apenas as posicoes que nao sao vazias (!=-1)*/
            if(jogador[i].pontos !=-1){
                
                printf(" %d \t%s\t%d\t%d\n\n",i+1,jogador[i].nome,jogador[i].pontos,jogador[i].movimentos);
            
            }
            i++;
            
        }

        printf("%sAperte ENTER para voltar ao menu",CYAN);
        printf("%s",NONE);
        getchar();
        getchar();

}

/*funcao que le o arquivo de configuracoes para gerar um novo jogo*/
void leArquivo(){
    
    FILE *fp_config;

        /*abre o arquivo de configuracoes*/
        fp_config=fopen("config.txt","r");
    
            /*le o arquivo*/
            /*altura, largura, quantB, quantX, qtdO, tamanhoB, tamanhoQ, dificuldade*/ 
            fscanf(fp_config,"%d %d %d %d %d %d %d %d",&altura,&largura,&quantB,&quantX,&quantO,&tamanhoB,&tamanhoQ,&nivelDeJogo);
            
            /*fecha o arquivo*/
            fclose(fp_config);
}

/*cria o campo e insere o jogador e os monstros*/
void criaCampo(){
    int i,j,cont;
    
    /*le o arquivo com as configuracoes gravadas*/
    leArquivo();

        /*cria o campo do jogo vazio com altura e largura informadas no arquivo de configurações lido */
        for(i=0;i<altura;i++){

            for(j=0;j<largura;j++){
                campo[i][j]='.';
                
            }
            printf("\n");
        }
    
        /*insercao do personagem*/
            x=rand()%altura;
            y=rand()%largura;
                    
            campo[x][y]=personagem; /* nao preciso verificar as coordenadas geradas para o personagem, ele sera o primeiro na insercao */
                    
            /*insercao do inimigo passivo B*/
            cont=0;
            while(cont<quantB){
                i=rand()%altura;
                j=rand()%largura;
                    if(campo[i][j]=='.'){
                        campo[i][j]=inimigoPassivo;
                        cont++;
                    }
            }
            
            /*insercao do inimigo agressivo X*/
            cont=0;
            while(cont<quantX){
                i=rand()%altura;
                j=rand()%largura;
                    if(campo[i][j]=='.'){
                        campo[i][j]=inimigoAgressivo;
                        cont++;
                    }
            }
            i=0;j=0;
            
            /*insercao do inimigo inimigoPonto O*/
            cont=0;
            while(cont<quantO){
                i=rand()%altura;
                j=rand()%largura;
                    if(campo[i][j]=='.'){
                        campo[i][j]=inimigoPonto;
                        cont++;

                    }
            }

            /*procura os inimigos B no mapa e aloco no vetor 'posicaoB' para fazer os movimentos depois*/
            for(i=0;i<altura; i++){
                for(j=0;j<largura;j++){
                    if(campo[i][j]==inimigoPassivo){
                        posicaoB[countB].i=i;
                        posicaoB[countB].j=j;
                        
                        countB++;
                    }
                }

            }
}

/*funcao para mostrar o campo*/
 void mostraCampo(int *movimentos,int *pontos){
    int i,j;
        CLEAR;
        printf("Pontos : %d    Movimentos restantes : %d\n",*pontos,*movimentos);
        
            for(i=0;i<altura;i++){
                for(j=0;j<largura;j++){
                    printf("%c",campo[i][j]);
                }
                    printf("\n");
            }

 }

/*funcao onde inicia e acontece o jogo*/ 
void partida(){
    char move;
    int respMove=0,aux=0;
    int tempoExplosao=-1,bomba=0;
    int bombaAparece=-1;
    int morreuB=0,morreuX=0;
    int movimentos=0;
    int pontos=0;
    
    criaCampo(); /*cria o campo do jogo */

    movimentos=altura*largura;

    /*fazendo o movimento do personagem,monstros e bomba
    Os monstros só fazem seus movimentos após o personagem fazer o seu antes.*/
    do{

        /*mostra o campo com o personagem e os monstros ja inseridos,
        bem como qtd de pontos e movimentos restantes*/
        mostraCampo(&movimentos,&pontos);

        /*le o movimento do personagem */
        getchar();
        printf("Movimento:");
        scanf("%c",&move);
        
        /*caso uma bomba nao exista, entao sorteia uma qtd de turnos para ela aparecer*/
        /*no minimo 1 turno, no maximo 5*/
        if(bomba!=1){

            bombaAparece=1+(rand()%5);
            limpaBomba(); 
        
        }

        /*pega o retorno da funcao moverPersonagem */
        respMove=moverPersonagem(move,&morreuX,&pontos);

        /*se retornar 1, o personagem andou pra alguma direcao possivel. se retornar 2, ele andou e pegou um inimigoPonto no mapa. */
        if(respMove ==1  || respMove==2){
            
            movimentos--;/*entao decrementa os movimentos restantes */

            /*movimenta os monstros de acordo com a dificuldade configurada*/
            switch(nivelDeJogo){
                
                /*nivel facil, o inimigo X sempre anda aleatorio*/
                /*o inimigo X pode matar o personagem tambem no caso de andar aletoriamente em todas as dificuldades, caso o personagem ande pra mesma casa que o inimigo X*/
                case 1:
                
                    moverB();
                    moverXaleatorio(&morreuX); 
                    
                    
                    break;

                /* nivel medio, proporcao de 50/50 do inimigo X de andar seguindo o personagem ou andar aleatorio, respectivamente*/
                case 2:

                    aux=rand()%1;

                    if(aux==0){
                        
                        moverXaleatorio(&morreuX); 
                        moverB();

                
                    }else{
                        
                        moverXseguindo(&morreuX); 
                        moverB();

                    }

                    break;

                /*nivel dificil, proporcao de 70/30 do inimigo X andar seguindo o personagem ou andar aleatorio, respectivamente*/
                case 3:

                    aux=rand()%4;

                    if(aux<=2){
                        
                        moverXseguindo(&morreuX); 
                        moverB();

                    }else{
                        
                        moverXaleatorio(&morreuX); 
                        moverB();
                    }

                break;

            }
        }

        /*verificando inimigoBomba*/
        /*se o valor de bombaAparece for maior que zero, significa que foi sorteado uma qtd de turnos, logo nao há uma bomba no mapa ainda*/
            if(bombaAparece>0){
                bombaAparece--; /*turnos restantes para a bomba aparecer decrementa*/
            
            /*se a qtd de turnos chegar a zero, entao a bomba eh colocada no mapa*/
            if (bombaAparece==0){
                
                /*funcao que coloca a bomba no mapa, 
                passando o tempo para explosao e uma flag chamada bomba, sinalizando que uma bomba foi colocada no mapa*/
                bombaColocada(&tempoExplosao,&bomba);
                
            }
            
            /*caso quando ja existe uma bomba no mapa*/
            }else if(bomba==1){
                
                tempoExplosao--;/*decrementa o qtd de turnos para a bomba explodir*/
                
                /*caso quando o tempo para a explosao da bomba chega a zero*/
                if(tempoExplosao==0){
                     
                    bombaExplode(&bomba,&morreuB);/*bomba explode*/
                   
                }   
            }
            
        }while(movimentos>0 && quantO>0 && morreuX==0 && morreuB==0); /*jogo continua enquanto a condicao seja verdadeira */

        /*quando a condicao acima nao for mais satisfeita, entao o jogo terminou*/ 

        mostraCampo(&movimentos,&pontos);/* mostra o campo*/
        
        fim(&morreuB,&morreuX,&movimentos,&pontos);/*quando terminar o jogo, verifica o motivo do termino*/

        /*se o modo rankeado estiver ativado, entao tenta gravar o registro da partida no ranking*/
        if(rankeado==1){

            /*auxiliar para guardar os pontos e movimentos do jogador corrente*/
            aux1.pontos = pontos;
            aux1.movimentos = movimentos;

            /*funcao que vai entao tentar gravar o jogador no ranking*/
            gravaRanking();
        
        }

        /*zera algumas variaveis globais*/
        rankeado=0;
        pontos=0;
        countB=0;   
}

/*funcao que movimenta o inimigo X aleatorio*/
void moverXaleatorio(int  *morreuX){

    int k,i,j,aux=0;
    int countX=0; /*contador para o guardar o numero de inimigos 'X' no mapa*/

    /* Procura os inimigos X e aloca em um vetor auxiliar*/
    for(i=0;i<altura; i++){
        for(j=0;j<largura;j++){
        
            if(campo[i][j]==inimigoAgressivo){
                posicaoX[countX].i=i;
                posicaoX[countX].j=j;
                countX++;
            }
        }
     }
    
    /*Percorre o vetor posicaoX do tipo inimigoX e entao muda a posicao de cada monstro*/
    for(k=0;k<countX; k++){
    
        aux=rand()%4;/*sorteio um movimento para X e se puder fazer o movimento, entao ele o faz */
            
                switch(aux){

                    case 0:

                        /*caso em que o inimigo X anda normalmente para uma coordenada vazia*/
                        if(campo[posicaoX[k].i][posicaoX[k].j-1]=='.'  && posicaoX[k].j-1>=0){ /*esquerda */
                        
                            campo[posicaoX[k].i][posicaoX[k].j-1]=inimigoAgressivo;
                            campo[posicaoX[k].i][posicaoX[k].j]='.';

                            /*atualiza apenas a coordenada j, pois andou para esquerda*/
                            posicaoX[k].j=posicaoX[k].j-1;
                    
                        /*caso em que o inimigo X mata o personagem*/
                        }else if(campo[posicaoX[k].i][posicaoX[k].j-1]==personagem  && posicaoX[k].j-1>=0){

                            campo[posicaoX[k].i][posicaoX[k].j-1]='*';
                            
                            /*a flag entao atualiza, indicando que o inimigo X matou o personagem*/
                            *morreuX=1;
                        }
                        
                    break;
                    
                    case 1:

                        /*caso em que o inimigo X anda normalmente para uma coordenada vazia*/
                        if (campo[posicaoX[k].i][posicaoX[k].j+1]=='.' && posicaoX[k].j+1<largura){ /*direita */
                        
                            campo[posicaoX[k].i][posicaoX[k].j+1]=inimigoAgressivo;
                            campo[posicaoX[k].i][posicaoX[k].j]='.';

                            /*atualiza apenas a coordenada j, pois andou para direita */
                            posicaoX[k].j=posicaoX[k].j+1;
                       
                        /*caso em que o inimigo X mata o personagem*/
                        }else if(campo[posicaoX[k].i][posicaoX[k].j+1]==personagem && posicaoX[k].j+1<largura){

                            campo[posicaoX[k].i][posicaoX[k].j+1]='*';

                            /*a flag entao atualiza, indicando que o inimigo X matou o personagem*/
                            *morreuX=1;
                            
                        }

                    break;

                    case 2:
                        
                        /*caso em que o inimigo X anda normalmente para uma coordenada vazia*/
                        if(campo[posicaoX[k].i+1][posicaoX[k].j]=='.' && posicaoX[k].i+1<altura){ /* baixo */
                        
                            campo[posicaoX[k].i+1][posicaoX[k].j]=inimigoAgressivo;
                            campo[posicaoX[k].i][posicaoX[k].j]='.';
                         
                            /*atualiza apenas a coordenada i, pois andou para baixo*/
                            posicaoX[k].i=posicaoX[k].i+1;

                          /*caso em que o inimigo X mata o personagem*/  
                        }else if(campo[posicaoX[k].i+1][posicaoX[k].j]==personagem && posicaoX[k].i+1<altura){

                            campo[posicaoX[k].i+1][posicaoX[k].j]='*';

                            /*a flag entao atualiza, indicando que o inimigo X matou o personagem*/
                            *morreuX=1;

                        }
                    break;

                    case 3:

                        /*caso em que o inimigo X anda normalmente para uma coordenada vazias*/
                        if(campo[posicaoX[k].i-1][posicaoX[k].j]=='.' && posicaoX[k].i-1>=0){ /*cima */
                        
                            campo[posicaoX[k].i-1][posicaoX[k].j]=inimigoAgressivo;
                            campo[posicaoX[k].i][posicaoX[k].j]='.';

                            /*atualiza entao a coordenada i, pois andou para cima*/
                            posicaoX[k].i=posicaoX[k].i-1;

                    /*caso em que inimigo X mata o personagem*/
                    }else if (campo[posicaoX[k].i-1][posicaoX[k].j]==personagem && posicaoX[k].i-1>=0){

                            campo[posicaoX[k].i-1][posicaoX[k].j]='*';
                            
                            /*a flag entao atualiza, indicando que o inimigo X matou o personagem*/
                            *morreuX=1;

                    }
                    break;

                    default: /*um dos casos é inclusive o inimigo X nao realizar nenhum movimento */
                            campo[posicaoX[k].i][posicaoX[k].j]=inimigoAgressivo;
                            posicaoX[k].i=posicaoX[k].i;
                            posicaoX[k].j=posicaoX[k].j;
                    
                    break;


                }   
            
      }

}

/*funcao que faz o inimigo X perseguir o personagem */
void moverXseguindo(int *morreuX){
    
    int k,i,j;
    int countX=0; /*contador para o guardar o numero de Xs no mapa*/

     /* Procura os inimigosX e aloca em um vetor auxiliar*/
     for(i=0;i<altura; i++){
        for(j=0;j<largura;j++){
             if(campo[i][j]==inimigoAgressivo){
                posicaoX[countX].i=i;
                posicaoX[countX].j=j;
                countX++;
            }
        }
     }
    

    /*Percorre o vetor posicao do tipo InimigoX e entao muda a posicao de cada monstro.*/
    for(k=0;k<countX; k++){

                 /*Vetor de possiveis movimentos de X, sao possiveis 4 movimentos*/
                 tipoMovimentoX movimentoX[4];

                 /*esquerda*/
                 movimentoX[0].i=posicaoX[k].i;
                 movimentoX[0].j=posicaoX[k].j-1;
                 movimentoX[0].avaliacao=1000;
                 
                 /*direita*/
                 movimentoX[1].i=posicaoX[k].i;
                 movimentoX[1].j=posicaoX[k].j+1;
                 movimentoX[1].avaliacao=1000;

                 /*cima*/
                 movimentoX[2].i=posicaoX[k].i-1;
                 movimentoX[2].j=posicaoX[k].j;         
                 movimentoX[2].avaliacao=1000;
                 
                 /*baixo*/
                 movimentoX[3].i=posicaoX[k].i+1;
                 movimentoX[3].j=posicaoX[k].j;    
                 movimentoX[3].avaliacao=1000;

                /* Quanto menor a avaliacao significa melhor jogada, quanto maior, o contrario.*/
                
                /*A avaliacao ira fazer uma diferenca do i e j do  personagem C com relacao ao i e j do monstro X 
                Depois ira somar o modulo das diferencas e entao a soma corresponde a avaliacao*/
               
                /*Em cada possivel jogada: 
                dif_c_i_j: corresponde as diferencas de c para X */

                if((campo[posicaoX[k].i][posicaoX[k].j-1]=='.' || campo[posicaoX[k].i][posicaoX[k].j-1]==personagem)  && posicaoX[k].j-1>=0){ /*esquerda */
                        
                    int dif_c_i, dif_c_j;
                            
                        dif_c_i=abs(x - posicaoX[k].i);
                        dif_c_j=abs(y - (posicaoX[k].j-1));
                        movimentoX[0].avaliacao = dif_c_i + dif_c_j;
                    
                }

                if ( (campo[posicaoX[k].i][posicaoX[k].j+1]=='.' || campo[posicaoX[k].i][posicaoX[k].j+1]==personagem) && posicaoX[k].j+1<largura){ /*direita */
                        
                    int dif_c_i, dif_c_j;
                            
                            dif_c_i=abs(x - posicaoX[k].i);
                            dif_c_j=abs(y - (posicaoX[k].j+1));
                            movimentoX[1].avaliacao = dif_c_i + dif_c_j;
                }

                if( (campo[posicaoX[k].i+1][posicaoX[k].j]=='.' || campo[posicaoX[k].i+1][posicaoX[k].j]==personagem) && posicaoX[k].i+1<altura){ /* baixo */
                        
                    int dif_c_i, dif_c_j;
                            
                            dif_c_i=abs(x - posicaoX[k].i+1);
                            dif_c_j=abs(y - (posicaoX[k].j));
                            movimentoX[2].avaliacao = dif_c_i + dif_c_j;
                }

                if( (campo[posicaoX[k].i-1][posicaoX[k].j]=='.' || campo[posicaoX[k].i-1][posicaoX[k].j]==personagem) && posicaoX[k].i-1>=0){ /*cima */
                        
                            int dif_c_i, dif_c_j;

                            dif_c_i=abs(x - posicaoX[k].i-1);
                            dif_c_j=abs(y - (posicaoX[k].j));
                            movimentoX[3].avaliacao = dif_c_i + dif_c_j;
                }
                        
                /*Percorrer agora o vetor de possiveis jogadas e entao identificar a menor avaliacao*/
                        int m=0;
                        int menor;
                        int indice_posicao=0;
                        menor=1000;

                        for(m=0;m<4;m++){
                            if(movimentoX[m].avaliacao<menor){
                                indice_posicao=m;
                                menor=movimentoX[m].avaliacao;
                            }
                        }
                        
                /*Movimentando o inimigo X de acordo com a melhor avaliacao*/

                /*caso em que o inimigo X mata o personagem*/
                if(campo[movimentoX[indice_posicao].i][movimentoX[indice_posicao].j]==personagem){
                            
                    campo[posicaoX[k].i][posicaoX[k].j]=inimigoAgressivo;
                    campo[movimentoX[indice_posicao].i][movimentoX[indice_posicao].j]='*';

                    *morreuX=1;

                }
                /*caso em que o inimigo X anda seguindo normalmente */
                else if(campo[movimentoX[indice_posicao].i][movimentoX[indice_posicao].j]=='.'){
                    
                        campo[movimentoX[indice_posicao].i][movimentoX[indice_posicao].j]=inimigoAgressivo;
                        campo[posicaoX[k].i][posicaoX[k].j]='.';   
                }
        }

}

/*funcao que movimenta o inimigo B*/
void moverB(){
    int aux=0,k;
     
      /*Percorre o vetor posicaoB do tipo inimigoB e entao muda a posicao de cada monstro.*/
      for(k=0;k<countB; k++){
          
          aux=rand()%4; /*sorteio um movimento para B e se puder fazer o movimento, entao ele o faz*/
                  
              switch(aux){
      
                  case 0:
                      
                      if(campo[posicaoB[k].i][posicaoB[k].j-1]=='.' && posicaoB[k].j-1>=0){/*esquerda*/

                          campo[posicaoB[k].i][posicaoB[k].j-1]=inimigoPassivo;
                        
                          /*atualiza a coordenada j, pois andou para esquerda*/
                          posicaoB[k].j=posicaoB[k].j-1;
                       
                      }

                      break;
                          
                  case 1:
                   
                      if (campo[posicaoB[k].i][posicaoB[k].j+1]=='.' && posicaoB[k].j+1<largura){/*direita*/

                              campo[posicaoB[k].i][posicaoB[k].j+1]=inimigoPassivo;
                              
                              /*atualiza a coordenada j, pois andou para direita*/
                              posicaoB[k].j=posicaoB[k].j+1;

                      }

                      break;
      
                  case 2:
                      
                      if(campo[posicaoB[k].i+1][posicaoB[k].j]=='.' && posicaoB[k].i+1<altura){ /*baixo*/ 
                              
                             campo[posicaoB[k].i+1][posicaoB[k].j]=inimigoPassivo;

                             /*atualiza a coordenada i, pois andou para baixo*/
                             posicaoB[k].i=posicaoB[k].i+1;

                      }

                      break;
      
                  case 3:
                      
                      if(campo[posicaoB[k].i-1][posicaoB[k].j]=='.' && posicaoB[k].i-1>=0){ /*cima*/
                
                             campo[posicaoB[k].i-1][posicaoB[k].j]=inimigoPassivo;

                             /*atualiza a coordenada k, pois andou para cima*/
                             posicaoB[k].i=posicaoB[k].i-1;

                      }

                      break;
                          
                  default:

                      /*um dos casos eh inclusive o inimigo B nao andar*/              
                      campo[posicaoB[k].i][posicaoB[k].j]=inimigoPassivo;
                      posicaoB[k].i=posicaoB[k].i;
                      posicaoB[k].j=posicaoB[k].j;
                                  
                     break;

        }            
    }
}

/*funcao que faz o movimento do personagem C */
int moverPersonagem(char move,int*morreuX,int *pontos){

    int andou=0,aux=0;

    /*funcao para deixar a entrada em minusculo */
    move=tolower(move);
   
    /*regras de movimento para esquerda*/
    if (move == 'a'){
        
		if (campo[x][y - 1] == '.' && y-1>=0){ /*andou para um campo vazio */
    
				campo[x][y] = '.';
			    y--;
                campo[x][y] = personagem;
                return 1;
                
        }
		else if(campo[x][y - 1] == inimigoPonto){ /*andou para um inimigoPonto O */
				campo[x][y] = '.';
		        y--;
			    campo[x][y] = personagem;
                *pontos+=10;
                quantO--;
                return 2;
                
        }else if(campo[x][y - 1] == inimigoAgressivo){ /*andou para um inimigo agressivo X */
              campo[x][y]='*';
              *morreuX=1;
              
        }	
    /*regras de movimento para cima */  
    }else if(move == 'w'){
            
                if (campo[x -1][y] == '.'){ /*andou para um campo vazio */
                    campo[x][y] = '.';
                     x--;
                    campo[x][y] = personagem;
                    return 1;
                    
                }else if(campo[x-1][y] == inimigoPonto){ /*andou para um inimigoPonto O */
                    campo[x][y] = '.';
                    x--;
                    campo[x][y] = personagem;
                    *pontos+=10;
                    quantO--;
                    return 2;
                    
            }else if(campo[x-1][y] == inimigoAgressivo){ /*andou para um inimigo agressivo X */
                *morreuX=1;
                campo[x][y]='*';
                
            }
            
        /*regras de movimento para direita */ 
        }else if(move == 'd'){

                if (campo[x][y+1] == '.' && y+1<largura){  /*andou para um campo vazio */
                    campo[x][y] = '.';
                    y++;
                    campo[x][y] = personagem;
                    return 1;
                    

                }else if(campo[x][y+1] == inimigoPonto){  /*andou para um inimigoPonto O */
                    campo[x][y] = '.';
                    y++;
                    campo[x][y] = personagem;
                    *pontos+=10;
                    quantO--;
                    return 2;
                    
                }else if(campo[x][y+1]== inimigoAgressivo){ /*andou para um inimigo agressivo X*/
                    *morreuX=1;
                    campo[x][y]='*';
                    
                }
        /*regras de movimento para baixo */     
        } else if(move == 's'){
                
                if (campo[x + 1][y] == '.'){  /*andou para um campo vazio */
                
                    campo[x][y] = '.';
                    x++;
                    campo[x][y] = personagem;
                    return 1;
                    
                }else if(campo[x+1][y] == inimigoPonto){ /*andou para um inimigoPonto O */
                    campo[x][y] = '.';
                    x++;
                    campo[x][y] = personagem;
                    *pontos+=10;
                    quantO--;
                    return 2;
                    
                }else if(campo[x+1][y]==inimigoAgressivo){ /*andou para um inimigo agressivo X */
                    *morreuX=1;
                    campo[x][y]='*';
                    

                }
            }   else if(move!='a' || move!='w' || move!='s' || move!='d'){ /*caso em que se faz um movimento invalido, resultando em uma punicao (movimento aleatorio) */
                    while(andou==0 ){
                        aux=rand()%4;
                            if( aux==0 && campo[x][y-1]=='.' && y-1>=0){
                                y--;
                                campo[x][y]=personagem;
                                campo[x][y+1]='.';
                                andou=1;
                                
                            }
                            else if (aux==1 && campo[x][y+1]=='.' && y+1<largura){ 
                                y++;
                                campo[x][y]=personagem;
                                campo[x][y-1]='.';
                                andou=1;
                                
                            }
                            else if(aux==2 && campo[x+1][y]=='.'){
                                x++;
                                campo[x][y]=personagem;
                                campo[x-1][y]='.';
                                andou=1;
                                
    
                            }
                            else if( aux==3 && campo[x-1][y]=='.' ){
                                x--;
                                campo[x][y]=personagem;
                                campo[x+1][y]='.';
                                andou=1;
                                
    
                            }else{
                                aux=rand()%4;
                                
                            }
                    }
                }else{
                    return 0;
                }

    return 0;
}

/*funcao que coloca a bomba no mapa */
void bombaColocada(int *tempoExplosao,int *bomba){
   int flag=0;

    /*sorteio das coordenadas da bomba */
    r=rand()%altura;
    s=rand()%largura;

    /*sorteio do tempo em turnos da  explosao da bomba */
    *tempoExplosao= 1 + ( rand() % 5 );

    /*colocando bomba no mapa */
    while(flag!=1){
        if(campo[r][s]=='.'){

            campo[r][s]=inimigoBomba;
            flag=1;
            *bomba=1;
        
        
        }else{
            r=rand()%altura;
            s=rand()%largura;
        }
    }
}
    
/*funcao que explode a bomba no mapa*/
void bombaExplode(int *bomba,int *morreuB){
   
    int i=1;
   
    /*onde esta a bomba recebe um '#' indicando a explosao */
    campo[r][s]='#';

    /*fazendo o rastro de explosao da bomba */
    while(campo[r][s-i]=='.' && i<=tamanhoQ && s-i>=0){/*esquerda */
        campo[r][s-i]='#'; i++;

    }
    if(campo[r][s-i]==personagem){/*caso em que a bomba mata o personagem*/
        campo[r][s-i]='*';
        *morreuB=1;
    } i=1;

    while(campo[r][s+i]=='.' && i<=tamanhoQ && s+i<largura){/*direita */
        campo[r][s+i]='#'; i++;

    }
    if(campo[r][s+i]==personagem){/*caso em que a bomba mata o personagem*/
        campo[r][s+i]='*';
        *morreuB=1;
    } i=1;

    while(campo[r-i][s]=='.' && i<=tamanhoQ){/*cima */
        campo[r-i][s]='#'; i++;

    }
    if(campo[r-i][s]==personagem){/*caso em que a bomba mata o personagem*/
        campo[r-i][s]='*';
        *morreuB=1;
    } i=1;

    while(campo[r+i][s]=='.' && i<=tamanhoQ){/*baixo */
        campo[r+i][s]='#'; i++;

    }
    if(campo[r+i][s]==personagem){/*caso em que a bomba mata o personagem*/
        campo[r+i][s]='*';
        *morreuB=1;
    } i=1;

    *bomba=0; /*por fim, apos a explosao, altero o valor de 'bomba', indicando que nao ha mais bomba no mapa*/

}

/*funcao que apaga o rastro da bomba */
void limpaBomba(){
    int i,j;

    for(i=0;i<altura;i++){
        for(j=0;j<largura;j++){
            if(campo[i][j]=='#'){
                campo[i][j]='.';

            }

        }
    }
}

/*funcao que verifica o motivo do fim do jogo */
void fim(int *morreuB,int *morreuX,int *movimentos,int *pontos){
    
    /*pegou todos os pontos do mapa*/
    if(quantO==0){
        printf("%s",RED);
        printf("\nGAME OVER\n");
        printf("%s",NONE);
        printf("Voce pegou todos os pontos do mapa!\n\n");
        printf("Pontuacao final: %d\n\n",*pontos);

        printf("Aperte ENTER para voltar ao menu");
        getchar();
        getchar();
        
           
    } /*movimentos acabaram*/
    else if(*movimentos==0){
        printf("%s",RED);
        printf("\nGAME OVER\n");
        printf("%s",NONE);
        printf("Seus movimentos acabaram!\n\n");
        printf("Pontuacao final: %d\n\n",*pontos);
        
        printf("Aperte ENTER para voltar ao menu");
        getchar();
        getchar();
  
    /*inimigo bomba 'Q' matou o personagem*/
    }else if(*morreuB==1){
        printf("%s",RED);
        printf("\nGAME OVER\n");
        printf("%s",NONE);
        printf("Uma bomba te matou!\n\n");
        printf("Pontuacao final: %d\n\n",*pontos);
        
        
        printf("Aperte ENTER para voltar ao menu");
        getchar();
        getchar();
           
    /*inimigo agressivo 'X' matou o personagem*/
    }else if(*morreuX==1){
        printf("%s",RED);
        printf("\nGAME OVER\n");
        printf("%s",NONE);
        printf("Um inimigo te matou!\n\n");
        printf("Pontuacao final: %d\n\n",*pontos);
        
        printf("Aperte ENTER para voltar ao menu");
        getchar();
        getchar();  
        
    }
}

/*funcao que mostra as instrucoes do jogo */
void instrucoes(){

        CLEAR;
        printf("\t%sJogo SnakeBomber APC\n\n",CYAN);
        printf("%sO jogador controla o personagem 'C' no tabuleiro\n\n",NONE);

        printf("%sControles do jogo:\n\n",GREEN);

        printf("%sw - cima\n\n",NONE);
        printf("a - esquerda\n\n");
        printf("s - baixo\n\n");
        printf("d - direita\n\n");
        printf("Demais teclas - Movimento aleatorio\n\n");

        printf("%sInimigos:\n\n",RED);
        printf("%s'B' - inimigo passivo, se move deixando um rastro de si mesmo,impedindo a passagem do personagem, sempre se movendo de forma aleatoria!\n\n",NONE);
        printf("'O' - inimigo passivo, fica fixo no mapa. O jogador ganha 10 pontos se pega-lo!\n\n");
        printf("'X' - inimigo agressivo, uma hora se move tentando te matar, outra hora se move de forma aleatoria!\n\n");
        
        printf("%sAperte ENTER para voltar ao menu",CYAN);
        printf("%s",NONE);
        getchar();
        getchar();
        
}

/*funcao que gerencia as configuracoes*/
void configuracoes(){
    
    int opcao;

    FILE *fp_config;
    
    int a,l,qtdB,qtdX,qtdO,tamB,tamQ;
    int alt,larg;
    int qB,qX,qO,tB,tQ,nivel,dificuldade;
    
    CLEAR;

    printf("%sSnakeBomber APC\n\n",CYAN);

    printf("%s1.Tabuleiro\n",NONE);
    printf("2.NPCs\n");
    printf("3.Dificuldade do Jogo\n");
    printf("4.Ativar Modo Rankeado\n");
    printf("5.Voltar\n");
    
    /*leitura da opcao do menu*/
    do{
        printf("\nEscolha uma opcao:");
        scanf("%d",&opcao);

        if(opcao < 1 || opcao > 5){
            
            printf("Opcao Invalida! Informe uma opcao de 1 a 5!\n");
    }

    }while(opcao < 1 || opcao > 5);

    getchar();

    
    switch(opcao){

        /*opcao 1 muda as dimensoes do tabuleiro*/
        case 1:

            printf("Informe uma altura para o tabuleiro < min 20 max 50 >\n");
                do{
           
                    scanf("%d",&a);

                    if(a<20 || a>50){
                        printf("Informe uma altura valida!\n");
                }


        }while(a<20 || a>50);

        
            printf("Informe uma largura para o tabuleiro < min 20 max 50 >\n");
                do{
                    
                    scanf("%d",&l);

                        if(l<20 || l>50){
                            printf("Informe uma largura valida!\n");
                }


        }while(l<20 || l>50);

        /*grava as novas configuracoes*/
        /*abre o arquivo de configuracoes, para leitura e gravacao*/
        fp_config=fopen("config.txt","r+");

        fscanf(fp_config,"%d %d %d %d %d %d %d %d",&alt,&larg,&qtdB,&qtdX,&qtdO,&tamB,&tamQ,&nivel);
       
        rewind(fp_config);

        fprintf(fp_config,"                        ");

        rewind(fp_config);

        fprintf(fp_config,"%d %d %d %d %d %d %d %d",a,l,qtdB,qtdX,qtdO,tamB,tamQ,nivel);

        fclose(fp_config);

        printf("%sConfiguracoes salvas! Aperte ENTER para voltar ao menu",CYAN);
        printf("%s",NONE);
        getchar();
        getchar();
        

        break;

        /*opcao 2 muda os monstros do jogo*/
        case 2: 
            

            printf("Quantidade de inimigos B no mapa: < min 1 max 10 >\n");
            
            do{
                scanf("%d",&qB);
                    if(qB<1 || qB >10){
                        printf("Informe uma quantidade valida!\n");
                    }
            }while(qB<1 || qB>10);

            printf("Quantidade de inimigos X no mapa: < min 1 max 10 >\n");

            do{
                scanf("%d",&qX);
                    if(qX<1 || qX >10){
                        printf("Informe uma quantidade valida!\n");
                    }
            }while(qX<1 || qX>10);

            printf("Quantidade de inimigos O no mapa: <  min 1 max 10 >\n");

            do{
                scanf("%d",&qO);
                    if(qO<1 || qO >10){
                        printf("Informe uma quantidade valida!\n");
                    }
            }while(qO<1 || qO>10);
            

            printf("Rastro da bomba Q no mapa: < min 5 max 10 >\n");

            do{
                scanf("%d",&tQ);
                    if(tQ<5 || tQ >10){
                        printf("Informe um tamanho valido!\n");
                    }
            }while(tQ<5 || tQ>10);


            printf("Rastro do inimigo B no mapa: < min 3 max 10 >\n");

            do{
                scanf("%d",&tB);
                    if(tB<3 || tB >10){
                        printf("Informe um tamanho valido!\n");
                    }
            }while(tB<1 || tB>10);


            /*abre o arquivo de configuracoes, para leitura e gravacao*/
            fp_config=fopen("config.txt","r+");
            
                fscanf(fp_config,"%d %d %d %d %d %d %d %d",&alt,&larg,&qtdB,&qtdX,&qtdO,&tamB,&tamQ,&nivel);
                    
                rewind(fp_config);

                fprintf(fp_config,"                        ");
            
                rewind(fp_config);

                fprintf(fp_config,"%d %d %d %d %d %d %d %d",alt,larg,qB,qX,qO,tB,tQ,nivel);
            
                fclose(fp_config);

                printf("%sConfiguracoes salvas! Aperte ENTER para voltar ao menu",CYAN);
                printf("%s",NONE);
                getchar();
                getchar();
                
        
        break;
        /* opcao 3 muda a dificuldade do jogo*/
        case 3:

            printf("1 - Facil 2 - Medio 3 - Dificil\n");

            do{
                scanf("%d",&dificuldade);
                if(dificuldade < 1 || dificuldade >3){
                    printf("Informe uma dificuldade valida!\n");

                }

            }while(dificuldade < 1 ||  dificuldade > 3);

            /*abre o arquivo de configuracoes, para leitura e gravacao*/
            fp_config=fopen("config.txt","r+");
            
                fscanf(fp_config,"%d %d %d %d %d %d %d %d",&alt,&larg,&qtdB,&qtdX,&qtdO,&tamB,&tamQ,&nivel);
                    
                rewind(fp_config);

                fprintf(fp_config,"                        ");
            
                rewind(fp_config);

                fprintf(fp_config,"%d %d %d %d %d %d %d %d",alt,larg,qtdB,qtdX,qtdO,tamB,tamQ,dificuldade);
            
                fclose(fp_config);

            printf("%sConfiguracao salva! Aperte ENTER para voltar ao menu",CYAN);
            printf("%s",NONE);
            getchar();
            getchar();

            break;

        /* opcao 4 ativa o modo rankeado do jogo*/
        case 4:

        printf("Informe um nickname entre 1 e 7 caracteres\n");
        scanf("%[^\n]s",aux1.nome);

        getchar();

        while(strlen(aux1.nome) > 7 || strlen(aux1.nome)==0){

            printf("Informe um nickname valido!\n");
            scanf("%[^\n]s",aux1.nome);
            getchar();

        }

            printf("\n%sModo rankeado ativado!\n\n",RED);
            printf("%s",NONE);

            printf("As seguintes configuracoes de jogo a partir de agora sao essas:\n\n");
            printf("*Tabuleiro 30x30\n");
            printf("*Quantidade de inimigos passivos: 3\n");
            printf("*Quantidade de inimigos agressivos: 7\n");
            printf("*Quantidade de inimigos pontos: 10\n");
            printf("*Tamanho do rastro do inimigo passivo: 7\n");
            printf("*Tamanho do rastro do inimigo bomba:10\n");
            printf("*Nivel de dificuldade do jogo: DIFICIL\n\n");

            /*muda a glogal, para fazer o teste do nivel na funcao partida()*/
            nivelDeJogo=nivel;

            /*muda a global rankeado*/
            rankeado=1;

            /*abre o arquivo de configuracoes, para leitura e gravacao*/
            fp_config=fopen("config.txt","r+");
             
                 fprintf(fp_config,"                        ");
             
                 rewind(fp_config);
 
                 /*alt,larg,qB,qX,qO,tB,tQ,nivel no modo rankeado*/
                 fprintf(fp_config,"%d %d %d %d %d %d %d %d",30,30,3,7,10,7,10,3);
             
                 fclose(fp_config);
 
            printf("%sAperte ENTER para voltar ao menu",CYAN);
            printf("%s",NONE);
            getchar();
             
        break;


        /*opcao 5 volta o menu*/
        case 5: break;

        
    }

}

/*funcao que verifica se os arquivos de configuracoes e ranking existem. caso nao existam, cria um novo arquivo*/
void verificaArquivo(){

    FILE *fp_config;
    FILE *fp_ranking;

    char nome_arquivo[]="config.txt";
    char nome_arquivo1[]="ranking.bin";

    int i,a=20,l=20, qtdB=3, qtdX=3, qtdO=3, tamB=4, tamQ=5;

    /*processo para gravar as configuracoes no arquivo */  
    fp_config=fopen(nome_arquivo,"r+");

    /*caso o arquivo nao exista, cria um novo*/
    if (fp_config == NULL) {
        fp_config = fopen(nome_arquivo, "w");

        /*grava um arquivo com as configuracoes padroes de jogo*/
        /*altura, largura, quantB, quantX, qtdO, tamanhoB, tamanhoQ,nivel de jogo*/
        fprintf(fp_config,"%d %d %d %d %d %d %d %d",a,l,qtdB,qtdX,qtdO,tamB,tamQ,nivelDeJogo);
    
        fclose(fp_config);

    }else{ /*caso contrario, apenas fecha o arquivo existente*/

        fclose(fp_config);

    }

    /*processo para verificar a existencia do ranking*/
    /*caso o arquivo nao exista, cria um novo*/
    fp_ranking = fopen(nome_arquivo1, "r+b");
    
	if (fp_ranking == NULL){

		fp_ranking= fopen(nome_arquivo1, "wb");

        /*inicia o vetor jogador com -1, representando as posicoes vazias*/
        for(i=0;i<10;i++){
            
            strcpy(jogador[i].nome,"Vazio");
            jogador[i].pontos=-1;
            jogador[i].movimentos=-1;
        
        }

        /*grava no arquivo um ranking vazio*/
        for(i = 0; i < 10; i++){
        
            fwrite(&jogador[i], sizeof(jogador[i]), 1, fp_ranking);
        
        }
        /*fecha o arquivo*/
        fclose(fp_ranking);

	}
    
    else{ /*caso contrario, apenas fecha o arquivo existente*/
    
        fclose(fp_ranking);
    }

}

/*funcao principal */
int main(){
  
    int resp=0;

    /*semente para os numeros pseudo aleatorios */
    srand(time(NULL));

    /*cria um arquivo para as configuracoes do jogo e ranking, caso nao existam*/
    verificaArquivo();

    do{

    
    /*chama o menu*/
    resp=menuMain();

    switch(resp){

        case 1:  partida(); break;
        case 2:  configuracoes();break;
        case 3:  ranking(); break;
        case 4:  instrucoes(); break;
        case 5:  exit(0); break;
        
        default: printf("Informe uma opcao valida!\n");break;
            
    }

    }while(resp > 0 || resp <= 5);
    
        
    return 0;
}
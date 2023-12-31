#include "../Headers/criptografar.h"

void Menu(){
    FILE *inputFile;
    FILE *outputFile;
    int chave,resposta;
    char entrada[30],saida[30];

    printf("Digite o arquivo de entrada: \n");
    scanf("%s", entrada);

    getchar();

    printf("Digite o arquivo de saida: \n");
    scanf("%s", saida);

    getchar();

    inputFile = fopen(entrada, "r");
    outputFile = fopen(saida, "w");
    
    if(inputFile==NULL){
        printf("O arquivo de entrada nao foi encontrada.\n");
        exit(1);
    }else if(outputFile==NULL){
        printf("O arquivo de saida nao foi encontrada.\n");
        exit(1);
    }

    printf("[1] Digitar uma propria chave\n");
    printf("[2] Usar chave aleatoria\n");
    scanf("%d",&resposta);

    if(resposta==1){
        printf("Entre com uma chave: \n");
        scanf("%d", &chave);
        printf("O que deseja fazer?\n");
        printf("[1] Criptografar\n");
        printf("[2] Descriptografar\n");
        scanf("%d",&resposta);
        if(resposta==1){
            Criptografar(inputFile, outputFile, chave);

        }else if(resposta==2){
            Descriptografar(inputFile, outputFile, chave);
        }else{
            printf("Opção inválida!\n");
            printf("Encerrando execução....");
            exit(1);
        }
    }else if(resposta==2){
        chave = rand() % 26 + 1;
        printf("O que deseja fazer?\n");
        printf("[1] Criptografar\n");
        printf("[2] Descriptografar\n");
        scanf("%d",&resposta);
        if(resposta==1){
            Criptografar(inputFile, outputFile, chave);
        }else if(resposta==2){
            Descriptografar(inputFile, outputFile, chave);
        }else {
            printf("Opção inválida!\n");
            printf("Encerrando execução....");
            exit(1);
        }
        ReiniciarLeitura(inputFile,outputFile);
        fclose(outputFile);
        outputFile = fopen(saida, "r");
        if(resposta==1){
            ExibirFrequencias(outputFile,1);
        }else{
            ExibirFrequencias(outputFile,2);
        }

        printf("Chave aleatoria gerada: %d\n", chave);
    }else{
        printf("Opção inválida!\n");
        printf("Encerrando execução....");
        exit(1);
    }
    fclose(inputFile);
    fclose(outputFile);

}

void Criptografar(FILE *ArquivoEntrada, FILE *ArquivoSaida, int chave) {
    char caractere;
    while ((caractere = fgetc(ArquivoEntrada)) != EOF) {
        char caractereCriptografado;
        
        if (caractere >= 'A' && caractere <= 'Z') {
            caractereCriptografado = (caractere + chave - 'A' + 26) % 26 + 'A';
        } else if (caractere >= 'a' && caractere <= 'z') {
            caractereCriptografado = (caractere + chave - 'a' + 26) % 26 + 'a';
        } else {
            caractereCriptografado = caractere;
        }
        
        fputc(caractereCriptografado, ArquivoSaida);
        putchar(caractereCriptografado);
    }
    
    ReiniciarLeitura(ArquivoEntrada, ArquivoSaida);
}

void Descriptografar(FILE *ArquivoEntrada, FILE *ArquivoSaida, int chave) {
    char caractere;
    while ((caractere = fgetc(ArquivoEntrada)) != EOF) {
        char caractereDescriptografado;
        
        if (caractere >= 'A' && caractere <= 'Z') {
            caractereDescriptografado = (caractere - chave - 'A' + 26) % 26 + 'A';
        } else if (caractere >= 'a' && caractere <= 'z') {
            caractereDescriptografado = (caractere - chave - 'a' + 26) % 26 + 'a';
        } else {
            caractereDescriptografado = caractere;
        }
        
        fputc(caractereDescriptografado, ArquivoSaida);
        putchar(caractereDescriptografado);
    }
    
    ReiniciarLeitura(ArquivoEntrada, ArquivoSaida);
}



void ExibirFrequencias(FILE *ArquivoSaida,int flag) {
    int *frequencias = (int*)malloc(26 * sizeof(int)),totalCaracteres = 0;
    char caractere;
    double VetorFrequencias[26];

    for(int i = 0;i<26;i++){

        frequencias[i] = 0;
        VetorFrequencias[i] = 0.0;
    
    }

    while ((caractere = fgetc(ArquivoSaida)) != EOF) {
        if ((caractere >= 'A' && caractere <= 'Z') || (caractere >= 'a' && caractere <= 'z')) {
            totalCaracteres+=1;
            if (caractere >= 'A' && caractere <= 'Z') {
                frequencias[caractere - 'A']+=1;
            } else if(caractere >= 'a' && caractere <= 'z'){
                frequencias[caractere - 'a']+=1;
            }
        }
    }

    printf("\nTabela de Frequencias:\n");
    printf("Caractere  |  Frequencia  |  Percentual\n");
    printf("-------------------------------------\n");

    for (int i = 0; i < 26; i++) {
        char caractereAtual = 'A' + i;
        int frequenciaAtual = frequencias[i];
        double percentual = (frequenciaAtual / (double)totalCaracteres) * 100;

        printf("    %c      |      %d      |     %.2lf%%\n", caractereAtual, frequenciaAtual, percentual);
        VetorFrequencias[i] =(double)(percentual / 100.0); 
    }
    EncontrarChaveAleatoria(VetorFrequencias,flag); 
}

void ReiniciarLeitura(FILE *ArquivoEntrada, FILE *ArquivoSaida){
    fseek(ArquivoSaida, 0, SEEK_SET);
    fseek(ArquivoEntrada, 0, SEEK_SET);
}

int compararFrequencias(const void *a, const void *b) {
    double frequenciaA = ((LetraFrequencia *)a)->frequencia;
    double frequenciaB = ((LetraFrequencia *)b)->frequencia;

    if (frequenciaA < frequenciaB) return -1;
    if (frequenciaA > frequenciaB) return 1;
    return 0;
}


void EncontrarChaveAleatoria(double VetorFrequencias[26],int flag){
    int MaiorChave = 0;
    double VetorPesos[] = {
        0.1463, 0.0104, 0.0388, 0.0499, 0.1257, 0.0102, 0.0130, 0.0128, 0.0618, 0.0040,
        0.0002, 0.0278, 0.0474, 0.0505, 0.1073, 0.0252, 0.0120, 0.0653, 0.0781, 0.0434,
        0.0463, 0.0167, 0.0001, 0.0021, 0.0001, 0.0047
    };
    LetraFrequencia *AUX01 = malloc(26 * sizeof(LetraFrequencia));
    LetraFrequencia *AUX02 = malloc(26 * sizeof(LetraFrequencia));
    for(int i = 0; i < 26; i++) {
        AUX01[i].frequencia = VetorPesos[i];
        AUX02[i].frequencia = VetorFrequencias[i];
        AUX01[i].caractere = 'a' + i;
        AUX02[i].caractere = 'a' + i;
    }

    qsort(AUX01, 26, sizeof(LetraFrequencia), compararFrequencias);
    qsort(AUX02, 26, sizeof(LetraFrequencia), compararFrequencias);

    if(flag == 1){
            MaiorChave = AUX02[25].caractere - AUX01[25].caractere;
    }else if(flag == 2){
            while(AUX02[25].caractere!=AUX01[25].caractere){
                MaiorChave +=1;
                AUX02[25].caractere+=1;
                if(AUX02[25].caractere>'z'){
                    AUX02[25].caractere = 'a';
                }
            }
    }else{
            MaiorChave = 26;
    }
    printf("Chave Chute: %d\n", MaiorChave);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50

typedef struct {
    int id;
    char arquivo[100];
    int paginas;
    char tipo;
} Trabalho;

Trabalho filaNormal[MAX];
int inicioNormal = 0;
int fimNormal = -1;
int qtdNormal = 0;

Trabalho filaPrioritaria[MAX];
int inicioPrioritaria = 0;
int fimPrioritaria = -1;
int qtdPrioritaria = 0;

int proximoId = 1;

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pausar() {
    printf("\nENTER para continuar");
    limparBuffer();
}

int buscarNormal(int id) {
    for (int i = 0; i < qtdNormal; i++) {
        int pos = (inicioNormal + i) % MAX;
        if (filaNormal[pos].id == id)
            return pos;
    }
    return -1;
}

int buscarPrioritaria(int id) {
    for (int i = 0; i < qtdPrioritaria; i++) {
        int pos = (inicioPrioritaria + i) % MAX;
        if (filaPrioritaria[pos].id == id)
            return pos;
    }
    return -1;
}

void adicionarNormal(Trabalho t) {
    if (qtdNormal >= MAX) {
        printf("\nFila normal cheia!\n");
        return;
    }
    fimNormal = (fimNormal + 1) % MAX;
    filaNormal[fimNormal] = t;
    qtdNormal++;
}

void adicionarPrioritaria(Trabalho t) {
    if (qtdPrioritaria >= MAX) {
        printf("\nFila prioritaria cheia!\n");
        return;
    }
    fimPrioritaria = (fimPrioritaria + 1) % MAX;
    filaPrioritaria[fimPrioritaria] = t;
    qtdPrioritaria++;
}

Trabalho removerNormal() {
    Trabalho t = filaNormal[inicioNormal];
    inicioNormal = (inicioNormal + 1) % MAX;
    qtdNormal--;
    return t;
}

Trabalho removerPrioritaria() {
    Trabalho t = filaPrioritaria[inicioPrioritaria];
    inicioPrioritaria = (inicioPrioritaria + 1) % MAX;
    qtdPrioritaria--;
    return t;
}

int filaNormalVazia() {
    return qtdNormal == 0;
}

int filaPrioritariaVazia() {
    return qtdPrioritaria == 0;
}

void cadastrarTrabalho() {
    Trabalho novo;
    novo.id = proximoId++;

    printf("\n--- Novo Trabalho ---\n");
    printf("ID: %d\n", novo.id);

    printf("Arquivo: ");
    fgets(novo.arquivo, 100, stdin);
    novo.arquivo[strcspn(novo.arquivo, "\n")] = '\0';

    printf("Paginas: ");
    scanf("%d", &novo.paginas);
    limparBuffer();

    printf("Tipo (N ou P): ");
    scanf(" %c", &novo.tipo);
    limparBuffer();

    if (novo.tipo == 'P' || novo.tipo == 'p') {
        novo.tipo = 'P';
        adicionarPrioritaria(novo);
        printf("\n'%s' entrou na fila PRIORITARIA.\n", novo.arquivo);
    } else {
        novo.tipo = 'N';
        adicionarNormal(novo);
        printf("\n'%s' entrou na fila NORMAL.\n", novo.arquivo);
    }
    pausar();
}

void processarProximo() {
    if (!filaPrioritariaVazia()) {
        Trabalho t = removerPrioritaria();
        printf("\n--- Imprimindo (PRIORITARIO) ---\n");
        printf("ID: %d\n", t.id);
        printf("Arquivo: %s\n", t.arquivo);
        printf("Paginas: %d\n", t.paginas);
        printf("Pronto!\n");
    }
    else if (!filaNormalVazia()) {
        Trabalho t = removerNormal();
        printf("\n--- Imprimindo (NORMAL) ---\n");
        printf("ID: %d\n", t.id);
        printf("Arquivo: %s\n", t.arquivo);
        printf("Paginas: %d\n", t.paginas);
        printf("Pronto!\n");
    }
    else {
        printf("\nNao tem nada na fila.\n");
    }
    pausar();
}

void listarFilas() {
    printf("\n--- Filas ---\n");

    if (filaPrioritariaVazia()) {
        printf("\nPrioritaria: vazia\n");
    } else {
        printf("\nPrioritaria (%d):\n", qtdPrioritaria);
        for (int i = 0; i < qtdPrioritaria; i++) {
            int pos = (inicioPrioritaria + i) % MAX;
            printf("  [%d] %s - %d pags\n",
                   filaPrioritaria[pos].id,
                   filaPrioritaria[pos].arquivo,
                   filaPrioritaria[pos].paginas);
        }
    }

    if (filaNormalVazia()) {
        printf("\nNormal: vazia\n");
    } else {
        printf("\nNormal (%d):\n", qtdNormal);
        for (int i = 0; i < qtdNormal; i++) {
            int pos = (inicioNormal + i) % MAX;
            printf("  [%d] %s - %d pags\n",
                   filaNormal[pos].id,
                   filaNormal[pos].arquivo,
                   filaNormal[pos].paginas);
        }
    }
    pausar();
}

void buscarTrabalho() {
    if (filaNormalVazia() && filaPrioritariaVazia()) {
        printf("\nAs filas estao vazias.\n");
        pausar();
        return;
    }

    int id;
    printf("\n--- Buscar ---\n");
    printf("ID: ");
    scanf("%d", &id);
    limparBuffer();

    int pos = buscarPrioritaria(id);
    if (pos != -1) {
        printf("\nAchei na fila PRIORITARIA:\n");
        printf("  ID: %d\n", filaPrioritaria[pos].id);
        printf("  Arquivo: %s\n", filaPrioritaria[pos].arquivo);
        printf("  Paginas: %d\n", filaPrioritaria[pos].paginas);
        pausar();
        return;
    }

    pos = buscarNormal(id);
    if (pos != -1) {
        printf("\nAchei na fila NORMAL:\n");
        printf("  ID: %d\n", filaNormal[pos].id);
        printf("  Arquivo: %s\n", filaNormal[pos].arquivo);
        printf("  Paginas: %d\n", filaNormal[pos].paginas);
        pausar();
        return;
    }

    printf("\nNao achei o ID %d.\n", id);
    pausar();
}

void cancelarTrabalho() {
    if (filaNormalVazia() && filaPrioritariaVazia()) {
        printf("\nAs filas estao vazias.\n");
        pausar();
        return;
    }

    int id;
    printf("\n--- Cancelar ---\n");
    printf("ID do trabalho: ");
    scanf("%d", &id);
    limparBuffer();

    int posP = buscarPrioritaria(id);
    if (posP != -1) {
        printf("\nCancelando '%s'...\n", filaPrioritaria[posP].arquivo);
        
        int i = posP;
        while (i != fimPrioritaria) {
            int proximo = (i + 1) % MAX;
            filaPrioritaria[i] = filaPrioritaria[proximo];
            i = proximo;
        }
        fimPrioritaria = (fimPrioritaria - 1 + MAX) % MAX;
        qtdPrioritaria--;
        
        printf("Cancelado!\n");
        pausar();
        return;
    }

    int posN = buscarNormal(id);
    if (posN != -1) {
        printf("\nCancelando '%s'...\n", filaNormal[posN].arquivo);
        
        int i = posN;
        while (i != fimNormal) {
            int proximo = (i + 1) % MAX;
            filaNormal[i] = filaNormal[proximo];
            i = proximo;
        }
        fimNormal = (fimNormal - 1 + MAX) % MAX;
        qtdNormal--;
        
        printf("Cancelado!\n");
        pausar();
        return;
    }

    printf("\nNao achei o ID %d.\n", id);
    pausar();
}

void salvarDados() {
    FILE *arq = fopen("impressao.csv", "w");

    if (arq == NULL) {
        printf("\nErro ao salvar!\n");
        pausar();
        return;
    }

    fprintf(arq, "Tipo;ID;Arquivo;Paginas\n");

    for (int i = 0; i < qtdPrioritaria; i++) {
        int pos = (inicioPrioritaria + i) % MAX;
        fprintf(arq, "P;%d;%s;%d\n",
                filaPrioritaria[pos].id,
                filaPrioritaria[pos].arquivo,
                filaPrioritaria[pos].paginas);
    }

    for (int i = 0; i < qtdNormal; i++) {
        int pos = (inicioNormal + i) % MAX;
        fprintf(arq, "N;%d;%s;%d\n",
                filaNormal[pos].id,
                filaNormal[pos].arquivo,
                filaNormal[pos].paginas);
    }

    fclose(arq);
    printf("\nSalvo! (P: %d, N: %d)\n", qtdPrioritaria, qtdNormal);
}

void carregarDados() {
    FILE *arq = fopen("impressao.csv", "r");

    if (arq == NULL) {
        printf("Nenhum arquivo salvo. Comecando vazio.\n");
        return;
    }

    qtdNormal = 0;
    qtdPrioritaria = 0;
    inicioNormal = 0;
    fimNormal = -1;
    inicioPrioritaria = 0;
    fimPrioritaria = -1;
    
    int maiorId = 0;

    char linha[200];
    char tipo;

    fgets(linha, sizeof(linha), arq);

    while (fgets(linha, sizeof(linha), arq) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';

        Trabalho t;
        sscanf(linha, "%c;%d;%[^;];%d", &tipo, &t.id, t.arquivo, &t.paginas);
        
        if (t.id > maiorId) {
            maiorId = t.id;
        }

        if (tipo == 'P') {
            adicionarPrioritaria(t);
        } else {
            adicionarNormal(t);
        }
    }

    fclose(arq);
    proximoId = maiorId + 1;
    printf("Arquivo carregado! (%d trabalhos)\n", qtdNormal + qtdPrioritaria);
}

int main() {
    carregarDados();
    pausar();

    int op;

    do {
        printf("\n");
        printf("+----------------------------+\n");
        printf("|     FILA DE IMPRESSAO      |\n");
        printf("+----------------------------+\n");
        printf("| Prioritaria: %d             |\n", qtdPrioritaria);
        printf("| Normal: %d                  |\n", qtdNormal);
        printf("+----------------------------+\n");
        printf("| 1 - Novo trabalho          |\n");
        printf("| 2 - Imprimir proximo       |\n");
        printf("| 3 - Ver filas              |\n");
        printf("| 4 - Buscar trabalho        |\n");
        printf("| 5 - Cancelar trabalho      |\n");
        printf("| 6 - Salvar                 |\n");
        printf("| 0 - Sair                   |\n");
        printf("+----------------------------+\n");
        printf("Opcao: ");
        scanf("%d", &op);
        limparBuffer();

        switch (op) {
            case 1: cadastrarTrabalho(); break;
            case 2: processarProximo(); break;
            case 3: listarFilas(); break;
            case 4: buscarTrabalho(); break;
            case 5: cancelarTrabalho(); break;
            case 6:
                salvarDados();
                pausar();
                break;
            case 0:
                printf("\nSalvando...\n");
                salvarDados();
                printf("Ate mais!\n");
                break;
            default:
                printf("\nOpcao invalida. Tenta de novo.\n");
                pausar();
        }

    } while (op != 0);

    return 0;
}
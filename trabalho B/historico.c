#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100

typedef struct {
    char url[100];
    char titulo[100];
    char dataHora[50];
} Pagina;

Pagina historico[MAX];
Pagina avancar[MAX];

int topoHistorico = -1;
int topoAvancar = -1;

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pausar() {
    printf("ENTER para continuar");
    limparBuffer();
}

void pegarDataHora(char *buffer) {
    time_t agora = time(NULL);
    struct tm *t = localtime(&agora);
    sprintf(buffer, "%02d/%02d/%04d %02d:%02d",
            t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
            t->tm_hour, t->tm_min);
}

void pushHistorico(Pagina p) {
    if (topoHistorico >= MAX - 1) {
        printf("\nHistorico cheio! Tirei a pagina mais antga.\n");
        for (int i = 0; i < topoHistorico; i++) {
            historico[i] = historico[i + 1];
        }
        topoHistorico--;
    }
    topoHistorico++;
    historico[topoHistorico] = p;
}

void pushAvancar(Pagina p) {
    if (topoAvancar >= MAX - 1) {
        printf("\npilha de avanco cheia\n");
        return;
    }
    topoAvancar++;
    avancar[topoAvancar] = p;
}

Pagina popHistorico() {
    Pagina p = historico[topoHistorico];
    topoHistorico--;
    return p;
}

Pagina popAvancar() {
    Pagina p = avancar[topoAvancar];
    topoAvancar--;
    return p;
}

Pagina verTopoHistorico() {
    return historico[topoHistorico];
}

int temPaginaAtual() {
    return topoHistorico >= 0;
}

int podeVoltar() {
    return topoHistorico > 0;
}

int podeAvancar() {
    return topoAvancar >= 0;
}

void limparAvancar() {
    topoAvancar = -1;
}

void visitarPagina() {
    Pagina nova;
    char dh[50];
    
    pegarDataHora(dh);

    printf("\n--- Nova Pagina ---\n");

    printf("URL: ");
    fgets(nova.url, 100, stdin);
    nova.url[strcspn(nova.url, "\n")] = '\0';

    printf("Titulo: ");
    fgets(nova.titulo, 100, stdin);
    nova.titulo[strcspn(nova.titulo, "\n")] = '\0';

    strcpy(nova.dataHora, dh);

    pushHistorico(nova);
    limparAvancar();

    printf("\nAbrindo: %s\n", nova.titulo);
    printf("Link: %s\n", nova.url);
    pausar();
}

void voltarPagina() {
    if (!podeVoltar()) {
        printf("\nNao tem pagina anterior.\n");
        pausar();
        return;
    }

    Pagina atual = popHistorico();
    pushAvancar(atual);

    Pagina anterior = verTopoHistorico();
    
    printf("\nVoltando...\n");
    printf("Pagina: %s\n", anterior.titulo);
    pausar();
}

void avancarPagina() {
    if (!podeAvancar()) {
        printf("\nNao tem pagina pra avancar.\n");
        pausar();
        return;
    }

    Pagina proxima = popAvancar();
    pushHistorico(proxima);

    printf("\nAvancando...\n");
    printf("Pagina: %s\n", proxima.titulo);
    pausar();
}

void paginaAtual() {
    if (!temPaginaAtual()) {
        printf("\nNenhuma pagina aberta ainda.\n");
        pausar();
        return;
    }

    Pagina p = verTopoHistorico();
    
    printf("\n--- Pagina Atual ---\n");
    printf("Titulo: %s\n", p.titulo);
    printf("Link: %s\n", p.url);
    printf("Acessado: %s\n", p.dataHora);
    
    if (podeAvancar()) {
        printf("(%d pagina(s) pra avancar)\n", topoAvancar + 1);
    }
    if (podeVoltar()) {
        printf("(%d pagina(s) pra voltar)\n", topoHistorico);
    }
    
    pausar();
}

void verHistorico() {
    if (!temPaginaAtual()) {
        printf("\nNenhuma pagina visitada ainda.\n");
        pausar();
        return;
    }

    printf("\n--- Historico ---\n\n");

    for (int i = topoHistorico; i >= 0; i--) {
        printf("%d. %s\n", topoHistorico - i + 1, historico[i].titulo);
        printf("   %s\n", historico[i].url);
        printf("   %s\n\n", historico[i].dataHora);
    }
    pausar();
}

void salvarDados() {
    FILE *arq = fopen("historico.csv", "w");

    if (arq == NULL) {
        printf("\nErro ao salvar!\n");
        pausar();
        return;
    }

    fprintf(arq, "Tipo;URL;Titulo;DataHora\n");

    for (int i = 0; i <= topoHistorico; i++) {
        fprintf(arq, "H;%s;%s;%s\n",
                historico[i].url,
                historico[i].titulo,
                historico[i].dataHora);
    }

    for (int i = 0; i <= topoAvancar; i++) {
        fprintf(arq, "A;%s;%s;%s\n",
                avancar[i].url,
                avancar[i].titulo,
                avancar[i].dataHora);
    }

    fclose(arq);
    printf("\nSalvo! (%d paginas no historico)\n", topoHistorico + 1);
}

void carregarDados() {
    FILE *arq = fopen("historico.csv", "r");

    if (arq == NULL) {
        printf("Nenhum arquivo salvoComecando vazio.\n");
        return;
    }

    topoHistorico = -1;
    topoAvancar = -1;

    char linha[300];
    char tipo;

    fgets(linha, sizeof(linha), arq);

    while (fgets(linha, sizeof(linha), arq) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';

        Pagina p;
        sscanf(linha, "%c;%[^;];%[^;];%[^\n]",
               &tipo, p.url, p.titulo, p.dataHora);

        if (tipo == 'H') {
            topoHistorico++;
            historico[topoHistorico] = p;
        } else if (tipo == 'A') {
            topoAvancar++;
            avancar[topoAvancar] = p;
        }
    }

    fclose(arq);
    printf("Arquivo carregado! (%d paginas)\n", topoHistorico + 1);
}

int main() {
    carregarDados();
    pausar();

    int op;

    do {
        printf("\n");
        printf("+---------------------------+\n");
        printf("|       NAVEGADOR WEB       |\n");
        printf("+---------------------------+\n");
        
        if (temPaginaAtual()) {
            Pagina p = verTopoHistorico();
            printf("| Pagina: %.18s\n", p.titulo);
        } else {
            printf("| Nenhuma pagina aberta\n");
        }
        
        printf("+---------------------------+\n");
        printf("| 1 - Abrir site            |\n");
        printf("| 2 - Voltar                |\n");
        printf("| 3 - Avancar               |\n");
        printf("| 4 - Pagina atual          |\n");
        printf("| 5 - Ver historico         |\n");
        printf("| 6 - Salvar                |\n");
        printf("| 0 - Sair                  |\n");
        printf("+---------------------------+\n");
        printf("Opcao: ");
        scanf("%d", &op);
        limparBuffer();

        switch (op) {
            case 1: visitarPagina(); break;
            case 2: voltarPagina(); break;
            case 3: avancarPagina(); break;
            case 4: paginaAtual(); break;
            case 5: verHistorico(); break;
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
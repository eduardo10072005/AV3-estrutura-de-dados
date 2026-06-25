#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct {
    int id;
    char titulo[50];
    char artista[50];
    float duracao;
    char genero[30];
} Musica;

Musica playlist[MAX];
int qtd = 0;
int proximoId = 1;

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pausar() {
    printf("ENTER para continuar");
    limparBuffer();
}

int buscarMusica(int id) {
    for (int i = 0; i < qtd; i++) {
        if (playlist[i].id == id)
            return i;
    }
    return -1;
}

void cadastrarMusica() {
    if (qtd >= MAX) {
        printf("\nPlaylist cheia\n");
        pausar();
        return;
    }

    Musica m;
    m.id = proximoId++;

    printf("\n--- Nova Musica ---\n");
    printf("ID: %d\n", m.id);

    printf("Titulo: ");
    fgets(m.titulo, 50, stdin);
    m.titulo[strcspn(m.titulo, "\n")] = '\0';

    printf("Artista: ");
    fgets(m.artista, 50, stdin);
    m.artista[strcspn(m.artista, "\n")] = '\0';

    printf("Duracao (min): ");
    scanf("%f", &m.duracao);
    limparBuffer();

    printf("Genero: ");
    fgets(m.genero, 30, stdin);
    m.genero[strcspn(m.genero, "\n")] = '\0';

    playlist[qtd++] = m;

    printf("\n'%s' adicionada !\n", m.titulo);
    pausar();
}

void listarMusicas() {
    if (qtd == 0) {
        printf("\nPlaylist vazia.\n");
        pausar();
        return;
    }

    printf("\n--- Playlist (%d musicas) ---\n\n", qtd);

    for (int i = 0; i < qtd; i++) {
        printf("[%d] %s\n", playlist[i].id, playlist[i].titulo);
        printf("    Artista: %s\n", playlist[i].artista);
        printf("    Duracao: %.2f min | Genero: %s\n\n", 
               playlist[i].duracao, playlist[i].genero);
    }
    pausar();
}

void consultarMusica() {
    if (qtd == 0) {
        printf("\nPlaylist vazia.\n");
        pausar();
        return;
    }

    int id;
    printf("\n--- Buscar ---\n");
    printf("ID: ");
    scanf("%d", &id);
    limparBuffer();

    int pos = buscarMusica(id);

    if (pos == -1) {
        printf("\nID %d nao encontrado.\n", id);
        pausar();
        return;
    }

    printf("\n[%d] %s\n", playlist[pos].id, playlist[pos].titulo);
    printf("Artista: %s\n", playlist[pos].artista);
    printf("Duracao: %.2f min\n", playlist[pos].duracao);
    printf("Genero: %s\n", playlist[pos].genero);
    pausar();
}

void editarMusica() {
    if (qtd == 0) {
        printf("\nPlaylist vazia\n");
        pausar();
        return;
    }

    int id;
    printf("\n--- Editar ---\n");
    printf("ID: ");
    scanf("%d", &id);
    limparBuffer();

    int pos = buscarMusica(id);

    if (pos == -1) {
        printf("\nID %d nao encontrado.\n", id);
        pausar();
        return;
    }

    char entrada[100];

    printf("\nEditando : %s (ID %d)\n", playlist[pos].titulo, playlist[pos].id);
    printf("(Enter = mantem)\n\n");

    printf("Titulo [%s]: ", playlist[pos].titulo);
    fgets(entrada, 100, stdin);
    if (entrada[0] != '\n') {
        entrada[strcspn(entrada, "\n")] = '\0';
        strcpy(playlist[pos].titulo, entrada);
    }

    printf("Artista [%s]: ", playlist[pos].artista);
    fgets(entrada, 100, stdin);
    if (entrada[0] != '\n') {
        entrada[strcspn(entrada, "\n")] = '\0';
        strcpy(playlist[pos].artista, entrada);
    }

    printf("Duracao [%.2f]: ", playlist[pos].duracao);
    fgets(entrada, 100, stdin);
    if (entrada[0] != '\n') {
        sscanf(entrada, "%f", &playlist[pos].duracao);
    }

    printf("Genero [%s]: ", playlist[pos].genero);
    fgets(entrada, 100, stdin);
    if (entrada[0] != '\n') {
        entrada[strcspn(entrada, "\n")] = '\0';
        strcpy(playlist[pos].genero, entrada);
    }

    printf("\nAtualizado\n");
    pausar();
}

void excluirMusica() {
    if (qtd == 0) {
        printf("\nplaylist vazia.\n");
        pausar();
        return;
    }

    int id;
    printf("\n--- Remover ---\n");
    printf("ID: ");
    scanf("%d", &id);
    limparBuffer();

    int pos = buscarMusica(id);

    if (pos == -1) {
        printf("\nID %d nao encontrado.\n", id);
        pausar();
        return;
    }

    printf("\nRemover '%s'? (s/n): ", playlist[pos].titulo);
    char conf;
    scanf(" %c", &conf);
    limparBuffer();

    if (conf == 's' || conf == 'S') {
        for (int i = pos; i < qtd - 1; i++) {
            playlist[i] = playlist[i + 1];
        }
        qtd--;
        printf("Removido!\n");
    } else {
        printf("Cancelado.\n");
    }
    pausar();
}

void salvarDados() {
    FILE *arq = fopen("musicas.csv", "w");

    if (arq == NULL) {
        printf("\nErro ao salvar\n");
        pausar();
        return;
    }

    fprintf(arq, "ID;Titulo;Artista;Duracao;Genero\n");

    for (int i = 0; i < qtd; i++) {
        fprintf(arq, "%d;%s;%s;%.2f;%s\n",
                playlist[i].id,
                playlist[i].titulo,
                playlist[i].artista,
                playlist[i].duracao,
                playlist[i].genero);
    }

    fclose(arq);
    printf("\nSalvo! (%d musicas)\n", qtd);
}

void carregarDados() {
    FILE *arq = fopen("musicas.csv", "r");

    if (arq == NULL) {
        printf("Nenhum arquivo salvo. Comecando vazio.\n");
        return;
    }

    qtd = 0;
    int maiorId = 0;
    char linha[256];

    fgets(linha, sizeof(linha), arq);

    while (fgets(linha, sizeof(linha), arq) != NULL && qtd < MAX) {
        linha[strcspn(linha, "\r\n")] = '\0';

        if (sscanf(linha, "%d;%49[^;];%49[^;];%f;%29[^\n]",
                   &playlist[qtd].id,
                   playlist[qtd].titulo,
                   playlist[qtd].artista,
                   &playlist[qtd].duracao,
                   playlist[qtd].genero) == 5) {
            
            if (playlist[qtd].id > maiorId)
                maiorId = playlist[qtd].id;
            qtd++;
        }
    }

    fclose(arq);
    proximoId = maiorId + 1;
    printf("Arquivo carregado! (%d musicas)\n", qtd);
}

int main() {
    carregarDados();
    pausar();

    int op;

    do {
        printf("\n");
        printf("+---------------------------+\n");
        printf("|     PLAYLIST DE MUSICAS   |\n");
        printf("+---------------------------+\n");
        printf("| %d musica(s) na playlist   |\n", qtd);
        printf("+---------------------------+\n");
        printf("| 1 - Adicionar musica      |\n");
        printf("| 2 - Ver playlist          |\n");
        printf("| 3 - Buscar musica         |\n");
        printf("| 4 - Editar musica         |\n");
        printf("| 5 - Remover musica        |\n");
        printf("| 6 - Salvar                |\n");
        printf("| 0 - Sair                  |\n");
        printf("+---------------------------+\n");
        printf("Opcao: ");
        scanf("%d", &op);
        limparBuffer();

        switch (op) {
            case 1: cadastrarMusica(); break;
            case 2: listarMusicas(); break;
            case 3: consultarMusica(); break;
            case 4: editarMusica(); break;
            case 5: excluirMusica(); break;
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
                printf("\nOpcao invalida.Tente de novo.\n");
                pausar();
        }

    } while (op != 0);

    return 0;
}
# Relatório - AV3 estruturas de Dados

Aluno: Eduardo da Silva Costa
Matrícula: 2250204739

Trabalho A - Playlist de Músicas (Lista Simples)

Estrutura
- Vetor de struct `Musica` com 100 posições
- Controle de quantidade com `qtd`
- IDs gerados automaticamente

Operações
- Cadastrar: adiciona ao final do vetor
- Buscar: pesquisa linear por ID
- Editar: altera campos mantendo o ID
- Excluir: remove e rearranja o vetor
- Listar: exibe todas as músicas


Trabalho B - Histórico de Navegação (Pilha Simples)

Estrutura
- Duas pilhas com vetor de struct `Pagina`
- Controle de topo com `topoHistorico` e `topoAvancar`

Operações
- Visitar: empilha no histórico e limpa pilha de avanço
- Voltar: desempilha do histórico e empilha no avanço
- Avançar: desempilha do avanço e empilha no histórico


Trabalho C - Fila de Impressão (Fila Simples e Circular)

Estrutura
- Fila simples para trabalhos normais
- Fila circular para trabalhos prioritários

Operações
- Enviar: adiciona na fila correta
- Processar: prioridade para fila prioritária
- Cancelar: remove de qualquer posição e reorganiza

Conceitos Aplicados

- Listas com vetores
- Pilhas (LIFO)
- Filas (FIFO) e Fila Circular
- Persistência em arquivo CSV
- Structs e organização de dados

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define NUM_CARTAS 2
#define NUM_ATRIBUTOS 5
#define MAX_TENTATIVAS 3

typedef struct {
    char estado[2];
    char codigo[4];
    char cidade[50];
    int populacao;       // em habitantes
    float area;          // em km²
    float pib;           // em bilhões de reais
    int pontos_turisticos;
} Carta;

// Protótipos
void exibirMenuPrincipal();
void exibirMenuAtributos(int atributoExcluido);
void compararCartas(Carta carta1, Carta carta2, int atributo1, int atributo2);
float obterValorAtributo(Carta carta, int atributo);
void exibirCarta(Carta carta);
void limparBuffer();
int validarEntrada(int min, int max, int tentativas);

int main() {
    Carta cartas[NUM_CARTAS] = {
        {"A", "A01", "São Paulo", 12325000, 1521.11, 699.28, 50},
        {"B", "B02", "Rio de Janeiro", 6748000, 1200.25, 300.50, 30}
    };

    int opcao, carta1 = -1, carta2 = -1;
    int atributo1 = 0, atributo2 = 0;
    
    printf("=== SUPER TRUNFO DE CIDADES ===\n");
    printf("REGRAS:\n");
    printf("- Compare duas cidades por 2 atributos diferentes\n");
    printf("- Maior valor vence (exceto Área: menor vence)\n");
    printf("- Soma dos atributos define o vencedor final\n\n");
    
    do {
        exibirMenuPrincipal();
        opcao = validarEntrada(1, 3, MAX_TENTATIVAS);
        
        if(opcao == -1) {
            printf("Tentativas excedidas. Reinicie o jogo.\n");
            exit(1);
        }

        switch(opcao) {
            case 1:
                printf("\nCartas disponíveis:\n");
                for(int i = 0; i < NUM_CARTAS; i++) {
                    printf("%d. %s (%s)\n", i+1, cartas[i].cidade, cartas[i].codigo);
                }
                
                printf("\nEscolha a primeira carta (1-%d): ", NUM_CARTAS);
                carta1 = validarEntrada(1, NUM_CARTAS, MAX_TENTATIVAS) - 1;
                
                printf("Escolha a segunda carta (1-%d, diferente): ", NUM_CARTAS);
                do {
                    carta2 = validarEntrada(1, NUM_CARTAS, MAX_TENTATIVAS) - 1;
                    if(carta2 == carta1) printf("Escolha diferente: ");
                } while(carta2 == carta1);
                
                printf("\nCartas selecionadas:\n");
                printf("1. "); exibirCarta(cartas[carta1]);
                printf("\n2. "); exibirCarta(cartas[carta2]);
                break;
                
            case 2:
                if(carta1 == -1 || carta2 == -1) {
                    printf("Escolha duas cartas primeiro!\n");
                    break;
                }
                
                printf("\nEscolha o PRIMEIRO atributo:\n");
                exibirMenuAtributos(-1);
                atributo1 = validarEntrada(1, NUM_ATRIBUTOS, MAX_TENTATIVAS);
                
                printf("\nEscolha o SEGUNDO atributo (diferente):\n");
                exibirMenuAtributos(atributo1);
                do {
                    atributo2 = validarEntrada(1, NUM_ATRIBUTOS, MAX_TENTATIVAS);
                    if(atributo2 == atributo1) printf("Escolha diferente: ");
                } while(atributo2 == atributo1);
                
                compararCartas(cartas[carta1], cartas[carta2], atributo1, atributo2);
                break;
                
            case 3:
                printf("Jogo encerrado.\n");
                break;
        }
    } while(opcao != 3);
    
    return 0;
}

void exibirMenuPrincipal() {
    printf("\nMENU:\n");
    printf("1. Escolher cartas\n");
    printf("2. Comparar cartas\n");
    printf("3. Sair\n");
    printf("Opção: ");
}

void exibirMenuAtributos(int atributoExcluido) {
    char* atributos[NUM_ATRIBUTOS] = {
        "População (maior vence)",
        "Área em km² (menor vence)",
        "PIB em bilhões (maior vence)",
        "Pontos Turísticos (maior vence)",
        "Densidade (hab/km²) (menor vence)"
    };
    
    for(int i = 0; i < NUM_ATRIBUTOS; i++) {
        if(i+1 != atributoExcluido) {
            printf("%d. %s\n", i+1, atributos[i]);
        }
    }
    printf("Digite: ");
}

float obterValorAtributo(Carta carta, int atributo) {
    switch(atributo) {
        case 1: return carta.populacao;
        case 2: return carta.area;
        case 3: return carta.pib;
        case 4: return carta.pontos_turisticos;
        case 5: return carta.populacao / carta.area; // Densidade
        default: return 0;
    }
}

void compararCartas(Carta c1, Carta c2, int a1, int a2) {
    float v1a1 = obterValorAtributo(c1, a1);
    float v2a1 = obterValorAtributo(c2, a1);
    float v1a2 = obterValorAtributo(c1, a2);
    float v2a2 = obterValorAtributo(c2, a2);
    
    // Regras especiais (Área e Densidade: menor vence)
    int menorVenceA1 = (a1 == 2 || a1 == 5);
    int menorVenceA2 = (a2 == 2 || a2 == 5);
    
    char* vencedorA1 = (menorVenceA1 ? 
                       (v1a1 < v2a1 ? c1.cidade : c2.cidade) :
                       (v1a1 > v2a1 ? c1.cidade : c2.cidade));
    
    char* vencedorA2 = (menorVenceA2 ? 
                       (v1a2 < v2a2 ? c1.cidade : c2.cidade) :
                       (v1a2 > v2a2 ? c1.cidade : c2.cidade));
    
    float soma1 = v1a1 + v1a2;
    float soma2 = v2a1 + v2a2;
    
    printf("\n=== RESULTADO ===\n");
    printf("Atributo 1: %s\n", (a1 == 5) ? "Densidade" : 
                              (a1 == 2) ? "Área" : 
                              (a1 == 3) ? "PIB" : 
                              (a1 == 4) ? "Pontos Turísticos" : "População");
    printf("%s: %.2f\n%s: %.2f\n", c1.cidade, v1a1, c2.cidade, v2a1);
    printf("Vencedor: %s\n\n", vencedorA1);
    
    printf("Atributo 2: %s\n", (a2 == 5) ? "Densidade" : 
                              (a2 == 2) ? "Área" : 
                              (a2 == 3) ? "PIB" : 
                              (a2 == 4) ? "Pontos Turísticos" : "População");
    printf("%s: %.2f\n%s: %.2f\n", c1.cidade, v1a2, c2.cidade, v2a2);
    printf("Vencedor: %s\n\n", vencedorA2);
    
    printf("SOMA FINAL:\n");
    printf("%s: %.2f + %.2f = %.2f\n", c1.cidade, v1a1, v1a2, soma1);
    printf("%s: %.2f + %.2f = %.2f\n", c2.cidade, v2a1, v2a2, soma2);
    
    if(soma1 == soma2) {
        printf("\nEMPATE!\n");
    } else {
        printf("\nVENCEDOR FINAL: %s\n", (soma1 > soma2) ? c1.cidade : c2.cidade);
    }
}

void exibirCarta(Carta c) {
    printf("%s (%s)\n", c.cidade, c.codigo);
    printf("- Estado: %s\n- População: %d\n- Área: %.2f km²\n", c.estado, c.populacao, c.area);
    printf("- PIB: R$ %.2f bi\n- Pontos Turísticos: %d\n", c.pib, c.pontos_turisticos);
    printf("- Densidade: %.2f hab/km²\n", c.populacao / c.area);
}

void limparBuffer() {
    while(getchar() != '\n');
}

int validarEntrada(int min, int max, int tentativas) {
    int valor, tent = 0;
    while(tent < tentativas) {
        if(scanf("%d", &valor) != 1 || valor < min || valor > max) {
            printf("Inválido! Digite %d-%d: ", min, max);
            limparBuffer();
            tent++;
        } else {
            return valor;
        }
    }
    return 0;
}
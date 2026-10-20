#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include "src/AVLTree.h"
#include "src/RBTree.h"

using namespace std;

int readInt(const string& message) {
    int value;
    while (true) {
        cout << message;
        if (cin >> value) return value;
        cout << "Entrada invalida. Digite um numero inteiro.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

template <typename Tree>
void runTreeMenu(Tree& tree, const string& name) {
    int option;
    do {
        cout << "\n=== Arvore " << name << " ===\n"
             << "1. Inserir elemento\n2. Remover elemento\n3. Buscar elemento\n"
             << "4. Percurso em ordem\n5. Imprimir estrutura\n6. Mostrar estatisticas\n"
             << "0. Voltar ao menu principal\n";
        option = readInt("Escolha: ");
        int value;
        switch (option) {
            case 1:
                value = readInt("Valor para inserir: ");
                if (tree.insert(value)) cout << "Elemento inserido.\n";
                else cout << "Valor repetido: insercao ignorada.\n";
                cout << "Propriedades preservadas? " << (tree.isValid() ? "SIM" : "NAO") << '\n';
                break;
            case 2:
                value = readInt("Valor para remover: ");
                if (tree.remove(value)) cout << "Elemento removido.\n";
                else cout << "Elemento nao encontrado; nada foi removido.\n";
                cout << "Propriedades preservadas? " << (tree.isValid() ? "SIM" : "NAO") << '\n';
                break;
            case 3:
                value = readInt("Valor para buscar: ");
                cout << (tree.search(value) ? "Elemento encontrado.\n" : "Elemento nao encontrado.\n");
                break;
            case 4:
                cout << "Elementos em ordem crescente: "; tree.inorder();
                break;
            case 5:
                cout << "Estrutura (raiz no topo; lado direito acima):\n"; tree.print();
                if (name == "Rubro-Negra") cout << "Legenda: V = vermelho, P = preto.\n";
                break;
            case 6:
                cout << "Altura: " << tree.getHeight() << '\n'
                     << "Rotacoes acumuladas: " << tree.getRotations() << '\n'
                     << "Propriedades preservadas? " << (tree.isValid() ? "SIM" : "NAO") << '\n';
                break;
            case 0: break;
            default: cout << "Opcao invalida.\n";
        }
    } while (option != 0);
}

void runAutomaticTests() {
    AVLTree avl;
    RBTree rb;
    int passed = 0, total = 0;
    auto checkCondition = [&](const string& description, bool condition) {
        ++total;
        bool ok = condition && avl.isValid() && rb.isValid();
        cout << (ok ? "[OK] " : "[FALHA] ") << description << '\n';
        if (ok) ++passed;
    };
    auto check = [&](const string& description) {
        checkCondition(description, true);
    };
    auto insertBoth = [&](int value) {
        bool a = avl.insert(value), b = rb.insert(value);
        checkCondition("insercao de " + to_string(value) + " e validacao das duas arvores", a == b);
    };
    auto removeBoth = [&](int value) {
        bool a = avl.remove(value), b = rb.remove(value);
        checkCondition("remocao de " + to_string(value) + " e validacao das duas arvores", a == b);
    };

    cout << "\n--- TESTES AUTOMATICOS COMPARATIVOS ---\n";
    check("arvores inicialmente vazias");
    cout << "Cenario 0: arvore com apenas um elemento\n";
    insertBoth(7);
    checkCondition("busca do unico elemento", avl.search(7) && rb.search(7));
    removeBoth(7);
    check("arvores vazias apos remover o unico elemento");
    cout << "Cenario 1: insercao crescente\n";
    for (int x : vector<int>{10, 20, 30, 40, 50, 60}) insertBoth(x);
    insertBoth(30);
    cout << "Cenario 2: busca existente e inexistente\n";
    checkCondition("busca do valor 40", avl.search(40) == rb.search(40) && avl.search(40));
    checkCondition("busca do valor 999", avl.search(999) == rb.search(999) && !avl.search(999));
    cout << "Cenario 3: remocoes (folha, no com filho, no com dois filhos e inexistente)\n";
    removeBoth(60); removeBoth(50); removeBoth(30); removeBoth(999);

    cout << "Cenario 4: inserir em ordem decrescente\n";
    for (int x : vector<int>{90, 80, 70, 60, 50, 40}) insertBoth(x);
    cout << "Cenario 5: inserir dados nao ordenados\n";
    for (int x : vector<int>{25, 5, 35, 15, 45, 55}) insertBoth(x);
    cout << "\n--- COMPARACAO ANTES DAS REMOCOES FINAIS ---\n";
    cout << "AVL: altura = " << avl.getHeight() << ", rotacoes acumuladas = " << avl.getRotations() << '\n';
    cout << "Rubro-Negra: altura = " << rb.getHeight() << ", rotacoes acumuladas = " << rb.getRotations() << '\n';
    cout << "Cenario 6: remover ate ficar vazia\n";
    for (int x : vector<int>{10, 20, 40, 70, 80, 90, 5, 15, 25, 35, 45, 55, 50, 60}) removeBoth(x);
    check("operacoes finais e invariantes das duas arvores");

    cout << "\n--- RESULTADO COMPARATIVO ---\n";
    cout << "Validacoes aprovadas: " << passed << "/" << total << '\n';
    cout << "AVL: altura final = " << avl.getHeight() << ", rotacoes acumuladas = " << avl.getRotations() << '\n';
    cout << "Rubro-Negra: altura final = " << rb.getHeight() << ", rotacoes acumuladas = " << rb.getRotations() << '\n';
    cout << "AVL valida? " << (avl.isValid() ? "SIM" : "NAO") << " | Rubro-Negra valida? " << (rb.isValid() ? "SIM" : "NAO") << '\n';
    cout << "Observacao: as alturas sao medidas em quantidade de nos no caminho mais longo; rotacoes sao acumuladas durante os testes.\n";
}

int main() {
    int option;
    do {
        cout << "\n==========================================\n"
             << " AMAZON ENGINEERING - ARVORES BALANCEADAS\n"
             << "==========================================\n"
             << "1. Utilizar arvore AVL\n2. Utilizar arvore Rubro-Negra\n"
             << "3. Executar testes automaticos comparativos\n0. Encerrar\n";
        option = readInt("Escolha uma opcao: ");
        if (option == 1) { AVLTree tree; runTreeMenu(tree, "AVL"); }
        else if (option == 2) { RBTree tree; runTreeMenu(tree, "Rubro-Negra"); }
        else if (option == 3) runAutomaticTests();
        else if (option != 0) cout << "Opcao invalida.\n";
    } while (option != 0);
    cout << "Programa encerrado.\n";
    return 0;
}

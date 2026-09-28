#include <iostream>
#include <cstring>
#include "lista.h"
#include "cadastro_usuario.h"
#include "cadastro_local.h"
#include "cadastro_doacao.h"

using namespace std;

void limpar_tela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");    
#endif
}

string perguntar_sn(const string& pergunta) {
    string resposta;
    do {
        cout << pergunta << " (S/N): ";
        cin >> resposta;
        for (auto &c : resposta) c = tolower(c);
        if (resposta != "s" && resposta != "n") {
            cout << "Opcao invalida. Digite 'S' para sim ou 'N' para nao." << endl;
        }
    } while (resposta != "s" && resposta != "n");
    return resposta;
}

int main() {
    cadastro_usuario cadastro_usuario;
    cadastro_local cadastro_local;
    cadastro_doacao cadastro_doacao;
    lista_alimentos lista_alimentos;
    lista_limpeza lista_limpeza;
    lista_higiene_pessoal lista_higiene_pessoal;
    int contador_doacao = 1;

    while (true) {
        cout << "\n=== Sistema de Doacoes ===\n";
        cout << "[1] - Fazer uma doacao\n";
        cout << "[2] - Listar doacoes\n";
        cout << "[3] - Listar locais de doacao\n";
        cout << "[4] - Fechar sistema\n";
        cout << "Digite a opcao desejada: ";

        string escolha;
        cin >> escolha;

        while (atoi(escolha.c_str()) < 1 || atoi(escolha.c_str()) > 4) {
            cout << "Opcao invalida. Digite novamente: ";
            cin >> escolha;
        }

        if (escolha[0] == '4') {
            cout << "Fechando o sistema. Obrigado por usar!\n";
            break;
        } else if (escolha[0] == '2') {
            cout<< "\n\n";
            cadastro_doacao.listar_doacoes();
            continue;
        } else if (escolha[0] == '3') {
            cout<< "\n\n";
            cadastro_local.listar_locais();
            continue;
        }
        
        doacao nova_doacao;
        string cpf;
        if (perguntar_sn("Voce ja e um usuario cadastrado?") == "s") {
            iniciar_doacao:
            cout << "Insira seu CPF: ";
            cin >> cpf;

            bool encontrado = false;
            for (const auto& user : cadastro_usuario.usuarios) {
                if (user.get_cpf() == atoi(cpf.c_str())) {
                    nova_doacao.set_doador(user.get_nome());
                    encontrado = true;
                    continue;
                }
            }
            if (!encontrado) {
                cout << "Usuario nao encontrado. Vamos fazer seu cadastro.\n";
            } else {
                goto continuar_doacao;
            }
        }

        {
            usuario novo_usuario;
            novo_usuario.set_nome();
            novo_usuario.set_idade();
            novo_usuario.set_cpf();
            novo_usuario.set_email();
            cadastro_usuario.adicionar_usuario(&novo_usuario);
            nova_doacao.set_doador(novo_usuario.get_nome());
        }

        limpar_tela();
        continuar_doacao:
        nova_doacao.set_num_da_doacao(contador_doacao);

        cout << "\nSelecione o local de doacao:\n";
        cadastro_local.listar_locais();
        string local_index;
        cin >> local_index;
        while (atoi(local_index.c_str()) < 1 || atoi(local_index.c_str()) > cadastro_local.locais.size()) {
            cout << "Selecao invalida. Tente novamente: ";
            cin >> local_index;
        }
        nova_doacao.set_local(cadastro_local.locais[atoi(local_index.c_str()) - 1].get_nome());

        bool confirmador;
        do {
            cout << "\n";
            cout << "Insira a data da doacao (DD/MM/AAAA): ";
            string data;
            cin >> data;
            confirmador = nova_doacao.set_data(data);
            if (!confirmador) {
                cout << "Data invalida. Tente novamente.\n";
            }
        } while (confirmador == false);
            
        cout << "Tipo de doacao:\n[1] Higiene Pessoal\n[2] Limpeza\n[3] Alimentos\nEscolha: ";
        string tipo;
        cin >> tipo;
        while (atoi(tipo.c_str()) < 1 || atoi(tipo.c_str()) > 3) {
            cout << "Opcao invalida. Tente novamente: ";
            cin >> tipo;
        }
        limpar_tela();
        string nome_item;
        int indice = -1;
        switch (atoi(tipo.c_str())) {
            case 1:
                lista_higiene_pessoal.print_lista(lista_higiene_pessoal.higiene_pessoal);
                cout << "Insira apenas o nome do item (Ou 'new' para novo): ";
                cin.ignore(); getline(cin >> ws, nome_item);
                indice = lista_higiene_pessoal.adicionar_elemento(lista_higiene_pessoal.higiene_pessoal, nome_item);
                nova_doacao.set_tipo_doacao("Higiene Pessoal");
                nova_doacao.set_item(lista_higiene_pessoal.higiene_pessoal[indice].get_nome());
                break;
            case 2:
                lista_limpeza.print_lista(lista_limpeza.limpeza);
               cout << "Insira apenas o nome do item (Ou 'new' para novo): ";
                cin.ignore(); getline(cin >> ws, nome_item);
                indice = lista_limpeza.adicionar_elemento(lista_limpeza.limpeza, nome_item);
                nova_doacao.set_tipo_doacao("Limpeza");
                nova_doacao.set_item(lista_limpeza.limpeza[indice].get_nome());
                break;
            case 3:
                lista_alimentos.print_lista(lista_alimentos.alimentos);
                cout << "Insira apenas o nome do item (Ou 'new' para novo): ";
                cin.ignore(); getline(cin >> ws, nome_item);
                indice = lista_alimentos.adicionar_elemento(lista_alimentos.alimentos, nome_item);
                nova_doacao.set_tipo_doacao("Alimentos");
                nova_doacao.set_item(lista_alimentos.alimentos[indice].get_nome());
                break;
        }   

        if (indice == -1) {
            cout << "Erro ao adicionar item. Tente novamente.\n";
            continue;
        }

        cadastro_doacao.adicionar_doacao(nova_doacao);
        contador_doacao++;

        cout << "\nDoacao registrada com sucesso!\n";
        if (perguntar_sn("Deseja realizar outra doacao?") == "n") {
            cout << "Obrigado por sua doacao!\n";
        }else{
            goto iniciar_doacao;
        }
        limpar_tela();
    }

    return 0;
}

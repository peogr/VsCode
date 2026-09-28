
#ifndef CADASTRO_USUARIO_H
#define CADASTRO_USUARIO_H
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdint>
#include <string>
#include <regex>
#include <limits>
using namespace std;

class usuario {
private:
    string nome;
    int idade;
    uint64_t cpf;
    string email;

public:
    void set_nome() {
        while (true) {
            cout << "Digite seu nome: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            getline(cin, nome);
            if (!nome.empty()) break;
            cout << "Nome nao pode ser vazio.\n";
        }
    }

    void set_idade() {
        while (true) {
            cout << "Digite sua idade: ";
            string temp;
            cin >> temp;
            try {
                idade = stoi(temp);
                if (idade >= 0 && idade <= 120) break;
                else cout << "Idade invalida. Insira uma idade entre 0 e 120.\n";
            } catch (...) {
                cout << "Valor invalido. Por favor, insira um numero.\n";
            }
        }
    }

    void set_cpf() {
    while (true) {
        cout << "Digite seu CPF (apenas numeros, 11 digitos): ";
        string cpf_str;
        cin >> cpf_str;

        if (cpf_str.length() == 11 && all_of(cpf_str.begin(), cpf_str.end(), ::isdigit)) {
            try {
                cpf = stoull(cpf_str); // agora pode ir direto
                break;
            } catch (const exception& e) {
                cout << "Erro ao converter CPF: " << e.what() << endl;
            }
        } else {
            cout << "CPF invalido. Certifique-se de que tem exatamente 11 digitos numericos.\n";
        }
    }
}

    void set_email() {
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        while (true) {
            cout << "Digite seu email: ";
            getline(cin, email);

            if (regex_match(email, regex(R"(\S+@\S+\.\S+)"))) {
                break;
            } else {
                cout << "Email invalido. Tente novamente.\n";
            }
        }
    }

    string get_nome() const {
        return nome;
    }

    int get_idade() const {
        return idade;
    }

    uint32_t get_cpf() const {
        return cpf;
    }

    string get_email() const {
        return email;
    }
};


class cadastro_usuario{
public:
    std::vector<usuario> usuarios;
    public: void adicionar_usuario(usuario *novo) {
        if (usuarios.size() < 100) { // Limite de 100 usuários
            usuarios.push_back(*novo);
            cout << "Usuario adicionado com sucesso." << endl;
        }
        }
    public: void listar_usuarios (){
        if (usuarios.empty()) {
            cout << "Nenhum usuario cadastrado." << endl;
            return;
        }
        for (const auto& user : usuarios) {
            cout << "Nome: " << user.get_nome() << ", Idade: " << user.get_idade() 
                      << ", CPF: " << user.get_cpf() << ", Email: " << user.get_email() << endl;
        }
    }
    /*public: void remover_usuario(uint32_t cpf) {
        for (auto it = usuarios.begin(); it != usuarios.end(); ++it) {
            if (it->get_cpf() == cpf) {
                usuarios.erase(it);
                std::cout << "Usuario removido com sucesso." << std::endl;
                return;
            }
        }
        std::cout << "Usuario não encontrado." << std::endl;
    }*/
};

#endif 
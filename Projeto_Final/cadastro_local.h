#ifndef CADASTRO_LOCAL_H
#define CADASTRO_LOCAL_H
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdint>
#include <string>
using namespace std;

class local {
private:
    string nome;
    string estado;
    string cidade;
public:
    void set_nome(string nome) {
        this->nome = nome;
    }
    void set_estado(string estado) {
        this->estado = estado;
    }
    void set_cidade(string cidade) {
        this->cidade = cidade;
    }
    string get_nome() const {
        return nome;
    }

    string get_estado() const {
        return estado;
    }

    string get_cidade() const {
        return cidade;
    }
};

class cadastro_local{
public:
    std::vector<local> locais;
    cadastro_local(){
            locais.push_back(local());
            locais[0].set_nome("Hospital Santa Casa");
            locais[0].set_estado("SP");
            locais[0].set_cidade("Sao Paulo");
            locais.push_back(local());
            locais[1].set_nome("Batalhao da Policia Militar");
            locais[1].set_estado("SP");
            locais[1].set_cidade("Sao Paulo");
            locais.push_back(local());
            locais[2].set_nome("Centro de Apoio ao Idoso");
            locais[2].set_estado("SP");
            locais[2].set_cidade("Sao Paulo");
            locais.push_back(local());
            locais[3].set_nome("Centro de Apoio ao Morador de Rua");
            locais[3].set_estado("SP");
            locais[3].set_cidade("Sao Paulo");
            locais.push_back(local());
            locais[4].set_nome("Hospital de Cancer de Sao Paulo");
            locais[4].set_estado("SP");
            locais[4].set_cidade("Sao Paulo");
        }
    /*void adicionar_local(const local& novo_local) {
        locais.push_back(novo_local);
    }
    void remover_local(const string& nome) {
        for (auto it = locais.begin(); it != locais.end(); ++it) {
            if (it->get_nome() == nome) {
                locais.erase(it);
                return;
            }
        }
    }*/
    /*void atualizar_local(const string& nome, const string& novo_estado, const string& nova_cidade) {
        for (auto& loc : locais) {
            if (loc.get_nome() == nome) {
                loc.set_estado(novo_estado);
                loc.set_cidade(nova_cidade);
                return;
            } 
        }
    }*/
    void listar_locais() const {
        int i = 1;
        for (const auto& loc : locais) {
            cout << "[" << i << "]" << "Nome: " << loc.get_nome() << ", Estado: " << loc.get_estado() << ", Cidade: " << loc.get_cidade() << endl;
            i++;
        }
    }
};

#endif 
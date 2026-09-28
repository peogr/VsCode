#include <iostream>
#include <vector>
#include <cstring>
#include <cstdint>
#include <string>
using namespace std;

bool data_valida(const string& data_str) {
    if (data_str.length() != 10 || data_str[2] != '/' || data_str[5] != '/') {
        return false;
    }

    int dia, mes, ano;
    try {
        dia = stoi(data_str.substr(0, 2));
        mes = stoi(data_str.substr(3, 2));
        ano = stoi(data_str.substr(6, 4));
    } catch (...) {
        return false; 
    }

    if (mes < 1 || mes > 12) return false;

    
    int dias_no_mes[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

    
    bool bissexto = (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
    if (mes == 2 && bissexto) dias_no_mes[1] = 29;

    
    if (dia < 1 || dia > dias_no_mes[mes - 1]) return false;

    return true;
}

class doacao {
private:
    int num_da_doacao;
    string doador;
    string local;
    string data;
    string item;
    string tipo_doacao;
public:
    void set_num_da_doacao(int num) {
        num_da_doacao = num;
    }

    void set_doador(string doador) {
        this->doador = doador;
    }

    void set_local(string local) {
        this->local = local;
    }

    bool set_data(const string& data) {
        if (data_valida(data)) {
            this->data = data;
            return true;
        }else {
            cout << "Data invalida. Formato correto: DD/MM/AAAA\n";
            return false;
        }
    }

    void set_item(string item) {
        this->item = item;
    }
    void set_tipo_doacao(string tipo_doacao) {
        this->tipo_doacao = tipo_doacao;
    }
    int get_num_da_doacao() const {
        return num_da_doacao;
    }

    string get_doador() const {
        return doador;
    }

    string get_local() const {
        return local;
    }

    string get_data() const {
        return data;
    }

    string get_item() const {
        return item;
    }
    string get_tipo_doacao() const {
        return tipo_doacao;
    }
};

class cadastro_doacao{
private:
    std::vector<doacao> doacoes;
public:
    void adicionar_doacao(const doacao& nova_doacao) {
        doacoes.push_back(nova_doacao);
    }
    void listar_doacoes() const {
        for (const auto& doacao : doacoes) { 
            cout << "Numero da Doacao: " << doacao.get_num_da_doacao() << endl
                 << ", Doador: " << doacao.get_doador() << endl
                 << ", Local: " << doacao.get_local() << endl
                 << ", Data: " << doacao.get_data() << endl
                 << ", Item: " << doacao.get_item() << endl
                 << ", Tipo de Doacao: " << doacao.get_tipo_doacao() << endl
                 << "\n";
        }
    }
};

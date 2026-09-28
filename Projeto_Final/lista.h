#ifndef LISTA_H
#define LISTA_H
#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;
class elemento {
private:
    char nome[50];
    double qtd = 0.0;
    char medida[10];
public:
    void set_nome(string nome) {
        strncpy(this->nome, nome.c_str(), sizeof(this->nome) - 1);
        this->nome[sizeof(this->nome) - 1] = '\0';
    }
    void set_qtd(double qtd) {
        this->qtd = qtd;
    }
    void set_medida(string medida) {
        strncpy(this->medida, medida.c_str(), sizeof(this->medida) - 1);
        this->medida[sizeof(this->medida) - 1] = '\0';
    }
    const char* get_nome() const {
        return nome;
    }
    double get_qtd() const {
        return qtd;
    }
    const char* get_medida() const {
        return medida;
    }
};

class lista {
public:
    int adicionar_elemento(std::vector<elemento>& elementos, std::string nome) {
        std::transform(nome.begin(), nome.end(), nome.begin(), ::tolower);
        if (nome == "new") {
            elemento novo;
            std::string nome_item;
            char resposta;

            do {
                std::cout << "O que voce deseja adicionar?\n> ";
                std::getline(std::cin, nome_item);

                std::cout << "Nome inserido: " << nome_item << std::endl;

                while (true) {
                    std::cout << "Deseja continuar com esse nome? (S/N)\n> ";
                    std::cin >> resposta;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // limpa resto da linha
                    resposta = std::tolower(resposta);

                    if (resposta == 's' || resposta == 'n') {
                        break;
                    } else {
                        std::cout << "Resposta invalida. Digite S ou N:\n";
                    }
                }
            } while (resposta != 's');

            novo.set_nome(nome_item);

            std::string medida;
            do {
                std::cout << "Qual a medida do item? (Kg, l, Un)\n> ";
                std::cin >> medida;
                if (medida != "Kg" && medida != "l" && medida != "Un") {
                    std::cout << "Medida invalida. Por favor, insira Kg, l ou Un.\n";
                }
            } while (medida != "Kg" && medida != "l" && medida != "Un");

            novo.set_medida(medida);
            elementos.push_back(novo);
            return elementos.size() - 1;

        } else {
            for (size_t i = 0; i < elementos.size(); i++) {
                std::string nome_elemento = elementos[i].get_nome();
                std::transform(nome_elemento.begin(), nome_elemento.end(), nome_elemento.begin(), ::tolower);
                if (nome == nome_elemento) {
                    string qtd;
                    do{
                        if(atoi(qtd.c_str())<0){
                            std::cout << "Quantidade nao pode ser negativa. Tente novamente.\n";
                        }
                        std::cout << "Qual a quantidade a ser doada?\n> ";
                        std::cin >> qtd;
                    }while(atoi(qtd.c_str())<0);
                    elementos[i].set_qtd(elementos[i].get_qtd() + atoi(qtd.c_str()));
                    std::cout << "Quantidade atualizada: "
                              << elementos[i].get_qtd() << " "
                              << elementos[i].get_medida() << std::endl;
                    return i;
                }
            }
            std::cout << "Elemento nao encontrado. Por favor, adicione o elemento primeiro.\n";
            return -1;
        }
    }

    /*void remover_elemento(std::vector<elemento>& elementos, const std::string& nome) {
        auto it = find_if(elementos.begin(), elementos.end(),
                               [&nome](const elemento& el) {
                                   return el.get_nome() == nome;
                               });
        if (it != elementos.end()) {
            elementos.erase(it);
            std::cout << "Elemento removido com sucesso.\n";
        } else {
            std::cout << "Elemento nao encontrado.\n";
        }
    }*/

    void print_lista(const std::vector<elemento>& elementos) const {
        if (elementos.empty()) {
            std::cout << "Lista vazia.\n";
            return;
        }

        for (const auto& el : elementos) {
            std::cout << el.get_nome()
                      << " - " << el.get_qtd()
                      << " " << el.get_medida() << std::endl;
        }
    }
};
class lista_alimentos : public lista{
    public:
    std::vector<elemento> alimentos;
    lista_alimentos(){
        alimentos.push_back(elemento());
        alimentos[0].set_nome("Arroz");
        alimentos[0].set_medida("Kg");
        alimentos.push_back(elemento());
        alimentos[1].set_nome("Feijao");
        alimentos[1].set_medida("Kg");
        alimentos.push_back(elemento());
        alimentos[2].set_nome("Macarrao");
        alimentos[2].set_medida("Kg");
        alimentos.push_back(elemento());
        alimentos[3].set_nome("Carne");
        alimentos[3].set_medida("Kg");
        alimentos.push_back(elemento());
        alimentos[4].set_nome("Frango");
        alimentos[4].set_medida("Kg");
        alimentos.push_back(elemento());
        alimentos[5].set_nome("Farinha");
        alimentos[5].set_medida("Kg");
        alimentos.push_back(elemento());
        alimentos[6].set_nome("Batata");
        alimentos[6].set_medida("Kg");
        alimentos.push_back(elemento());
        alimentos[7].set_nome("Cenoura");
        alimentos[7].set_medida("Kg");
        alimentos.push_back(elemento());
        alimentos[8].set_nome("Sal");
        alimentos[8].set_medida("Kg");
        alimentos.push_back(elemento());
        alimentos[9].set_nome("Acucar");
        alimentos[9].set_medida("Kg");
        alimentos.push_back(elemento());
        alimentos[10].set_nome("Cebola");
        alimentos[10].set_medida("Kg");
        alimentos.push_back(elemento());
        alimentos[11].set_nome("Alho");
        alimentos[11].set_medida("Kg");
        alimentos.push_back(elemento());
        alimentos[12].set_nome("Ovo");
        alimentos[12].set_medida("Un");
        alimentos.push_back(elemento());
        alimentos[13].set_nome("Queijo");
        alimentos[13].set_medida("Kg");
        alimentos.push_back(elemento());
        alimentos[14].set_nome("Pao");
        alimentos[14].set_medida("Kg");
        alimentos.push_back(elemento());
        alimentos[15].set_nome("Manteiga");
        alimentos[15].set_medida("Kg");
        alimentos.push_back(elemento());
        alimentos[16].set_nome("Leite");
        alimentos[16].set_medida("l");
        alimentos.push_back(elemento());
        alimentos[17].set_nome("Tomate");
        alimentos[17].set_medida("Kg");
        alimentos.push_back(elemento());
        alimentos[18].set_nome("Suco");
        alimentos[18].set_medida("l");
        alimentos.push_back(elemento());
        alimentos[19].set_nome("Agua");
        alimentos[19].set_medida("l");
        alimentos.push_back(elemento());
        alimentos[20].set_nome("Oleo");
        alimentos[20].set_medida("l");

    }
};

class lista_limpeza : public lista {
    public: 
    std::vector<elemento> limpeza;
    lista_limpeza(){
        limpeza.push_back(elemento());
        limpeza[0].set_nome("Detergente");
        limpeza[0].set_medida("Un");

        limpeza.push_back(elemento());
        limpeza[1].set_nome("Sabao em po");
        limpeza[1].set_medida("Kg");

        limpeza.push_back(elemento());
        limpeza[2].set_nome("Desinfetante");
        limpeza[2].set_medida("l");

        limpeza.push_back(elemento());
        limpeza[3].set_nome("Agua sanitaria");
        limpeza[3].set_medida("l");

        limpeza.push_back(elemento());
        limpeza[4].set_nome("Esponja");
        limpeza[4].set_medida("Un");

        limpeza.push_back(elemento());
        limpeza[5].set_nome("Vassoura");
        limpeza[5].set_medida("Un");

        limpeza.push_back(elemento());
        limpeza[6].set_nome("Rodo");
        limpeza[6].set_medida("Un");

        limpeza.push_back(elemento());
        limpeza[7].set_nome("Pano de chao");
        limpeza[7].set_medida("Un");

        limpeza.push_back(elemento());
        limpeza[8].set_nome("Saco de lixo");
        limpeza[8].set_medida("Un");
        
        limpeza.push_back(elemento());
        limpeza[9].set_nome("Luvas de borracha");
        limpeza[9].set_medida("Un");
    }
};

class lista_higiene_pessoal : public lista {
    public: 
    std::vector<elemento> higiene_pessoal;
    lista_higiene_pessoal(){
        higiene_pessoal.push_back(elemento());
        higiene_pessoal[0].set_nome("Sabonete");
        higiene_pessoal[0].set_medida("Un");

        higiene_pessoal.push_back(elemento());
        higiene_pessoal[1].set_nome("Shampoo");
        higiene_pessoal[1].set_medida("l");

        higiene_pessoal.push_back(elemento());
        higiene_pessoal[2].set_nome("Condicionador");
        higiene_pessoal[2].set_medida("l");

        higiene_pessoal.push_back(elemento());
        higiene_pessoal[3].set_nome("Pasta de dente");
        higiene_pessoal[3].set_medida("Un");

        higiene_pessoal.push_back(elemento());
        higiene_pessoal[4].set_nome("Escova de dente");
        higiene_pessoal[4].set_medida("Un");

        higiene_pessoal.push_back(elemento());
        higiene_pessoal[5].set_nome("Desodorante");
        higiene_pessoal[5].set_medida("Un");

        higiene_pessoal.push_back(elemento());
        higiene_pessoal[6].set_nome("Fio dental");
        higiene_pessoal[6].set_medida("Un");

        higiene_pessoal.push_back(elemento());
        higiene_pessoal[7].set_nome("Lencos umedecidos");
        higiene_pessoal[7].set_medida("Un");
        
        higiene_pessoal.push_back(elemento());
        higiene_pessoal[8].set_nome("Creme dental");
        higiene_pessoal[8].set_medida("Un");

        higiene_pessoal.push_back(elemento());
        higiene_pessoal[9].set_nome("Hidratante");
        higiene_pessoal[9].set_medida("Un");
        
        higiene_pessoal.push_back(elemento());
        higiene_pessoal[10].set_nome("Protetor solar");
        higiene_pessoal[10].set_medida("Un");

        higiene_pessoal.push_back(elemento());
        higiene_pessoal[11].set_nome("Lamina de barbear");
        higiene_pessoal[11].set_medida("Un");

        higiene_pessoal.push_back(elemento());
        higiene_pessoal[12].set_nome("Desodorante aerosol");
        higiene_pessoal[12].set_medida("Un");
        
        higiene_pessoal.push_back(elemento());
        higiene_pessoal[13].set_nome("Absorvente");
        higiene_pessoal[13].set_medida("Un");

        higiene_pessoal.push_back(elemento());
        higiene_pessoal[14].set_nome("Fralda descartavel");
        higiene_pessoal[14].set_medida("Un");
        
        higiene_pessoal.push_back(elemento());
        higiene_pessoal[15].set_nome("Algodao");
        higiene_pessoal[15].set_medida("Un");

        higiene_pessoal.push_back(elemento());
        higiene_pessoal[16].set_nome("Cotonete");
        higiene_pessoal[16].set_medida("Un");

        higiene_pessoal.push_back(elemento());
        higiene_pessoal[17].set_nome("Escova de cabelo");
        higiene_pessoal[17].set_medida("Un");

        higiene_pessoal.push_back(elemento());
        higiene_pessoal[18].set_nome("Pente");
        higiene_pessoal[18].set_medida("Un");
        
        higiene_pessoal.push_back(elemento());
        higiene_pessoal[19].set_nome("Lixa de unha");
        higiene_pessoal[19].set_medida("Un");
    }
};

#endif
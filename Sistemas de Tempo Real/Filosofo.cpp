#include <iostream> 
#include <vector>
#include <string> 
#include <mutex>
#include <chrono>
#include <random>

class Filosofo {
    private:
        std::string estadoAtual;
        Garfo& garfoEsq;
        Garfo& garfoDir;
        
        static const std::vector<std::string> listaDeEstados;

    public:
        Filosofo(Garfo& esq, Garfo& dir) : garfoEsq(esq), garfoDir(dir) {
            setEstadoAtual(0); 
        }

        std::string getEstadoAtual(){
                return estadoAtual;
            }

            // 0 pensando
            // 1 com fome
            // 2 comendo
        void setEstadoAtual (int novoEstado){
                estadoAtual = listaDeEstados[novoEstado];
            }

        void pegarGarfos() {
            bool try1 = garfoEsq.pegar();
            bool try2 = garfoDir.pegar();

            if (try1 && try2) {
                
                setEstadoAtual(2); //comendo
            } else {
                
                if (try1) {
                    garfoEsq.soltar();
                }
                
                if (try2) {
                    garfoDir.soltar();
                }
                
                setEstadoAtual(1); //pensando VERIFICAR!!!!
            }
        }

        void soltarGarfos(){
            garfoEsq.soltar();
            garfoDir.soltar();
        }
};

const std::vector<std::string> Filosofo::listaDeEstados = {
    "Pensando",
    "Com Fome",
    "Comendo"
};

class Garfo {
    private:
        std::mutex ocupado;

    public:
        bool pegar(){
            bool result = ocupado.try_lock();
            return result;
        }
        void soltar(){
            ocupado.unlock();
        }

};

int main(){
    Garfo vetorGarfos[5];
    

    return 0;
}
#include <atomic>
#include <iostream> 
#include <vector>
#include <string> 
#include <mutex>
#include <chrono>
#include <random>
#include <thread>
#include <array>
#include <memory>

std::mutex mutexEstados;
std::vector<char> estados {'P', 'P', 'P', 'P', 'P', 'L', 'L', 'L', 'L', 'L'};

void definirNovoEstado(int indice, char novoEstado) {
    if (indice < 0 || indice >= static_cast<int>(estados.size())) {
        return;
    }

    std::lock_guard<std::mutex> lock(mutexEstados);
    if (estados[indice] == novoEstado) {
        return;
    }

    estados[indice] = novoEstado;

    for (std::size_t i = 0; i < estados.size(); ++i) {
        if (i > 0) {
            std::cout << ',';
        }
        std::cout << estados[i];
    }
    std::cout << '\n';
}

class Garfo {
    private:
        int indiceEstado;
        std::mutex ocupado;
        char estadoAtual;

        static const std::vector<char> listaDeEstadosGarfos;

    public:
        explicit Garfo(int indice) : indiceEstado(indice),
                                     estadoAtual(listaDeEstadosGarfos[0]) {}

        bool pegar() {
            if (ocupado.try_lock()) {
                estadoAtual = listaDeEstadosGarfos[1];
                definirNovoEstado(indiceEstado, estadoAtual);
                return true;
            }
            return false;
        }

        void soltar() {
            estadoAtual = listaDeEstadosGarfos[0];
            definirNovoEstado(indiceEstado, estadoAtual);
            ocupado.unlock();
        }
};

class Filosofo {
    private:
        int id;
        char estadoAtual;
        Garfo& garfoEsq;
        Garfo& garfoDir;
        std::chrono::steady_clock::duration tempoTotalEsperando{};
        
        static const std::vector<char> listaDeEstadosFilosofos;

        std::random_device rd;
        std::mt19937 gen{rd()};

        int comeu;

    public:
        Filosofo(int identificador, Garfo& esq, Garfo& dir)
            : id(identificador), estadoAtual(listaDeEstadosFilosofos[0]),
              garfoEsq(esq), garfoDir(dir), comeu(0) {}

        int getId() const {
            return id;
        }
        
        auto getComeu(){
            return comeu;
        }

        std::chrono::steady_clock::duration getTempoTotalEsperando() const {
            return tempoTotalEsperando;
        }

        int TempoRand() {
            return std::uniform_int_distribution<int>{0, 5000}(gen);
        }

        char getEstadoAtual(){
            return estadoAtual;
        }

            // 0 pensando
            // 1 com fome
            // 2 comendo
        void setEstadoAtual (int novoEstado){
            if (estadoAtual == listaDeEstadosFilosofos[novoEstado]) return;
            estadoAtual = listaDeEstadosFilosofos[novoEstado];
            definirNovoEstado(id - 1, estadoAtual);
        }

        bool pegarGarfos() {
            bool try1 = garfoEsq.pegar();
            bool try2 = garfoDir.pegar();

            if (try1 && try2) {
                return true;
            } else {
                
                if (try1) {
                    garfoEsq.soltar();
                }
                
                if (try2) {
                    garfoDir.soltar();
                }

                return false;
            }
        }

        void soltarGarfos(){
            garfoEsq.soltar();
            garfoDir.soltar();
        }

        void executar(std::atomic<bool>& rodando) {
            while (rodando.load()) {
                setEstadoAtual(0); //pensa por tempo aleatorio
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(TempoRand()));

                if (!rodando.load()) {
                    break;
                }

                setEstadoAtual(1);
                const auto inicioEspera = std::chrono::steady_clock::now();

                bool garfosPegos = false;

                while (rodando.load() && !garfosPegos) {
                    garfosPegos = pegarGarfos();
                    if (!garfosPegos) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    }
                }

                tempoTotalEsperando +=
                    std::chrono::steady_clock::now() - inicioEspera;

                if (!garfosPegos) {
                    break;
                }

                if (rodando.load()) {
                    setEstadoAtual(2); 
                    std::this_thread::sleep_for(std::chrono::milliseconds(TempoRand()));
                }

                soltarGarfos();
                comeu++;
            }

            setEstadoAtual(0);
        }
};

const std::vector<char> Filosofo::listaDeEstadosFilosofos = {'P', 'F', 'C'};
const std::vector<char> Garfo::listaDeEstadosGarfos = {'L', 'O'};

int main(){
    constexpr std::size_t quantidadeFilosofos = 5;
    std::array<std::unique_ptr<Garfo>, quantidadeFilosofos> vetorGarfos;
    std::array<std::unique_ptr<Filosofo>, quantidadeFilosofos> filosofos;
    std::array<std::thread, quantidadeFilosofos> threads;
    std::atomic<bool> rodando{true};

    for (std::size_t i = 0; i < quantidadeFilosofos; ++i) {
        vetorGarfos[i] = std::make_unique<Garfo>( static_cast<int>(quantidadeFilosofos + i));
    }

    for (std::size_t i = 0; i < quantidadeFilosofos; ++i) {
        filosofos[i] = std::make_unique<Filosofo>(static_cast<int>(i) + 1, *vetorGarfos[i], *vetorGarfos[(i + 1) % quantidadeFilosofos]);
    }

    
    for (std::size_t i = 0; i < quantidadeFilosofos; ++i) {
        threads[i] = std::thread([&, i] {filosofos[i]->executar(rodando);});
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(60000*5));
    rodando.store(false);

    for (std::thread& thread : threads) {
        thread.join();
    }

    std::chrono::steady_clock::duration somaTemposEspera{};
    for (const auto& filosofo : filosofos) {
        const auto tempoEspera = filosofo->getTempoTotalEsperando();
        somaTemposEspera += tempoEspera;
        std::cout << "Tempo esperando por garfos do filosofo "
                  << filosofo->getId() << ": "
                  << std::chrono::duration<double, std::milli>(tempoEspera).count()
                  << " ms\n";
    }

    std::cout << "Soma dos tempos de espera: "
              << std::chrono::duration<double, std::milli>(somaTemposEspera).count()
              << " ms\n";

    for (const auto& filosofo : filosofos) {
        const auto pratos = filosofo->getComeu();
        std::cout << "Filosofo "
                  << filosofo->getId() << ": "
                  << "comeu "
                  << pratos 
                  << " vezes\n";
    }

    return 0;
}
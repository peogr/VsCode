#### ===================================================

#### COMANDOS UTILIZADOS NO EXPERIMENTO DOS FILÓSOFOS

#### ===================================================

#### 

#### 1\. COMPILAÇÃO DO CÓDIGO C++:

#### g++ -O2 -std=c++17 filosofo.cpp -o filosofo -pthread

#### 

#### 2\. EXECUÇÃO EM TEMPO REAL (SCHED\_RR) COM MEDIÇÃO DO KERNEL:

#### sudo chrt -r 50 /usr/bin/time -v ./filosofo

#### 

#### 

#### 3\. SALVAR SAÍDA DA MEDIÇÃO EM UM ARQUIVO DE TEXTO:

#### sudo chrt -r 50 /usr/bin/time -v ./filosofo > resultado\_sched\_rr.txt 2>\&1


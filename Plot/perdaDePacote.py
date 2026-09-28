import pandas as pd
import numpy as np

# --- Configurações ---
CSV_FILE = r"C:\Users\peogr\Downloads\log.csv"
# Esta é a coluna que contém o seu 'baseValue + 1' (sequência de 1 a 100)
SEQUENCE_COLUMN_NAME = 'blr' 
SEQUENCE_MODULO = 100 # O 'baseValue' vai de 0 a 99
SEQUENCE_OFFSET = 1   # O valor salvo é 'baseValue + 1'
TIME_COLUMN_NAME = 'time' # Nome da coluna de tempo (em ms)

def analyze_csv_loss(file_path):
    print(f"Carregando arquivo CSV: {file_path}...")
    try:
        # Tenta ler o CSV, pulando linhas com erro
        df = pd.read_csv(file_path, on_bad_lines='skip')
    except FileNotFoundError:
        print(f"Erro: Arquivo '{file_path}' não encontrado.")
        return
    except Exception as e:
        print(f"Erro inesperado ao ler o CSV: {e}")
        return

    # Limpa espaços em branco dos nomes das colunas
    df.columns = df.columns.str.strip()

    # --- Verificação de Colunas ---
    if SEQUENCE_COLUMN_NAME not in df.columns:
        print(f"Erro: A coluna de sequência '{SEQUENCE_COLUMN_NAME}' não foi encontrada.")
        print(f"Colunas encontradas: {list(df.columns)}")
        return
        
    if TIME_COLUMN_NAME not in df.columns:
        print(f"Erro: A coluna de tempo '{TIME_COLUMN_NAME}' não foi encontrada.")
        print(f"Colunas encontradas: {list(df.columns)}")
        return

    # --- Limpeza e Preparação dos Dados ---
    
    # Converte ambas as colunas para números, forçando erros a virar 'NaN'
    df[SEQUENCE_COLUMN_NAME] = pd.to_numeric(df[SEQUENCE_COLUMN_NAME], errors='coerce')
    df[TIME_COLUMN_NAME] = pd.to_numeric(df[TIME_COLUMN_NAME], errors='coerce')
    
    # Remove linhas onde *qualquer uma* das colunas principais é inválida
    df_cleaned = df.dropna(subset=[SEQUENCE_COLUMN_NAME, TIME_COLUMN_NAME])

    # Cria as listas de dados
    # Usamos int64 para garantir que os timestamps caibam
    sequence_data = df_cleaned[SEQUENCE_COLUMN_NAME].astype(int).tolist()
    time_data = df_cleaned[TIME_COLUMN_NAME].astype('int64').tolist() 

    if not sequence_data: # Se uma estiver vazia, a outra também estará
        print("As colunas de sequência ou tempo estão vazias ou contêm apenas dados inválidos.")
        return

    # --- Análise de Duração ---
    total_valid_lines = len(sequence_data)
    first_time_ms = time_data[0]
    last_time_ms = time_data[-1]
    total_duration_ms = last_time_ms - first_time_ms
    total_duration_sec = total_duration_ms / 1000.0

    print(f"Analisando {total_valid_lines} linhas válidas (com dados de tempo e sequência)...")
    
    if total_duration_sec <= 0:
        print("Duração total da coleta é zero ou negativa. Não é possível calcular a frequência.")
        # Prossegue mesmo assim para a análise de perda, mas a frequência será 0
        total_duration_sec = 0.0
    else:
        print(f"Duração total dos dados: {total_duration_sec:.2f} segundos (de {first_time_ms}ms a {last_time_ms}ms)")


    print(f"Analisando {len(sequence_data)} linhas válidas da coluna '{SEQUENCE_COLUMN_NAME}'...")

    packets_received = 0
    packets_lost = 0
    packets_duplicate = 0 # Novo contador para duplicados

    # Pega o primeiro valor como ponto de partida
    last_valid_seq_value = sequence_data[0]
    packets_received = 1

    # Itera por todos os outros valores a partir do segundo
    for i in range(1, len(sequence_data)):
        received_seq_value = sequence_data[i]

        # ***************************************************************
        # ** INÍCIO DA LÓGICA CORRIGIDA (para ignorar duplicados) **
        # ***************************************************************

        # 1. Verifica se é um pacote duplicado
        if received_seq_value == last_valid_seq_value:
            packets_duplicate += 1
            continue # Ignora esta linha e vai para a próxima

        # 2. Se não for duplicado, calcula qual era o próximo valor esperado
        #    (Ex: se o último foi 100, espera 1. Se foi 99, espera 100)
        next_expected_value = (last_valid_seq_value % SEQUENCE_MODULO) + SEQUENCE_OFFSET

        # 3. Verifica se o pacote recebido é o esperado
        if received_seq_value == next_expected_value:
            # Pacote recebido em ordem
            packets_received += 1
        else:
            # Não é duplicado e não é o próximo. É um salto (perda de pacotes).
            
            # Converte para a base 0-99 para calcular a diferença
            expected_num = (next_expected_value - SEQUENCE_OFFSET)
            received_num = (received_seq_value - SEQUENCE_OFFSET)
            
            # Calcula quantos pacotes foram perdidos no "salto"
            # (recebido - esperado + 100) % 100
            diff = (received_num - expected_num + 100) % SEQUENCE_MODULO
            
            if diff > 0:
                # O '+1' na linha é para corresponder ao índice do Excel/CSV (base 1)
                # Adicionamos +1 ao 'i' (base 0) e +1 do cabeçalho
                print(f"*** PERDA DETECTADA! (Linha CSV aprox. {i+2}): Esperado: {next_expected_value}, Recebido: {received_seq_value}. ({diff} pacotes perdidos) ***")
                packets_lost += diff
            
            packets_received += 1 # Conta o pacote que acabamos de receber

        # 4. Atualiza o último valor válido que vimos
        last_valid_seq_value = received_seq_value
        
        # ***************************************************************
        # ** FIM DA LÓGICA CORRIGIDA **
        # ***************************************************************

    # --- Imprime o Relatório Final ---
    print("\n--- Análise Concluída ---")
    total_packets_expected = packets_received + packets_lost
    loss_percentage = 0.0
    if total_packets_expected > 0:
        loss_percentage = (packets_lost / total_packets_expected) * 100

    # --- Cálculo de Frequência ---
    avg_frequency_hz = 0.0
    if total_duration_sec > 0:
        # Usamos packets_received pois é o número de pacotes *únicos*
        avg_frequency_hz = packets_received / total_duration_sec

    print(f"Duração Total da Coleta: {total_duration_sec:.2f} s")
    print(f"Frequência Média de Pacotes ÚNICOS: {avg_frequency_hz:.2f} Hz (pacotes/segundo)")
    print("-----------------------------------")
    print(f"Pacotes ÚNICOS Registrados: {packets_received}")
    print(f"Pacotes Duplicados Ignorados: {packets_duplicate}")
    print(f"Pacotes Perdidos Estimados: {packets_lost}")
    print(f"Total de Pacotes (Enviados): {total_packets_expected}")
    print(f"Taxa de Perda de Pacotes: {loss_percentage:.2f}%")

# --- Roda a análise ---
if __name__ == "__main__":
    analyze_csv_loss(CSV_FILE)
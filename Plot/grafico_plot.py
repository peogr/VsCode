import pandas as pd
import matplotlib.pyplot as plt
import re
import io
from scipy.signal import butter, filtfilt
from scipy.fft import fft, fftfreq
import numpy as np

# Caminho do CSV
csv_file_path = r"C:\Users\peogr\Downloads\log.csv"
processed_lines = []
column_names = []

try:
    # --- Lê todas as linhas ---
    with open(csv_file_path, 'r', encoding='utf-8') as f:
        lines = f.readlines()

    # --- Encontra o índice da ÚLTIMA linha que contém o cabeçalho ---
    header_index = -1
    for i, line in enumerate(lines):
        if 'time' in line.lower():
            header_index = i

    if header_index == -1:
        raise ValueError("Nenhum cabeçalho com 'time' foi encontrado.")

    # --- Pega somente os dados a partir do último cabeçalho ---
    lines = lines[header_index:]
    header_line = lines[0].strip()

    normalized_header = header_line.replace('\t', ',').strip(',')
    column_names = normalized_header.split(',')
    for i, name in enumerate(column_names):
        if not name:
            column_names[i] = f"Unnamed_{i}"

    # --- Processa as linhas seguintes ---
    for line in lines[1:]:
        line = line.strip()
        if not line:
            continue
        cleaned_line = re.sub(r'[\s\t,]+', ',', line).strip(',')
        parts = cleaned_line.split(',')
        if len(parts) > len(column_names):
            parts = parts[:len(column_names)]
        elif len(parts) < len(column_names):
            parts.extend([''] * (len(column_names) - len(parts)))
        processed_lines.append(','.join(parts))

    # Lê para DataFrame
    processed_csv_content = io.StringIO("\n".join(processed_lines))
    df = pd.read_csv(processed_csv_content, sep=',', names=column_names, header=None, index_col=False)
    print("CSV carregado e pré-processado com sucesso.")

except FileNotFoundError:
    print(f"Erro: O arquivo '{csv_file_path}' não foi encontrado.")
    exit()
except Exception as e:
    print(f"Ocorreu um erro ao carregar ou pré-processar o CSV: {e}")
    exit()

# --- Conversões ---
print("\n--- Informações iniciais do DataFrame (APÓS PRÉ-PROCESSAMENTO) ---")
print(df.head())
print("\nTipos de dados originais (APÓS PRÉ-PROCESSAMENTO):")
print(df.dtypes)

if 'time' in df.columns:
    print("\n--- Primeiros 20 valores brutos da coluna 'time' ---")
    print(df['time'].head(20))
else:
    print("Coluna 'time' não encontrada. Colunas disponíveis:")
    print(df.columns.tolist())

df['time'] = pd.to_numeric(df['time'], errors='coerce')
df = df.apply(pd.to_numeric, errors='coerce')

df_original_shape = df.shape[0]
df = df.dropna(subset=['time'])
df_new_shape = df.shape[0]
print(f"\nLinhas removidas devido a 'time' ser NaN: {df_original_shape - df_new_shape}")

# --- Converte 'time' de milissegundos para segundos ---
if df['time'].max() > 1e4:
    print("Detectado 'time' em milissegundos. Convertendo para segundos.")
    df['time'] = df['time'] / 1000.0

# --- Estatísticas ---
print("\n--- Estatísticas da coluna 'time' ---")
print(df['time'].describe())

if 'blf' in df.columns:
    print("\n--- Estatísticas da coluna 'blf' ---")
    print(df['blf'].describe())

# --- Seleção de colunas ---
colunas_desejadas = [ 'blf', 'blr']
colunas_validas = [col for col in colunas_desejadas if col in df.columns and (df[col] != 0).any()]
print(f"\nColunas válidas para plotar: {colunas_validas}")

if not colunas_validas:
    raise ValueError("Nenhuma coluna válida com dados diferentes de zero.")


# --- Plotagem da FFT do sinal original ---
'''for col in colunas_validas:
    sinal = df[col].dropna().values
    tempo = df['time'].dropna().values

    # Garante que o tempo e o sinal tenham o mesmo tamanho
    min_len = min(len(sinal), len(tempo))
    sinal = sinal[:min_len]
    tempo = tempo[:min_len]

    # Recalcula frequência de amostragem com base nesse trecho
    dt = np.median(np.diff(tempo))
    fs_local = 120

    N = len(sinal)
    yf = fft(sinal)
    xf = fftfreq(N, d=dt)[:N // 2]

    plt.figure(figsize=(12, 5))
    plt.plot(xf, 2.0/N * np.abs(yf[0:N//2]))
    plt.title(f"Espectro de Frequência (FFT) - {col} (original)")
    plt.xlabel("Frequência (Hz)")
    plt.ylabel("Magnitude")
    plt.grid(True)
    plt.tight_layout()
    plt.show()'''

# --- Filtro Butterworth passa-baixo ---
def butter_lowpass_filter(data, cutoff_freq, fs, order=4):
    nyquist = 0.5 * fs
    normal_cutoff = cutoff_freq / nyquist
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    return filtfilt(b, a, data)

# Estimativa da frequência de amostragem (fs)
df_sorted = df.sort_values('time')
time_diffs = df_sorted['time'].diff().dropna()
fs_est = 1 / time_diffs.median() if not time_diffs.empty else 50.0
fs_est = max(fs_est, 2 * 2.0 + 1)  # garante condição de Nyquist

cutoff_hz = 2.0  # frequência de corte em Hz
df_filtered = df.copy()

for col in colunas_validas:
    try:
        df_filtered[col] = butter_lowpass_filter(df[col], cutoff_hz, fs_est)
    except Exception as e:
        print(f"Erro ao aplicar filtro Butterworth na coluna '{col}': {e}")
        df_filtered[col] = df[col]


# --- Plotagem: original e filtrado ---
# --- Plotagem: original e filtrado (com deslocamento de 'a ccx') ---
plt.figure(figsize=(14, 6))
for col in colunas_validas:
    if col == 'a ccx':
        #plt.plot(df['time'], df[col] + 3, alpha=0.5, label=f"{col} (original) +3")
        plt.plot(df_filtered['time'], df_filtered[col] + 110, linewidth=2, label=f"{col} (filtrado) +110")
    if col == 'gear':
        plt.plot(df['time'], df[col]*1000, alpha=0.5, label=f"{col} (original)")
    else:
        plt.plot(df['time'], df[col], alpha=0.5, label=f"{col} (original)")
        plt.plot(df_filtered['time'], df_filtered[col], linewidth=2, label=f"{col} (filtrado)")
plt.xlabel("Tempo (s)")
plt.ylabel("Valor")
plt.title("Dados Filtrados")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()
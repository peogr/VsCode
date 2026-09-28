import pandas as pd
import matplotlib.pyplot as plt
import re
import io

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
df['time'] = pd.to_numeric(df['time'], errors='coerce')
df = df.apply(pd.to_numeric, errors='coerce')

# Remove linhas sem tempo válido
df = df.dropna(subset=['time'])

# Converte 'time' de ms para s, se necessário
if df['time'].max() > 1e4:
    print("Detectado 'time' em milissegundos. Convertendo para segundos.")
    df['time'] = df['time'] / 1000.0

# --- Seleção de colunas ---
colunas_desejadas = ['blf', 'blr']  # pode ajustar conforme seu log
colunas_validas = [
    col for col in colunas_desejadas
    if col in df.columns and (df[col].notna() & (df[col] != 0)).any()
]

print(f"Colunas válidas para plotar: {colunas_validas}")

if not colunas_validas:
    raise ValueError("Nenhuma coluna válida com dados diferentes de zero.")

# --- Plotagem dos sinais originais ---
plt.figure(figsize=(14, 6))
for col in colunas_validas:
    if col == 'a ccx':
        plt.plot(df['time'], df[col] + 110, label=f"{col} +110")
    elif col == 'gear':
        plt.plot(df['time'], df[col] * 1000, label=f"{col} *1000")
    else:
        plt.plot(df['time'], df[col], label=col)

plt.xlabel("Tempo (s)")
plt.ylabel("Valor")
plt.title("Sinais Originais (sem filtro)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

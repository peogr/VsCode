import os

# Caminho original do CSV
csv_path = r"C:\Users\peogr\Downloads\log.csv"
output_dir = os.path.dirname(csv_path)

try:
    with open(csv_path, 'r', encoding='utf-8') as f:
        lines = f.readlines()

    partes = []
    parte_atual = []
    indices_de_cabecalho = []

    # Identifica todas as linhas que são cabeçalhos (que têm 'time')
    for i, line in enumerate(lines):
        if 'time' in line.lower():
            indices_de_cabecalho.append(i)

    # Se não tiver nenhum cabeçalho, aborta
    if not indices_de_cabecalho:
        raise ValueError("Nenhum cabeçalho com 'time' encontrado no arquivo.")

    # Adiciona o índice do final do arquivo para facilitar a divisão
    indices_de_cabecalho.append(len(lines))

    # Separa as partes com base nos índices dos cabeçalhos
    for idx in range(len(indices_de_cabecalho) - 1):
        start = indices_de_cabecalho[idx]
        end = indices_de_cabecalho[idx + 1]
        parte = lines[start:end]
        partes.append(parte)

    # Salva cada parte em um novo CSV
    for i, parte in enumerate(partes):
        out_path = os.path.join(output_dir, f"parte_{i+1}.csv")
        with open(out_path, 'w', encoding='utf-8') as f:
            f.writelines(parte)
        print(f"Parte {i+1} salva em: {out_path}")

    print(f"\nTotal de partes extraídas: {len(partes)}")

except FileNotFoundError:
    print(f"Erro: Arquivo '{csv_path}' não encontrado.")
except Exception as e:
    print(f"Ocorreu um erro: {e}")

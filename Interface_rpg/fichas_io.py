import json

CAMINHO_ARQUIVO = "fichas.json"

CAMPOS_PADRAO = {
    "Iniciativa": 0,
    "Pv": 0,
    "PE": 0,
    # ... outros campos ...
    "Habilidades": []
}

def carregar_fichas():
    try:
        with open(CAMINHO_ARQUIVO, "r", encoding="utf-8") as f:
            return json.load(f)
    except FileNotFoundError:
        return {}

def salvar_fichas(fichas):
    with open(CAMINHO_ARQUIVO, "w", encoding="utf-8") as f:
        json.dump(fichas, f, indent=2, ensure_ascii=False)

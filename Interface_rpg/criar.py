from dearpygui.dearpygui import *
import json
import fichas_io  # seu módulo pra carregar e salvar fichas

habilidades_temp = []

def adicionar_habilidade_callback(sender, app_data):
    habilidade = get_value("input_nova_habilidade").strip()
    if habilidade:
        habilidades_temp.append(habilidade)
        set_value("input_nova_habilidade", "")
        atualizar_lista_habilidades()

def atualizar_lista_habilidades():
    delete_item("lista_habilidades", children_only=True)
    for i, hab in enumerate(habilidades_temp):
        add_text(f"{i+1}. {hab}", parent="lista_habilidades")

def salvar_ficha_callback(sender, app_data):
    fichas = fichas_io.carregar_fichas()

    nome = get_value("input_nome").strip()
    if not nome:
        print("Nome não pode ser vazio.")
        return

    if nome in fichas:
        print(f"A ficha '{nome}' já existe!")
        return

    ficha_nova = fichas_io.CAMPOS_PADRAO.copy()
    ficha_nova["Iniciativa"] = get_value("input_iniciativa")
    ficha_nova["Pv"] = get_value("input_pv")
    ficha_nova["PE"] = get_value("input_pm")
    ficha_nova["Habilidades"] = habilidades_temp.copy()

    fichas[nome] = ficha_nova
    fichas_io.salvar_fichas(fichas)

    delete_item("criar_ficha_window")
    print(f"Ficha '{nome}' criada com sucesso:")
    print(json.dumps(ficha_nova, indent=2, ensure_ascii=False))

def criar_ficha():
    global habilidades_temp
    habilidades_temp = []

    with window(label="Criar Ficha", width=500, height=650, tag="criar_ficha_window"):
        add_input_text(label="Nome do personagem", tag="input_nome")
        add_input_int(label="Iniciativa", tag="input_iniciativa")
        add_input_int(label="PV", tag="input_pv")
        add_input_int(label="PE", tag="input_pm")

        add_input_text(label="Nova Habilidade", tag="input_nova_habilidade", multiline=True, width=400, height=80)
        add_button(label="Adicionar Habilidade", callback=adicionar_habilidade_callback)

        with child_window(tag="lista_habilidades", width=400, height=150):
            pass  # aqui as habilidades aparecerão listadas

        add_button(label="Salvar Ficha", callback=salvar_ficha_callback)



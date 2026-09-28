from dearpygui.dearpygui import *
from PIL import Image
import json

# Dados das fichas
fichas = {
    "Player 1": {
        "Iniciativa": 0,
        "Pv": 100,
        "PE": 50,
        "Armadura": 22,
        "Esquiva": 25,
        "Bloqueio": 24,
        "Contra-Ataque": 30,
        "Sanidade": 100,
        "Habilidades": [],
        "Rituais": []
    }
}

# Variáveis globais para imagem
imagem_original = None
texture_tag = "bg_texture"

def carregar_imagem_original(path):
    global imagem_original
    imagem_original = Image.open(path).convert("RGBA")

def atualizar_textura(largura, altura):
    global imagem_original
    if imagem_original is None:
        return

    # Redimensiona a imagem para o tamanho atual da viewport
    imagem_redimensionada = imagem_original.resize((largura, altura), Image.LANCZOS)

    # Converte para lista de floats 0-1 (RGBA)
    data = list(imagem_redimensionada.getdata())
    data_float = []
    for pixel in data:
        r, g, b, a = pixel
        data_float.extend([r / 255.0, g / 255.0, b / 255.0, a / 255.0])

    if does_item_exist(texture_tag):
        set_value(texture_tag, data_float)
    else:
        with texture_registry(show=False):
            add_static_texture(largura, altura, data_float, tag=texture_tag)

def callback_redimensionar(sender, app_data):
    largura, altura = get_viewport_client_width(), get_viewport_client_height()
    atualizar_textura(largura, altura)
    
    if does_item_exist("janela_principal"):
        configure_item("janela_principal", width=largura, height=altura)
        set_item_pos("janela_principal", [0,0])
    if does_item_exist("imagem_fundo"):
        configure_item("imagem_fundo", width=largura, height=altura)

# Janela para editar ficha
def editar_ficha_callback():
    nomes = list(fichas.keys())
    if not nomes:
        add_text("Nenhuma ficha cadastrada!", color=[255, 0, 0])
        return
    
    def salvar():
        nome = get_value("combo_nome")
        for campo in campos_atuaveis:
            try:
                fichas[nome][campo] = int(get_value(f"input_{campo}"))
            except ValueError:
                fichas[nome][campo] = get_value(f"input_{campo}")
        delete_item("editar_ficha_window")
        print("Ficha atualizada:")
        print(json.dumps(fichas[nome], indent=2))

    campos_atuaveis = ["Iniciativa", "Pv", "PE", "Armadura", "Esquiva", "Bloqueio", "Contra-Ataque", "Sanidade"]

    with window(label="Editar Ficha", modal=True, tag="editar_ficha_window", width=400, height=450):
        add_text("Editar Ficha de Jogador")
        add_spacing(count=2)
        add_combo(nomes, default_value=nomes[0], tag="combo_nome")

        for campo in campos_atuaveis:
            add_input_int(label=campo, tag=f"input_{campo}")
        
        def atualizar_inputs(sender, app_data):
            nome = app_data
            ficha = fichas[nome]
            for campo in campos_atuaveis:
                set_value(f"input_{campo}", ficha.get(campo, 0))

        set_item_callback("combo_nome", atualizar_inputs)
        atualizar_inputs(None, nomes[0])

        add_spacing(count=1)
        add_button(label="Salvar", callback=salvar, width=100, pos=[150, 380])

# Janela para iniciar combate
def iniciar_combate_callback():
    if not fichas:
        add_text("Nenhuma ficha cadastrada!", color=[255, 0, 0])
        return
    
    ordem = sorted(fichas.items(), key=lambda x: x[1].get("Iniciativa", 0), reverse=True)
    campos_atuaveis = [k for k,v in fichas[next(iter(fichas))].items() if isinstance(v, (int, float))]
    
    def salvar_combate():
        for nome, dados in ordem:
            for campo in campos_atuaveis:
                tag = f"{nome}_{campo}"
                try:
                    fichas[nome][campo] = int(get_value(tag))
                except ValueError:
                    fichas[nome][campo] = get_value(tag)
        delete_item("combate_window")
        print("Fichas atualizadas:")
        print(json.dumps(fichas, indent=2))

    with window(label="Tela de Combate", tag="combate_window", width=1200, height=600):
        # Cabeçalhos
        add_text("Ordem", pos=[20, 10])
        add_text("Nome", pos=[70, 10])
        for i, campo in enumerate(campos_atuaveis):
            add_text(campo, pos=[200 + i * 110, 10])
        
        # Dados e inputs
        for idx, (nome, dados) in enumerate(ordem):
            y = 40 + idx * 30
            add_text(str(idx + 1), pos=[20, y])
            add_text(nome, pos=[70, y])
            for j, campo in enumerate(campos_atuaveis):
                tag = f"{nome}_{campo}"
                add_input_int(default_value=dados[campo], width=100, pos=[200 + j * 110, y], tag=tag)

        add_button(label="Encerrar Combate", callback=salvar_combate, pos=[20, 40 + len(ordem)*30 + 20])

# Função principal para criar a interface
def setup_interface():
    create_context()

    carregar_imagem_original(r"C:\Users\peogr\OneDrive\Documentos\VScode\Interface_rpg\fundo.jpeg")
    largura, altura = 1280, 800  # Tamanho inicial do viewport

    create_viewport(title="Painel RPG", width=largura, height=altura)
    setup_dearpygui()
    show_viewport()

    # Cria a textura inicial (redimensionada para o tamanho inicial)
    atualizar_textura(largura, altura)

    with window(label="Painel RPG", tag="janela_principal", no_move=True, no_resize=False, no_title_bar=True, no_collapse=True):
        # Vai posicionar a janela na origem e ajustar o tamanho para o viewport
        set_item_pos("janela_principal", [0,0])
        configure_item("janela_principal", width=largura, height=altura)
        
        add_image(texture_tag, tag="imagem_fundo", width=largura, height=altura)
        add_spacing(count=3)
        add_button(label="Iniciar Combate", callback=iniciar_combate_callback)
        add_button(label="Editar Ficha", callback=editar_ficha_callback)

    # Callback que escuta mudança no tamanho do viewport
    set_viewport_resize_callback(callback_redimensionar)

    start_dearpygui()
    destroy_context()

if __name__ == "__main__":
    setup_interface()

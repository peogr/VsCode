from dearpygui.dearpygui import *
import fichas_io
import json

# Função principal que cria a janela para edição de fichas
def editar_ficha_callback():
    # Carrega todas as fichas do arquivo
    fichas = fichas_io.carregar_fichas()
    nomes = list(fichas.keys())  # Lista de nomes das fichas disponíveis

    # Se não houver fichas, mostra mensagem e encerra
    if not nomes:
        with window(label="Editar fichas", width=400, height=300):
            add_text("Nenhuma ficha cadastrada!", color=[255, 0, 0])
        return

    habilidades_temp = []  # Lista temporária para armazenar habilidades durante edição

    # Atualiza a lista de habilidades exibida na interface
    def atualizar_lista_habilidades():
        # Remove todos os elementos filhos do container "lista_habilidades"
        delete_item("lista_habilidades", children_only=True)
        for i, hab in enumerate(habilidades_temp):
            # Cria um grupo horizontal para cada habilidade (texto + botão de excluir)
            with group(parent="lista_habilidades", horizontal=True):
                add_text(f"{i+1}. {hab}", wrap=350)  # Mostra a habilidade numerada

                # Botão para excluir a habilidade (passando o índice correto)
                add_button(
                    label="Excluir",
                    width=60,
                    callback=lambda s, a, idx=i: excluir_habilidade(idx)
                )

    # Remove habilidade pelo índice e atualiza a interface
    def excluir_habilidade(idx):
        habilidades_temp.pop(idx)
        atualizar_lista_habilidades()

    # Adiciona habilidade da caixa de texto para a lista temporária
    def adicionar_habilidade(sender, app_data):
        hab = get_value("input_nova_habilidade").strip()
        if hab:
            habilidades_temp.append(hab)
            set_value("input_nova_habilidade", "")
            atualizar_lista_habilidades()

    # Salva as alterações feitas na ficha selecionada
    def salvar():
        nome = get_value("combo_nome")
        # Tenta salvar PV e PM (como inteiros)
        try:
            fichas[nome]["Pv"] = int(get_value("input_pv"))
            fichas[nome]["PE"] = int(get_value("input_pm"))
        except ValueError:
            print("PV e PM devem ser números inteiros.")
            return

        # Salva habilidades
        fichas[nome]["Habilidades"] = habilidades_temp.copy()
        fichas_io.salvar_fichas(fichas)

        # Fecha a janela
        delete_item("editar_ficha_window")
        print(f"Ficha '{nome}' atualizada com sucesso:")
        print(json.dumps(fichas[nome], indent=2, ensure_ascii=False))
        delete_item("Editar Ficha")

    # Atualiza campos de PV, PM e habilidades ao trocar o nome no combo
    def atualizar_inputs(sender, app_data):
        nonlocal habilidades_temp
        nome = app_data
        ficha = fichas[nome]
        set_value("input_pv", ficha.get("Pv", 0))
        set_value("input_pm", ficha.get("PE", 0))
        habilidades_temp = ficha.get("Habilidades", []).copy()
        atualizar_lista_habilidades()
    
    # Apaga a ficha selecionada
    def apagar_ficha():
        nome = get_value("combo_nome")
        if nome in fichas:
            del fichas[nome]
            fichas_io.salvar_fichas(fichas)

            novos_nomes = list(fichas.keys())  
            if novos_nomes:
                # Atualiza o combo com o novo nome padrão
                configure_item("combo_nome", items=novos_nomes, default_value=novos_nomes[0])  
                atualizar_inputs(None, novos_nomes[0])  
            else:
                # Fecha a janela se não houver mais fichas
                delete_item("editar_ficha_window")  
    
    # Cria a janela de edição
    with window(label="Editar Ficha", modal=True, tag="editar_ficha_window", width=700, height=1050):
        add_text("Editar Ficha de Jogador")
        add_spacing(count=2)

        # Combo para selecionar ficha
        add_combo(nomes, default_value=nomes[0], tag="combo_nome")

        # Campos de PV e PM
        add_input_int(label="PV", tag="input_pv")
        add_input_int(label="PM", tag="input_pm")

        add_separator()
        add_text("Habilidades")

        # Campo para nova habilidade
        add_input_text(label="Nova Habilidade", tag="input_nova_habilidade", multiline=True, width=400, height=80)
        add_button(label="Adicionar Habilidade", callback=adicionar_habilidade)
        
        # Janela que lista habilidades
        with child_window(tag="lista_habilidades", width=400, height=150):
            pass

        # Callback quando muda o nome selecionado
        set_item_callback("combo_nome", atualizar_inputs)
        atualizar_inputs(None, nomes[0])  # Carrega primeira ficha ao abrir

        add_spacing(count=1)
        add_button(label="Salvar", callback=salvar, width=100, pos=[175, 400])

        add_spacing(count=3)
        add_separator()
        add_button(label="apagar ficha", callback=apagar_ficha)

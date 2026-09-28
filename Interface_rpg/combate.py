from dearpygui.dearpygui import *
import fichas_io
import json

def combat_window(sender, app_data):
    fichas = fichas_io.carregar_fichas()

    with window(label="combat control", width=500, height=500):
        if not fichas:
            add_text("Nenhuma ficha cadastrada!", color=[255, 0, 0])
            return

        ordem = sorted(fichas.items(), key=lambda x: x[1].get("Iniciativa", 0), reverse=True)
        campos_atuaveis = [k for k, v in fichas[next(iter(fichas))].items() if isinstance(v, (int, float))]

        def salvar_combate():
            for nome, dados in ordem:
                for campo in campos_atuaveis:
                    tag = f"{nome}_{campo}"
                    try:
                        fichas[nome][campo] = int(get_value(tag))
                    except ValueError:
                        fichas[nome][campo] = get_value(tag)
            fichas_io.salvar_fichas(fichas)  # Salva no arquivo .json
            delete_item("combate_window")
            

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

            add_button(label="Encerrar Combate", callback=salvar_combate, pos=[20, 40 + len(ordem) * 30 + 20])

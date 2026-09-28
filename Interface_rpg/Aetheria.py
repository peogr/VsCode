from dearpygui import dearpygui as dpg
import combate
import Edit
import criar  # importa seu módulo de criação de fichas

dpg.create_context()

with dpg.window(label="Aetheria", width=1050, height=1000):
    dpg.add_text("Bem vindo a Aetheria")

    dpg.add_button(label="Iniciar combate", callback=combate.combat_window)
    dpg.add_button(label="Adicionar ficha", callback=lambda: criar.criar_ficha())
    dpg.add_button(label="Editar ficha", callback = Edit.editar_ficha_callback)
dpg.create_viewport(title="Inicio", width=1050, height=1000)

dpg.setup_dearpygui()
dpg.show_viewport()

dpg.start_dearpygui()

dpg.destroy_context()

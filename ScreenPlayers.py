import tkinter as tk
from tkinter import messagebox, simpledialog
from settings.ManagementPlayers import ManagementPlayers

class InterfaceJogadores:
    def __init__(self, root):
        self.root = root
        self.root.title("Airsoft Game Master - Gestão Jogadores")
        self.root.geometry("400x400")
        self.root.resizable(False, False)

        # Criar a barra de menus
        self.barra_menus = tk.Menu(self.root)
        self.root.config(menu=self.barra_menus)

        # Adicionar opções ao menu "Menu"
        self.menu_opcoes = tk.Menu(self.barra_menus, tearoff=0)
        self.barra_menus.add_cascade(label="Menu", menu=self.menu_opcoes)

        # Adicionar submenu "Jogador" ao menu "Menu"
        self.submenu_jogador = tk.Menu(self.menu_opcoes, tearoff=0)
        self.submenu_jogador.add_command(label="Adicionar", command=self.adicionar_jogador)
        self.submenu_jogador.add_command(label="Editar", command=self.editar_jogador, state=tk.DISABLED)
        self.submenu_jogador.add_command(label="Excluir", command=self.excluir_jogador, state=tk.DISABLED)
        self.menu_opcoes.add_cascade(label="Jogador", menu=self.submenu_jogador)

        # Adicionar opção "Voltar" ao menu "Menu"
        self.menu_opcoes.add_command(label="Voltar", command=self.sair)

        # Criar a janela principal para gerenciar jogadores
        self.management_players = ManagementPlayers(self.root, self.editar_jogador, self.excluir_jogador, self.habilitar_opcoes)

    def adicionar_jogador(self):
        self.management_players.adicionar_jogador()

    def editar_jogador(self):
        self.management_players.editar_jogador()

    def excluir_jogador(self):
        self.management_players.excluir_jogador()

    def sair(self):
        self.root.destroy()

    def habilitar_opcoes(self):
        self.submenu_jogador.entryconfig("Editar", state=tk.NORMAL)
        self.submenu_jogador.entryconfig("Excluir", state=tk.NORMAL)



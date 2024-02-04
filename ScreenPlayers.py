import tkinter as tk
from tkinter import messagebox, simpledialog
import sqlite3
import os

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
        self.submenu_jogador.add_command(label="Editar", command=self.editar_jogador, state="disabled")
        self.submenu_jogador.add_command(label="Excluir", command=self.excluir_jogador, state="disabled")
        self.menu_opcoes.add_cascade(label="Jogador", menu=self.submenu_jogador)

        # Adicionar opção "Voltar" ao menu "Menu"
        self.menu_opcoes.add_command(label="Voltar", command=self.sair)

        # Variável para armazenar jogadores
        self.jogadores = []

        # Label no centro da interface
        self.label_titulo = tk.Label(self.root, text="Jogadores", font=("Helvetica", 16))
        self.label_titulo.pack(pady=10)

        # Listbox para exibir jogadores
        self.listbox_jogadores = tk.Listbox(self.root, width=40, height=10)
        self.listbox_jogadores.pack(pady=20)

        # Adicionar evento de clique na listagem
        self.listbox_jogadores.bind("<ButtonRelease-1>", self.selecionar_jogador)

        # Exibir jogadores já cadastrados ao iniciar a interface
        self.atualizar_lista_jogadores()

    def atualizar_lista_jogadores(self):
        self.listbox_jogadores.delete(0, tk.END)  # Limpar a lista atual
        self.jogadores.clear()

        # Verificar se o banco de dados existe
        if not os.path.exists("settings/players.db"):
            return

        conn = sqlite3.connect("settings/players.db")
        cursor = conn.cursor()
        cursor.execute("CREATE TABLE IF NOT EXISTS jogadores (id INTEGER PRIMARY KEY AUTOINCREMENT, nome TEXT NOT NULL)")
        cursor.execute("SELECT * FROM jogadores")
        jogadores = cursor.fetchall()

        for jogador in jogadores:
            self.jogadores.append(jogador[1])
            self.listbox_jogadores.insert(tk.END, jogador[1])

        conn.close()

    def adicionar_jogador(self):
        nome_jogador = simpledialog.askstring("Adicionar Jogador", "Digite o nome do jogador:")
        if nome_jogador:
            conn = sqlite3.connect("settings/players.db")
            cursor = conn.cursor()
            cursor.execute("INSERT INTO jogadores (nome) VALUES (?)", (nome_jogador,))
            conn.commit()
            conn.close()

            # Atualizar a lista de jogadores
            self.atualizar_lista_jogadores()

    def editar_jogador(self):
        selected_index = self.listbox_jogadores.curselection()
        if selected_index:
            jogador_atual = self.jogadores[selected_index[0]]
            novo_nome = simpledialog.askstring("Editar Jogador", f"Novo nome para {jogador_atual}:")
            if novo_nome:
                conn = sqlite3.connect("settings/players.db")
                cursor = conn.cursor()
                cursor.execute("UPDATE jogadores SET nome = ? WHERE nome = ?", (novo_nome, jogador_atual))
                conn.commit()
                conn.close()

                # Atualizar a lista de jogadores
                self.atualizar_lista_jogadores()

    def excluir_jogador(self):
        selected_index = self.listbox_jogadores.curselection()
        if selected_index:
            jogador_atual = self.jogadores[selected_index[0]]
            opcao = messagebox.askokcancel("Remover Jogador", f"Tem certeza que deseja remover o jogador {jogador_atual}?")
            if opcao:
                conn = sqlite3.connect("settings/players.db")
                cursor = conn.cursor()
                cursor.execute("DELETE FROM jogadores WHERE nome = ?", (jogador_atual,))
                conn.commit()
                conn.close()

                # Atualizar a lista de jogadores
                self.atualizar_lista_jogadores()

    def selecionar_jogador(self, event):
        selected_index = self.listbox_jogadores.curselection()
        if selected_index:
            self.submenu_jogador.entryconfig("Editar", state="normal")
            self.submenu_jogador.entryconfig("Excluir", state="normal")

    def sair(self):
        self.root.destroy()


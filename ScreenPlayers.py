import tkinter as tk
from tkinter import messagebox, simpledialog
import sqlite3
from tkinter import ttk

class InterfaceJogadores:
    def __init__(self, root):
        self.root = root
        self.root.title("Airsoft Game Master - Gestão Jogadores")
        self.root.geometry("800x600")
        self.root.resizable(False, False)

        # Criar o menu
        self.menu_principal = tk.Menu(self.root)
        self.root.config(menu=self.menu_principal)

        # Adicionar opções ao menu
        self.menu_opcoes = tk.Menu(self.menu_principal, tearoff=0)
        self.menu_principal.add_cascade(label="Menu", menu=self.menu_opcoes)

        # Adicionar submenu "Definições" ao menu "Menu"
        self.submenu_definicoes = tk.Menu(self.menu_opcoes, tearoff=0)
        self.submenu_definicoes.add_command(label="Jogadores", command=self.abrir_jogadores)
        self.menu_opcoes.add_cascade(label="Adicionar", menu=self.submenu_definicoes)

        self.menu_opcoes.add_separator()
        self.menu_opcoes.add_command(label="Voltar", command=self.sair)

        # Adicionar a tabela de jogadores
        self.tree_jogadores = ttk.Treeview(self.root, columns=("Nome", ""))
        self.tree_jogadores.heading("#0", text="ID")
        self.tree_jogadores.heading("#1", text="Nome")
        self.tree_jogadores.pack(pady=10)

    def abrir_jogadores(self):
        messagebox.showinfo("Airsoft Game Master - Gestão Jogadores", "Aqui você pode gerenciar jogadores.")

        # Chamar a função para adicionar jogadores
        self.registrar_novo_jogador()

        # Atualizar a tabela de jogadores
        self.atualizar_tabela_jogadores()

    def sair(self):
        self.root.destroy()

    def registrar_novo_jogador(self):
        # Função para adicionar um novo jogador ao banco de dados SQLite
        nome_jogador = simpledialog.askstring("Novo Jogador", "Digite o nome do novo jogador:")
        if nome_jogador:
            # Conectar ao banco de dados
            conn = sqlite3.connect("jogadores.db")
            cursor = conn.cursor()

            # Criar a tabela se não existir
            cursor.execute('''
                CREATE TABLE IF NOT EXISTS jogadores (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    nome TEXT NOT NULL
                )
            ''')

            # Inserir novo jogador
            cursor.execute("INSERT INTO jogadores (nome) VALUES (?)", (nome_jogador,))
            
            # Commit e fechar a conexão
            conn.commit()
            conn.close()

    def atualizar_tabela_jogadores(self):
        # Função para atualizar a tabela de jogadores
        # Limpar itens existentes na tabela
        for item in self.tree_jogadores.get_children():
            self.tree_jogadores.delete(item)

        # Conectar ao banco de dados
        conn = sqlite3.connect("jogadores.db")
        cursor = conn.cursor()

        # Recuperar jogadores do banco de dados
        cursor.execute("SELECT * FROM jogadores")
        jogadores = cursor.fetchall()

        # Inserir jogadores na tabela
        for jogador in jogadores:
            self.tree_jogadores.insert("", "end", values=jogador)

        # Fechar a conexão
        conn.close()



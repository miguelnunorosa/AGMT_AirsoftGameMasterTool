import tkinter as tk
from tkinter import messagebox, simpledialog
import sqlite3

class InterfaceJogadores:
    def __init__(self, root):
        self.root = root
        self.root.title("Airsoft Game Master - Gestão Jogadores")
        self.root.geometry("400x400")
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

        # Variáveis para entrada de dados
        self.nome_var = tk.StringVar()

        # Frame para entrada de dados
        self.frame_entrada = tk.Frame(self.root)
        self.frame_entrada.pack(pady=20)

        tk.Label(self.frame_entrada, text="Nome do Jogador:").grid(row=0, column=0, padx=10, pady=10)
        entry_nome = tk.Entry(self.frame_entrada, textvariable=self.nome_var)
        entry_nome.grid(row=0, column=1, padx=10, pady=10)

        # Botão para adicionar jogador
        tk.Button(self.frame_entrada, text="Adicionar Jogador", command=self.registrar_novo_jogador).grid(row=1, column=0, columnspan=2, pady=10)

        # Widget Listbox para exibir jogadores
        self.listbox_jogadores = tk.Listbox(self.root, width=40, height=10)
        self.listbox_jogadores.pack(pady=20)

        # Adicionar evento de clique na listagem
        self.listbox_jogadores.bind("<ButtonRelease-1>", self.selecionar_jogador)

        # Exibir jogadores já cadastrados ao iniciar a interface
        self.atualizar_lista_jogadores()

    def abrir_jogadores(self):
        messagebox.showinfo("Airsoft Game Master - Gestão Jogadores", "Aqui você pode gerenciar jogadores.")

    def sair(self):
        self.root.destroy()

    def registrar_novo_jogador(self):
        nome_jogador = self.nome_var.get()

        if nome_jogador:
            conn = sqlite3.connect("jogadores.db")
            cursor = conn.cursor()
            cursor.execute('''
                CREATE TABLE IF NOT EXISTS jogadores (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    nome TEXT NOT NULL
                )
            ''')
            cursor.execute("INSERT INTO jogadores (nome) VALUES (?)", (nome_jogador,))
            conn.commit()
            conn.close()

            # Limpar a entrada após adicionar jogador
            self.nome_var.set("")

            # Atualizar a lista de jogadores
            self.atualizar_lista_jogadores()

    def atualizar_lista_jogadores(self):
        self.listbox_jogadores.delete(0, tk.END)  # Limpar a lista atual

        conn = sqlite3.connect("jogadores.db")
        cursor = conn.cursor()
        cursor.execute("SELECT nome FROM jogadores")
        jogadores = cursor.fetchall()

        for jogador in jogadores:
            self.listbox_jogadores.insert(tk.END, jogador[0])

        conn.close()

    def selecionar_jogador(self, event):
        selected_index = self.listbox_jogadores.curselection()
        if selected_index:
            nome_jogador = self.listbox_jogadores.get(selected_index)
            #messagebox.showinfo("Selecionar Jogador", f"Você selecionou o jogador: {nome_jogador}")


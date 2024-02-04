import tkinter as tk
from tkinter import messagebox, simpledialog
import sqlite3
import os

class ManagementPlayers:
    def __init__(self, root, editar_callback, excluir_callback, habilitar_opcoes_callback):
        self.root = root

        # Variáveis para armazenar jogadores e índices selecionados
        self.jogadores = []
        self.indices_selecionados = []

        # Callbacks
        self.editar_callback = editar_callback
        self.excluir_callback = excluir_callback
        self.habilitar_opcoes_callback = habilitar_opcoes_callback

        # Label no centro da interface
        self.label_titulo = tk.Label(self.root, text="Jogadores", font=("Helvetica", 16))
        self.label_titulo.pack(pady=10)

        # Listbox para exibir jogadores sem barra de rolagem
        self.listbox_jogadores = tk.Listbox(self.root, width=40, height=10, selectmode=tk.MULTIPLE)
        self.listbox_jogadores.pack(pady=20)

        # Adicionar evento de clique na listagem
        self.listbox_jogadores.bind("<ButtonRelease-1>", self.selecionar_jogador)

        # Exibir jogadores já cadastrados ao iniciar a interface
        self.atualizar_lista_jogadores()

        # Botão "Gerar Equipas"
        self.botao_gerar_equipas = tk.Button(self.root, text="Gerar Equipas", command=self.gerar_equipas)
        self.botao_gerar_equipas.pack(pady=10)

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

    def habilitar_opcoes(self):
        if self.habilitar_opcoes_callback:
            self.habilitar_opcoes_callback()

    def selecionar_jogador(self, event):
        # Limpar índices selecionados anteriores
        self.indices_selecionados = []

        selected_indices = self.listbox_jogadores.curselection()
        for index in selected_indices:
            self.indices_selecionados.append(index)

        # Atualizar opções com base nos itens selecionados
        if len(self.indices_selecionados) == 1:
            self.habilitar_opcoes()
        else:
            self.desabilitar_opcoes()

    def desabilitar_opcoes(self):
        if self.habilitar_opcoes_callback:
            self.habilitar_opcoes_callback(desativar=True)

    def editar_jogador(self):
        if self.editar_callback and len(self.indices_selecionados) == 1:
            nome_jogador = self.jogadores[self.indices_selecionados[0]]
            novo_nome = simpledialog.askstring("Editar Jogador", f"Novo nome para {nome_jogador}:")
            if novo_nome:
                conn = sqlite3.connect("settings/players.db")
                cursor = conn.cursor()
                cursor.execute("UPDATE jogadores SET nome = ? WHERE nome = ?", (novo_nome, nome_jogador))
                conn.commit()
                conn.close()

                # Atualizar a lista de jogadores
                self.atualizar_lista_jogadores()

    def excluir_jogador(self):
        if self.excluir_callback and len(self.indices_selecionados) > 0:
            jogadores_selecionados = [self.jogadores[index] for index in self.indices_selecionados]
            opcao = messagebox.askokcancel("Remover Jogador", f"Tem certeza que deseja remover os jogadores {', '.join(jogadores_selecionados)}?")
            if opcao:
                conn = sqlite3.connect("settings/players.db")
                cursor = conn.cursor()
                for jogador in jogadores_selecionados:
                    cursor.execute("DELETE FROM jogadores WHERE nome = ?", (jogador,))
                conn.commit()
                conn.close()

                # Atualizar a lista de jogadores
                self.atualizar_lista_jogadores()

    def gerar_equipas(self):
        jogadores_selecionados = [self.jogadores[index] for index in self.indices_selecionados]

        if len(jogadores_selecionados) % 2 != 0:
            jogadores_selecionados.insert(0, "")

        meio = len(jogadores_selecionados) // 2
        equipe1 = jogadores_selecionados[:meio]
        equipe2 = jogadores_selecionados[meio:]

        mensagem = f"Equipa 1: \n{', '.join(equipe1)}\n\nEquipa 2: \n{', '.join(equipe2)}"
        messagebox.showinfo("Equipas Geradas", mensagem)



import tkinter as tk
from tkinter import messagebox

class Main:
    def __init__(self, root):
        self.root = root
        self.root.title("Airsoft Game Master")
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
        self.menu_opcoes.add_cascade(label="Definições", menu=self.submenu_definicoes)

        self.menu_opcoes.add_separator()
        self.menu_opcoes.add_command(label="Sair", command=self.sair)

    def abrir_jogadores(self):
        messagebox.showinfo("Jogadores", "Aqui você pode gerenciar jogadores.")

    def sair(self):
        if messagebox.askokcancel("Sair", "Tem certeza que deseja sair?"):
            self.root.destroy()

# Criar a janela principal
root = tk.Tk()
app = Main(root)
root.mainloop()

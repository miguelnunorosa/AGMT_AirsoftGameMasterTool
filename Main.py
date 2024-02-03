import tkinter as tk
from tkinter import messagebox

class InterfaceGrafica:
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
        self.menu_opcoes.add_command(label="Definições", command=self.abrir_definicoes)
        self.menu_opcoes.add_separator()
        self.menu_opcoes.add_command(label="Sair", command=self.sair)
        self.menu_principal.add_cascade(label="Menu", menu=self.menu_opcoes)

    def abrir_definicoes(self):
        messagebox.showinfo("Definições", "Aqui você pode configurar suas definições.")

    def sair(self):
        if messagebox.askokcancel("Sair", "Tem certeza que deseja sair?"):
            self.root.destroy()

# Criar a janela principal
root = tk.Tk()
app = InterfaceGrafica(root)
root.mainloop()

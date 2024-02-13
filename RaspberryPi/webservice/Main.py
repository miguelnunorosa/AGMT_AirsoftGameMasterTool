from flask import Flask, render_template
import sqlite3



app = Flask(__name__, template_folder='settings/templates')
appTitle = 'Airsoft Game Master Tool - Results'
dbPath = 'settings/database/agmtdatabase.db'




@app.route('/jogadores') # Rota para a página de jogadores
def jogadores():
    conn = sqlite3.connect(dbPath)                                # Conectar ao banco de dados
    cursor = conn.cursor()
    cursor.execute("SELECT * FROM players")                       # Executar a query para obter os jogadores
    jogadores = cursor.fetchall()                                 # Obter todos os jogadores
    conn.close()                                                  # Fechar a conexão com o banco de dados
    return render_template('jogadores.html', jogadores=jogadores) # Renderizar o template com os dados dos jogadores


@app.route('/')
def index():
    return render_template('index.html', title=appTitle)


if __name__ == '__main__':
    app.run(host="0.0.0.0", port=int("3000"), debug=True)
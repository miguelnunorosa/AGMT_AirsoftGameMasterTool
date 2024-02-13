from flask import Flask, render_template
import Players
import sqlite3



app = Flask(__name__, template_folder='settings/templates')
appTitle = 'Airsoft Game Master Tool - Results'
dbPath = 'settings/database/agmtdatabase.db'





@app.route('/')
def index():
    return render_template('index.html', title=appTitle)



@app.route('/jogadores')
def jogadores():
    # Obtém a lista de jogadores do banco de dados
    jogadores = Players.get_players()
    return render_template('jogadores.html', jogadores=jogadores)


if __name__ == '__main__':
    app.run(host="0.0.0.0", port=int("3000"), debug=True)
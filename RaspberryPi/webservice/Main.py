from flask import Flask, redirect, render_template, request, url_for
import Players

app = Flask(__name__, template_folder='settings/templates')
appTitle = 'Airsoft Game Master Tool - Results'




@app.route('/')
def index():
    Players.create_table()
    return render_template('index.html', title=appTitle)



@app.route('/jogadores')
def jogadores():
    jogadores = Players.get_players()
    return render_template('jogadores.html', jogadores=jogadores)

@app.route('/jogadores', methods=['POST'])
def adicionar_jogador():
    nome = request.form['nome']
    ativo = request.form.get('ativo', 0)
    Players.insert_players(nome, ativo)
    return redirect(url_for('jogadores'))

@app.route('/jogadores')
def jogadores_equipas_modal():
    jogadores_ativos = Players.get_active_players()
    return render_template('jogadores.html', jogadores_ativos=jogadores_ativos)




if __name__ == '__main__':
    app.run(host="0.0.0.0", port=int("3000"), debug=True)

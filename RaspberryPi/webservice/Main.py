from flask import Flask, jsonify, redirect, render_template, request, url_for
import Players

app = Flask(__name__, template_folder='settings/templates')
appTitle = 'Airsoft Game Master Tool - Results'




@app.route('/')
def index():
    Players.create_table()
    return render_template('index.html', title=appTitle)

@app.route('/sobre')
def sobre():
    return render_template('sobre.html', title=appTitle)



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

@app.route('/detalhes_jogador/<int:jogador_id>')
def detalhes_jogador(jogador_id):
    jogador = Players.get_player_by_id(jogador_id)
    detalhes = {
        'name': jogador[1],
        'isActive': bool(jogador[2])
    }
    return jsonify(detalhes)

# Rota para atualizar um jogador pelo ID
@app.route('/atualizar_jogador/<int:jogador_id>', methods=['POST'])
def atualizar_jogador(jogador_id):
    # Obter os dados do formulário
    nome = request.form['nome']
    ativo = request.form.get('ativo', 0)

    Players.update_players(nome, ativo, jogador_id)
    return redirect(url_for('jogadores'))


@app.route('/jogadores')
def jogadores_equipas_modal():
    jogadores_ativos = Players.get_active_players()
    return render_template('jogadores.html', jogadores_ativos=jogadores_ativos)




if __name__ == '__main__':
    app.run(host="0.0.0.0", port=int("3000"), debug=True)

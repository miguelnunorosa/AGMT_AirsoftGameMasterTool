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

@app.route('/atualizar_ativo/<int:jogador_id>', methods=['POST'])
def atualizar_ativo(jogador_id):
    ativo = request.form['ativo']
    
    # Aqui você pode chamar a função para atualizar o estado "ativo" do jogador no banco de dados
    Players.update_player_ativo(jogador_id, ativo)
    
    # Retornar uma resposta de sucesso para o cliente
    return '', 200

@app.route('/gerar_equipas')
def gerar_equipas():
    jogadores_ativos = Players.get_active_players()
    # Divida os jogadores ativos em duas equipes
    metade = len(jogadores_ativos) // 2
    equipe_azul = jogadores_ativos[:metade]
    equipe_vermelha = jogadores_ativos[metade:]
    print(equipe_azul)
    return render_template('jogadores.html', equipe_azul=equipe_azul, equipe_vermelha=equipe_vermelha)



if __name__ == '__main__':
    app.run(host="0.0.0.0", port=int("3000"), debug=True)

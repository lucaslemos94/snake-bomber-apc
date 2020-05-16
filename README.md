# snake-bomber-apc
Um jogo implementado na linguagem de programação C, durante o  curso de Computação, na disciplina Algoritmos e Programação de Computadores (APC). Universidade de Brasília - UnB.

# Objetivo
O objetivo do jogo é conseguir pegar todos os pontos no mapa sem ser morto pelos inimigos!

# Tipos de inimigos
**Inimigo X** - Inimigo agressivo, pode te matar se você o tocar! às vezes ele te segue, às vezes anda aleatoriamente!

**Inimigo B** - Inimigo passivo, anda aleatoriamente deixando um rastro! apesar de não te matar, pode te deixar preso!

**Inimigo Q** - Inimigo bomba, não tem hora certa para aparecer no mapa e nem para explodir tudo!

**Inimigo O** - Inimigo ponto, há diversos espalhados pelo mapa! pegue todos eles e ganhe o jogo!

# Dificuldades de jogo
    
**Fácil** - O inimigo X anda somente aletório no mapa, sempre podendo matar o personagem nesse movimento;

**Médio** - O inimigo X anda na proporção de 50/50 seguindo/aletório, respectivamente, sempre podendo matar em ambos os movimentos;

**Difícil** - O inimigo X anda na proporção 70/30 seguindo/aletório, respectivamente, sempre podendo matar em ambos os movimentos;

Por padrão, as partidas são jogadas na dificuldade médio.  **São 10 inimigos ponto, 7 inimigos x e 3 inimigos b no mapa em qualquer dificuldade**, sendo possível personalizar a quantidade ao gosto do jogador no menu de configurações.

# Modo rankeado
O modo rankeado fica ativo apenas quando for solicitado pelo jogador no menu, onde o jogo é jogado no difícil e com mais inimigos no mapa. Apenas partidas rankeadas são gravadas no ranking. Mesmo ao iniciar um novo jogo que está com as configurações rankeadas gravadas anteriormente, a partida não conta como rankeada.

O ranking mostra além do nome e pontos do jogador, as suas jogadas, que é o critério de desempate. Caso o novo jogador que tente entrar no ranking empate com o último jogador do mesmo e empate também nas jogadas, o novo jogador não entra no ranking.

# Funcionalidades Gerais
Ao iniciar o jogo, é gerado dois arquivos: ranking.bin e config.txt, que correspondem ao ranking e as configurações de jogo, respectivamente. Caso os arquivos sejam apagados, o programa sempre gera ambos novamente, para seu funcionamento

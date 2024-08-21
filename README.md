# INF01047-TrabalhoFinal

Repositório para o trabalho final da cadeira INF01047 - Fundamentos de Computação Gráfica.

Alunos:

- Guilherme Ismael Flach - 00342108
- Vinícius Daniel Spadotto - 00341554

# Relatório

Durante o começo do desenvolvimento, priorizou-se criar uma base sólida para a construção do programa, simulando features de uma Engine. Começou-se um sistema de GameObjects que permitisse o aninhamento de objetos em cadeia (baseados em mudanças de base), semelhante ao conceito de transformações sucessivas. Ligou-se junto a este, um sistema de renderização baseado em modelos, permitindo que vários GameObjects compartilhem o mesmo modelo 3D para renderização.

Após isso, foram desenvolvidos sistemas adjacentes, como o gerenciamento das câmeras virtuais (e o sistema de Dolly Cameras, utilizando curvas de Bézier), além da movimentação básica do jogador.

Uma vez com mais base para os modelos de iluminação, foram desenvolvidas as RenderQueues, permitindo com que sejam usados shaders arbitrários para cada um dos objetos da cena (ex: Gouraud Shading para os planetas e Phong Shading para a nave). Em conjunto com isso, também foram incluidas as texturas para os modelos. Para o mapeamento de texturas, optou-se por gerar os objetos no Blender e fazer o UV Unwrapping (ainda que tenham sido só esferas, era uma ideia interessante caso fossem usados modelos mais complexos).

Nesse ponto, com a base bem construída, que foram realmente desenvolvidas as funcionalidades do projeto, o sistema de física, leitura dos planetas de um arquivo, detecção e handling de colisões e por fim o sistema de spawn dinâmico de planetas.

Ter construído essa fundação sólida foi uma escolha que definitivamente se pagou a longo prazo, uma vez que facilitou drásticamente que novas funcionalidades fossem implementadas (como a inclusão dos modelos de iluminação e leitura de modelos 3D de arquivos .obj). Apesar de ter sido demorado para se ter algo concreto, vale a dor de cabeça economizada em comparação a soluções mais imediatistas.

## Imagens:

![Demonstração 1](https://github.com/Guilherme-Flach/INF01047-TrabalhoFinal/blob/main/Demo1.png?raw=true)
![Demonstração 2](https://github.com/Guilherme-Flach/INF01047-TrabalhoFinal/blob/main/Demo2.png?raw=true)

## Contribuições

Guilherme Flach:

- Rendering
- DollyCamera
- Movimentação / Física
- Lógica de controle e criação de planetas

Vínicius Spadotto:

- Colisões
- Sistema de GameObjects / Troca de Bases
- Testes

## Controles:

**Botão Esquerdo do Mouse** - Cria um planeta com as configurações atuais (no cursor do mouse, em modo panorâmico, ou na direção que estiver olhando, em modo nave)

**Scroll do Mouse** - Aumenta ou diminui o tamanho e massa do planeta a ser criado

**1/2/3** - Aumenta a velocidade do planeta a ser criado nos eixos X/Y/Z

**ALT + 1/2/3** - Diminui a velocidade do planeta a ser criado nos eixos X/Y/Z

**TAB** - Alterna entre modos nave e panorâmico

**Movimento do Mouse** - Controle de Câmera (em modo nave)

**WASD** - Movimentação (no sentido da nave)

**WASD + SHIFT** - Movimentação com turbo (no sentido da nave)

**Setas** - Rotacionam a orientação da nave

**E/Q** - Rotacionam a nave no sentido horário/anti-horário

## Compilação

### Linux

Para compilar e executar este projeto no Linux, primeiro instalemos
as bibliotecas necessárias. Para tanto, execute o comando abaixo em um terminal.
Esse é normalmente suficiente em uma instalação de Linux Ubuntu:

    sudo apt-get install build-essential make libx11-dev libxrandr-dev \
                         libxinerama-dev libxcursor-dev libxcb1-dev libxext-dev \
                         libxrender-dev libxfixes-dev libxau-dev libxdmcp-dev

No caso do Linux Mint, talvez seja necessário instalar mais algumas bibliotecas:

    sudo apt-get install libmesa-dev libxxf86vm-dev

Finalmente, para compilar, basta executar, na raiz do projeto

    make && make run

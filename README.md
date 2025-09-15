# Simulação do Sistema Solar de Outer Wilds

Este projeto tem o intuito de recriar parte do sistema solar do jogo outer wilds e as mecânicas de cada planeta.
  
![Solar System](outerwilds-solar-system.webp)

#

O sistema será composto por:

### Sol 
Fonte de luz, e, no fim da animação deve explodir, marcando o início de uma nova animação.

### Recanto lenhoso
Similar a Terra do nosso sistema, sem nenhuma mecânica em especial.

### Vale da incerteza
Interno a ele, existe o buraco negro. Ao longo do tempo, se despedaça, e seus pedaços são teleportados para outra região do mapa (região essa em torno do buraco branco).

### Profundezas do gigante
Planeta gasoso contendo um globo de água, e vários tornados. 

### Abrolho sinistro
De longe, parece pequeno, mas interno a ele, existe uma região infinita, que só dá para sair indo para as regiões corretas.

### Lua Quântica
Orbita um planeta, porém se sair do campo de visão do jogador, teleporta e começa a orbitar outro planeta.

### O Xereta
Cometa congelado que derrete ao chegar perto do Sol.

#

## Uso

* Observação: aparentemente, compilar não funciona diretamente, por isso recomendo dar um make clean antes.

Compilar:
```
make
```

Executar:

```
./main
```

Ou, para executar make clean, make e ./main juntos, use:
```
./run.sh
```
#

## Dependências
Caso não tenha o GLUT instalado:
```
sudo apt-get install freeglut3-dev
```
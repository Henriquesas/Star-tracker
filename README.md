# Star-tracker

## Descrição do Projeto

Este projeto tem como objetivo desenvolver um rastreador de astros para o estudo de sistemas embarcados, utilizando a integração de diversos sensores, atuadores e microcontroladores. O equipamento será capaz de rastrear a posição de astros no céu e realizar cálculos que permitem controlar a orientação de um laser apontador para o rastreio do corpo celeste.

## Da construção do equipamento

A construção de um rastreador de estrelas com sistema embarcado oferece várias vantagens e oportunidades para a exploração astronômica. A integração de sensores como IMU (Unidade de Medição Inercial), GPS e encoders possibilita uma precisão elevada no rastreio dos astros, permitindo um estudo mais detalhado e acurado das posições celestes.

A utilização de atuadores, como motores que movimentam a plataforma, torna possível ajustar a direção do rastreamento inicial e acompanhar o movimento aparente das estrelas no céu, garantindo uma observação contínua mesmo durante longos períodos de tempo.

Além disso, a programação dos microcontroladores desempenha um papel fundamental no cálculo de posição e controle do rastreador, bem como na comunicação com dispositivos externos. Nesse caso, um smartphone será usado como a interface de interação do usuário com o equipamento.

## Interação com o Smartphone

O smartphone desempenha um papel crucial na operação do rastreador de astros. Através do dispositivo móvel, os usuários podem enviar comandos e receber dados de rastreio, tornando a experiência de observação mais intuitiva e interativa.

Além disso, a utilização do GPS interno do telefone permite detectar a posição do rastreador, tornando-o ideal para uso em locais próximos e facilitando a configuração inicial do equipamento.

Diversas funcionalidades do smartphone foram implementadas para  o projeto, como:

- Acesso à internet para obter informações de um banco de dados de astros, eliminando a necessidade de criar um banco de dados próprio;
- Recebimento de informações e atualizações em tempo real sobre eventos astronômicos observáveis;
- Uso de APIs para mapas celestes;

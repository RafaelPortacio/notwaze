# Final project

O projeto final é uma aplicação similar ao `Waze` que computa o menor caminho entre dois pontos. O projeto deve apenas considerar a cidade do Rio de Janeiro. Os dados podem ser baixados no site https://www.openstreetmap.org. Para facilitar a obtenção dos dados, é permitido usar a biblioteca `osmnx` como descrito em https://geoffboeing.com/2016/11/osmnx-python-street-networks/.

## Back end
O back-end do projeto deve ser feito em C++. O programa deve computar o menor caminho pelas ruas dado dois pontos e o algoritmo a ser usado: Dijkstra ou A*.

## Front end
O front-end do projeto deve ser um webapp para a interface gráfica. O usuário deve poder escolher dois pontos da cidade e o algoritmo que vai computar o menor caminho. Além disso, deve haver um modo de exibição que mostra os trajetos sendo considerados pelo algoritmo.

A interface deve exibir o mapa das ruas da cidade, os dois pontos extremos, o caminho ótimo e os caminhos sendo considerados (somente quanto pedido).

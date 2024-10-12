breed [leoes leao]
breed [hienas hiena]

turtles-own [energia]
hienas-own [agrupamento]
leoes-own [stay-timer]

globals[tickCounter]

to Setup
  clear-all
  Setup-patches
  Setup-turtles
  reset-ticks
end

to Setup-patches
  clear-all
  set-patch-size 15
  ask patches [set pcolor black]
  ask n-of (256 * (percComidaPP / 100)) patches [set pcolor brown]
  ask n-of (256 * (percComidaGP / 100)) patches [set pcolor red]
  ask n-of numAzuis patches [set pcolor blue]
  set tickCounter 0
end

to Setup-turtles
  clear-turtles
  create-leoes nLeoes
  ask leoes
  [
    set size 1.3
    set shape "wolf 6"
    set color brown + 2
    set energia energia + energiaInicial
    set heading 90
    set stay-timer 0
    setxy random-xcor random-ycor
  ]
  create-hienas nHienas
  ask hienas
  [
    set size 1.2
    set shape "wolf 2"
    set color yellow
    set energia energia + energiaInicial
    set agrupamento 0
    setxy random-xcor random-ycor
  ]
end

to Go
  grow-ComidaGP
  grow-ComidaPP
  moveleoes
  check-agrupamento
  ask hienas
  [
    ask hienas with [agrupamento >= 1]
    [
      moveHienasGrupo
    ]
    ask hienas with [agrupamento = 0]
    [
      moveHienas
    ]
  ]
  energia-check
  if reproduce?
  [
    reproduction
  ]
  if count hienas = 0 or count leoes = 0
  [
    stop
  ]
  counter-ticks
  tick
end

to counter-ticks
  set tickCounter tickCounter + 1
end

to rest
  while[tickCounter - stay-timer < restTimer]
  [
    set stay-timer stay-timer - 1
    rt 90
  ]
  fd 1
end


to moveleoes
  ask hienas[
    let esqHienas (count hienas-on patch-left-and-ahead 90 1)
    let dirHienas (count hienas-on patch-right-and-ahead 90 1)
    let frtHienas (count hienas-on patch-ahead 1)


    ask leoes
    [
      ifelse [pcolor] of patch-ahead 1 = blue
      [
        set stay-timer restTimer
        fd 1
      ]
      [
        ifelse [pcolor] of patch-ahead 1 = red or [pcolor] of patch-ahead 1 = brown
        [
          eat
        ];fecha if comida a frente
        [
          ifelse [pcolor] of patch-left-and-ahead 90 1 = red or
                [pcolor] of patch-left-and-ahead 90 1 = brown
          [
            eat
          ];fecha if comida a esquerda
          [
            ifelse [pcolor] of patch-right-and-ahead 90 1 = red or
                  [pcolor] of patch-right-and-ahead 90 1 = brown
            [
            eat
            ];fecha if comida a direita
            [
              ifelse (any? hienas-on patch-ahead 1 or
                any? hienas-on patch-right-and-ahead 90 1 or
                any? hienas-on patch-left-and-ahead 90 1)
              [
                if ((esqHienas > 1 and dirHienas = 0 and frtHienas = 0) or
                  (esqHienas > 0 and dirHienas = 1 and frtHienas = 0) or
                  (esqHienas > 0 and  dirHienas = 0 and frtHienas = 1)) and
                (energia > energia - ((100 / energiaPerdidaEmCombate) * energia) + 1)
                [
                  let prey one-of hienas-on neighbors4
                  if prey != nobody
                  [
                    ask  prey[
                      die
                    ]
                  ]
                  set energia energia - ((100 / energiaPerdidaEmCombate) * energia) + energiaPorComida
                ];fecha if apenas uma hiena perto
              ];fecha if hienas perto
              [
                if (esqHienas >= 1 and
                  dirHienas >= 1 and
                  frtHienas >= 1)
                [
                  rt 180
                  fd 2
                  set energia energia - 5
                ]
                if dirHienas >= 1 and
                esqHienas >= 0 and
                frtHienas >= 1
                [
                  lt 180
                  fd 1
                  lt 90
                  fd 1
                set energia energia - 5
                ]
                if dirHienas >= 0 and
                esqHienas >= 1 and
                frtHienas >= 1
                [
                  rt 180
                  fd 1
                  rt 90
                  fd 1
                  set energia energia - 5
                ]
                if frtHienas >= 2 or (esqHienas >= 1 and dirHienas >= 1)
                [
                  fd -1
                  set energia energia - 3
                ]
                if dirHienas >= 2 and
                esqHienas = 0 and
                frtHienas = 0
                [
                  lt 90
                fd 1
                  set energia energia - 2
                ]
                if esqHienas >= 2 and
                dirHienas = 0 and
                frtHienas = 0
                [
                  rt 90
                  fd 1
                  set energia energia - 2
                ]
              ];fecha else hienas perto
              ifelse random 101 <= 90
              [
                fd 1
              set energia energia - 1
              ]
              [
                ifelse random 101 <= 50
                [
                  rt 90
                  set energia energia - 1
                ]
                [
                  lt 90
                  set energia energia - 1
                ]
              ]
            ];fecha else comida a direita
          ];fecha else comida a esquerda
        ];fecha else comida a frente
      ]
    ]
  ]
end

to moveHienas

  ask hienas
  [
    set color yellow
    ifelse [pcolor] of patch-ahead 1 = red or [pcolor] of patch-ahead 1 = brown
    [
      eat
    ];fecha if comida a frente
    [
      ifelse [pcolor] of patch-left-and-ahead 90 1 = red or
        [pcolor] of patch-left-and-ahead 90 1 = brown
      [
        eat
      ];fecha if comida a esquerda
      [
        ifelse [pcolor] of patch-right-and-ahead 90 1 = red or
          [pcolor] of patch-right-and-ahead 90 1 = brown
        [
          eat
        ];fecha if comida a direita
        [
          ifelse random 101 <= 90
          [
            fd 1
            set energia energia - 1
          ]
          [
            ifelse random 101 <= 50
            [
              rt 90
              fd 1
              set energia energia - 1
            ]
            [
              rt 90
              fd 1
              set energia energia - 1
            ]
          ]
        ];fecha else comida a direita
      ];fecha else comida a esquerda
    ];fecha else comida a frente
  ];ask hienas
end

to moveHienasGrupo

  ask leoes[
    let esqLeoes (count leoes-on patch-left-and-ahead 90 1)
    let dirLeoes (count leoes-on patch-right-and-ahead 90 1)
    let frtLeoes (count leoes-on patch-ahead 1)


    ask hienas with [agrupamento >= 1]
    [
      set color red

      ifelse [pcolor] of patch-ahead 1 = red or [pcolor] of patch-ahead 1 = brown
      [
        eat
      ];fecha if comida a frente
      [
        ifelse [pcolor] of patch-left-and-ahead 90 1 = red or
        [pcolor] of patch-left-and-ahead 90 1 = brown
        [
          eat
        ];fecha if comida a esquerda
        [
          ifelse [pcolor] of patch-right-and-ahead 90 1 = red or
          [pcolor] of patch-right-and-ahead 90 1 = brown
          [
            eat
          ];fecha if comida a direita
          [
            ifelse any? leoes-on patch-ahead 1 or
            any? leoes-on patch-left-and-ahead 90 1 or
            any? leoes-on patch-right-and-ahead 90 1
            [
              ifelse (esqLeoes + dirLeoes + frtLeoes) = 1
              [
                let victim one-of leoes-on neighbors4
                if victim != nobody
                [
                  ask  victim[
                    die
                  ]
                ]
              ]
              [
                ask hienas-on neighbors4
                [
                  ifelse random 101 <= 50
                  [rt 90]
                  [lt 90]
                  fd 1
                  set energia energia - 1
                ]
                ifelse random 101 <= 50
                [rt 90]
                [lt 90]
                fd 1
                set energia energia - 1
              ]
            ]
            [
              ifelse random 101 <= probHienasAndaremJuntas
              [
                if any? hienas-on neighbors4
                [
                  let x mean [xcor] of hienas-on neighbors4
                  let y mean [ycor] of hienas-on neighbors4
                  facexy x y
                  fd 1
                  set energia energia - 1
                ]
              ]
              [
                ifelse random 101 <= 90
              [
                ask hienas-on neighbors4
                [
                  fd 1
                  set energia energia - 1
                ]
                fd 1
                set energia energia - 1
              ]
              [
                ifelse random 101 <= 50
                [
                  ask hienas-on neighbors4
                  [
                    lt 90
                    fd 1
                    set energia energia - 1
                  ]
                  lt 90
                  fd 1
                  set energia energia - 1
                ]
                [
                  ask hienas-on neighbors4
                  [
                    rt 90
                    fd 1
                    set energia energia - 1
                  ]
                  rt 90
                  fd 1
                  set energia energia - 1
                ]
              ]
              ]
            ]
          ];fecha else comida a direita
        ];fecha else comida a esquerda
      ];fecha else comida a frente
    ]
  ]
end

to check-agrupamento
  ask hienas [
    let neighbors-hienas hienas-on neighbors4
    ifelse any? neighbors-hienas [
      set agrupamento count neighbors-hienas
    ] [
      set agrupamento 0
    ]
  ]
end

to eat
  fd 1
  set energia energia - 1
  if energia <= 100 - energiaPorComida
  [
    set energia energia + energiaPorComida
    ifelse pcolor = red
    [
      set pcolor brown
    ]
    [
      if pcolor = brown
      [
        set pcolor black
      ]
    ]
  ]
end

to energia-check
  ask hienas[
    if energia <= 0
    [
      die
    ]
  ]

  ask leoes[
    if energia <= 0
    [
      die
    ]
  ]
end


to grow-ComidaPP
if (count patches with [pcolor = brown]) < (256 * (percComidaPP / 100))
[
  ask n-of (256 * (percComidaPP / 100)) patches with [pcolor = black]
  [
    set pcolor brown
  ]
]
end


to grow-ComidaGP
  if (count patches with [pcolor = red]) < (256 * (percComidaGP / 100))
  [
    ask n-of (256 * (percComidaGP / 100)) patches with [pcolor = black]
    [
      set pcolor red
    ]
  ]
end


to reproduction
  ask leoes
  [
    if energia > birthEnergy
    [
      if random 101 <= reproductionLeoes
      [
        set energia energia / 2
        hatch NumLeoesNinhada [jump 4]
        set energia energia / 2
      ]
    ]
  ]

  ask hienas
  [
    if energia > birthEnergy
    [
      if random 101 <= reproductionHienas
      [
        set energia energia / 2
        hatch NumHienasNinhada [jump 4]
        set energia energia / 2
      ]
    ]
  ]
end


@#$#@#$#@
GRAPHICS-WINDOW
433
10
936
514
-1
-1
15.0
1
10
1
1
1
0
1
1
1
-16
16
-16
16
0
0
1
ticks
30.0

BUTTON
182
17
245
50
Setup
Setup
NIL
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

BUTTON
275
18
338
51
Go
Go
T
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

SLIDER
179
279
351
312
nLeoes
nLeoes
0
100
20.0
1
1
NIL
HORIZONTAL

SLIDER
179
324
351
357
nHienas
nHienas
0
100
20.0
1
1
NIL
HORIZONTAL

SLIDER
180
101
352
134
percComidaPP
percComidaPP
0
20
20.0
1
1
%
HORIZONTAL

SLIDER
179
147
351
180
percComidaGP
percComidaGP
0
10
10.0
1
1
%
HORIZONTAL

SLIDER
179
189
351
222
numAzuis
numAzuis
0
5
4.0
1
1
NIL
HORIZONTAL

SLIDER
179
232
351
265
energiaPorComida
energiaPorComida
0
50
50.0
1
1
NIL
HORIZONTAL

SLIDER
179
367
351
400
energiaInicial
energiaInicial
0
100
85.0
1
1
NIL
HORIZONTAL

MONITOR
962
23
1068
68
numero de leoes
count leoes
17
1
11

MONITOR
1082
22
1195
67
numero de hienas
count hienas
17
1
11

SLIDER
963
101
1169
134
energiaPerdidaEmCombate
energiaPerdidaEmCombate
1
100
25.0
1
1
%
HORIZONTAL

SLIDER
179
409
351
442
restTimer
restTimer
10
250
250.0
10
1
ticks
HORIZONTAL

SWITCH
964
150
1076
183
reproduce?
reproduce?
0
1
-1000

SLIDER
964
200
1136
233
birthEnergy
birthEnergy
25
50
35.0
1
1
NIL
HORIZONTAL

SLIDER
965
246
1137
279
reproductionHienas
reproductionHienas
0
2
0.5
0.1
1
NIL
HORIZONTAL

SLIDER
965
299
1137
332
reproductionLeoes
reproductionLeoes
0
2
0.5
0.1
1
NIL
HORIZONTAL

SLIDER
966
353
1138
386
NumHienasNinhada
NumHienasNinhada
0
3
2.0
1
1
NIL
HORIZONTAL

SLIDER
966
400
1138
433
NumLeoesNinhada
NumLeoesNinhada
0
3
2.0
1
1
NIL
HORIZONTAL

SLIDER
967
444
1171
477
probHienasAndaremJuntas
probHienasAndaremJuntas
0
100
50.0
1
1
%
HORIZONTAL

@#$#@#$#@
## WHAT IS IT?

(a general understanding of what the model is trying to show or explain)

## HOW IT WORKS

(what rules the agents use to create the overall behavior of the model)

## HOW TO USE IT

(how to use the model, including a description of each of the items in the Interface tab)

## THINGS TO NOTICE

(suggested things for the user to notice while running the model)

## THINGS TO TRY

(suggested things for the user to try to do (move sliders, switches, etc.) with the model)

## EXTENDING THE MODEL

(suggested things to add or change in the Code tab to make the model more complicated, detailed, accurate, etc.)

## NETLOGO FEATURES

(interesting or unusual features of NetLogo that the model uses, particularly in the Code tab; or where workarounds were needed for missing features)

## RELATED MODELS

(models in the NetLogo Models Library and elsewhere which are of related interest)

## CREDITS AND REFERENCES

(a reference to the model's URL on the web if it has one, as well as any other necessary credits, citations, and links)
@#$#@#$#@
default
true
0
Polygon -7500403 true true 150 5 40 250 150 205 260 250

airplane
true
0
Polygon -7500403 true true 150 0 135 15 120 60 120 105 15 165 15 195 120 180 135 240 105 270 120 285 150 270 180 285 210 270 165 240 180 180 285 195 285 165 180 105 180 60 165 15

arrow
true
0
Polygon -7500403 true true 150 0 0 150 105 150 105 293 195 293 195 150 300 150

box
false
0
Polygon -7500403 true true 150 285 285 225 285 75 150 135
Polygon -7500403 true true 150 135 15 75 150 15 285 75
Polygon -7500403 true true 15 75 15 225 150 285 150 135
Line -16777216 false 150 285 150 135
Line -16777216 false 150 135 15 75
Line -16777216 false 150 135 285 75

bug
true
0
Circle -7500403 true true 96 182 108
Circle -7500403 true true 110 127 80
Circle -7500403 true true 110 75 80
Line -7500403 true 150 100 80 30
Line -7500403 true 150 100 220 30

butterfly
true
0
Polygon -7500403 true true 150 165 209 199 225 225 225 255 195 270 165 255 150 240
Polygon -7500403 true true 150 165 89 198 75 225 75 255 105 270 135 255 150 240
Polygon -7500403 true true 139 148 100 105 55 90 25 90 10 105 10 135 25 180 40 195 85 194 139 163
Polygon -7500403 true true 162 150 200 105 245 90 275 90 290 105 290 135 275 180 260 195 215 195 162 165
Polygon -16777216 true false 150 255 135 225 120 150 135 120 150 105 165 120 180 150 165 225
Circle -16777216 true false 135 90 30
Line -16777216 false 150 105 195 60
Line -16777216 false 150 105 105 60

car
false
0
Polygon -7500403 true true 300 180 279 164 261 144 240 135 226 132 213 106 203 84 185 63 159 50 135 50 75 60 0 150 0 165 0 225 300 225 300 180
Circle -16777216 true false 180 180 90
Circle -16777216 true false 30 180 90
Polygon -16777216 true false 162 80 132 78 134 135 209 135 194 105 189 96 180 89
Circle -7500403 true true 47 195 58
Circle -7500403 true true 195 195 58

cat
false
0
Line -7500403 true 285 240 210 240
Line -7500403 true 195 300 165 255
Line -7500403 true 15 240 90 240
Line -7500403 true 285 285 195 240
Line -7500403 true 105 300 135 255
Line -16777216 false 150 270 150 285
Line -16777216 false 15 75 15 120
Polygon -7500403 true true 300 15 285 30 255 30 225 75 195 60 255 15
Polygon -7500403 true true 285 135 210 135 180 150 180 45 285 90
Polygon -7500403 true true 120 45 120 210 180 210 180 45
Polygon -7500403 true true 180 195 165 300 240 285 255 225 285 195
Polygon -7500403 true true 180 225 195 285 165 300 150 300 150 255 165 225
Polygon -7500403 true true 195 195 195 165 225 150 255 135 285 135 285 195
Polygon -7500403 true true 15 135 90 135 120 150 120 45 15 90
Polygon -7500403 true true 120 195 135 300 60 285 45 225 15 195
Polygon -7500403 true true 120 225 105 285 135 300 150 300 150 255 135 225
Polygon -7500403 true true 105 195 105 165 75 150 45 135 15 135 15 195
Polygon -7500403 true true 285 120 270 90 285 15 300 15
Line -7500403 true 15 285 105 240
Polygon -7500403 true true 15 120 30 90 15 15 0 15
Polygon -7500403 true true 0 15 15 30 45 30 75 75 105 60 45 15
Line -16777216 false 164 262 209 262
Line -16777216 false 223 231 208 261
Line -16777216 false 136 262 91 262
Line -16777216 false 77 231 92 261

circle
false
0
Circle -7500403 true true 0 0 300

circle 2
false
0
Circle -7500403 true true 0 0 300
Circle -16777216 true false 30 30 240

cow
false
0
Polygon -7500403 true true 200 193 197 249 179 249 177 196 166 187 140 189 93 191 78 179 72 211 49 209 48 181 37 149 25 120 25 89 45 72 103 84 179 75 198 76 252 64 272 81 293 103 285 121 255 121 242 118 224 167
Polygon -7500403 true true 73 210 86 251 62 249 48 208
Polygon -7500403 true true 25 114 16 195 9 204 23 213 25 200 39 123

cylinder
false
0
Circle -7500403 true true 0 0 300

dot
false
0
Circle -7500403 true true 90 90 120

face happy
false
0
Circle -7500403 true true 8 8 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Polygon -16777216 true false 150 255 90 239 62 213 47 191 67 179 90 203 109 218 150 225 192 218 210 203 227 181 251 194 236 217 212 240

face neutral
false
0
Circle -7500403 true true 8 7 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Rectangle -16777216 true false 60 195 240 225

face sad
false
0
Circle -7500403 true true 8 8 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Polygon -16777216 true false 150 168 90 184 62 210 47 232 67 244 90 220 109 205 150 198 192 205 210 220 227 242 251 229 236 206 212 183

fish
false
0
Polygon -1 true false 44 131 21 87 15 86 0 120 15 150 0 180 13 214 20 212 45 166
Polygon -1 true false 135 195 119 235 95 218 76 210 46 204 60 165
Polygon -1 true false 75 45 83 77 71 103 86 114 166 78 135 60
Polygon -7500403 true true 30 136 151 77 226 81 280 119 292 146 292 160 287 170 270 195 195 210 151 212 30 166
Circle -16777216 true false 215 106 30

flag
false
0
Rectangle -7500403 true true 60 15 75 300
Polygon -7500403 true true 90 150 270 90 90 30
Line -7500403 true 75 135 90 135
Line -7500403 true 75 45 90 45

flower
false
0
Polygon -10899396 true false 135 120 165 165 180 210 180 240 150 300 165 300 195 240 195 195 165 135
Circle -7500403 true true 85 132 38
Circle -7500403 true true 130 147 38
Circle -7500403 true true 192 85 38
Circle -7500403 true true 85 40 38
Circle -7500403 true true 177 40 38
Circle -7500403 true true 177 132 38
Circle -7500403 true true 70 85 38
Circle -7500403 true true 130 25 38
Circle -7500403 true true 96 51 108
Circle -16777216 true false 113 68 74
Polygon -10899396 true false 189 233 219 188 249 173 279 188 234 218
Polygon -10899396 true false 180 255 150 210 105 210 75 240 135 240

house
false
0
Rectangle -7500403 true true 45 120 255 285
Rectangle -16777216 true false 120 210 180 285
Polygon -7500403 true true 15 120 150 15 285 120
Line -16777216 false 30 120 270 120

leaf
false
0
Polygon -7500403 true true 150 210 135 195 120 210 60 210 30 195 60 180 60 165 15 135 30 120 15 105 40 104 45 90 60 90 90 105 105 120 120 120 105 60 120 60 135 30 150 15 165 30 180 60 195 60 180 120 195 120 210 105 240 90 255 90 263 104 285 105 270 120 285 135 240 165 240 180 270 195 240 210 180 210 165 195
Polygon -7500403 true true 135 195 135 240 120 255 105 255 105 285 135 285 165 240 165 195

line
true
0
Line -7500403 true 150 0 150 300

line half
true
0
Line -7500403 true 150 0 150 150

pentagon
false
0
Polygon -7500403 true true 150 15 15 120 60 285 240 285 285 120

person
false
0
Circle -7500403 true true 110 5 80
Polygon -7500403 true true 105 90 120 195 90 285 105 300 135 300 150 225 165 300 195 300 210 285 180 195 195 90
Rectangle -7500403 true true 127 79 172 94
Polygon -7500403 true true 195 90 240 150 225 180 165 105
Polygon -7500403 true true 105 90 60 150 75 180 135 105

plant
false
0
Rectangle -7500403 true true 135 90 165 300
Polygon -7500403 true true 135 255 90 210 45 195 75 255 135 285
Polygon -7500403 true true 165 255 210 210 255 195 225 255 165 285
Polygon -7500403 true true 135 180 90 135 45 120 75 180 135 210
Polygon -7500403 true true 165 180 165 210 225 180 255 120 210 135
Polygon -7500403 true true 135 105 90 60 45 45 75 105 135 135
Polygon -7500403 true true 165 105 165 135 225 105 255 45 210 60
Polygon -7500403 true true 135 90 120 45 150 15 180 45 165 90

sheep
false
15
Circle -1 true true 203 65 88
Circle -1 true true 70 65 162
Circle -1 true true 150 105 120
Polygon -7500403 true false 218 120 240 165 255 165 278 120
Circle -7500403 true false 214 72 67
Rectangle -1 true true 164 223 179 298
Polygon -1 true true 45 285 30 285 30 240 15 195 45 210
Circle -1 true true 3 83 150
Rectangle -1 true true 65 221 80 296
Polygon -1 true true 195 285 210 285 210 240 240 210 195 210
Polygon -7500403 true false 276 85 285 105 302 99 294 83
Polygon -7500403 true false 219 85 210 105 193 99 201 83

square
false
0
Rectangle -7500403 true true 30 30 270 270

square 2
false
0
Rectangle -7500403 true true 30 30 270 270
Rectangle -16777216 true false 60 60 240 240

star
false
0
Polygon -7500403 true true 151 1 185 108 298 108 207 175 242 282 151 216 59 282 94 175 3 108 116 108

target
false
0
Circle -7500403 true true 0 0 300
Circle -16777216 true false 30 30 240
Circle -7500403 true true 60 60 180
Circle -16777216 true false 90 90 120
Circle -7500403 true true 120 120 60

tree
false
0
Circle -7500403 true true 118 3 94
Rectangle -6459832 true false 120 195 180 300
Circle -7500403 true true 65 21 108
Circle -7500403 true true 116 41 127
Circle -7500403 true true 45 90 120
Circle -7500403 true true 104 74 152

triangle
false
0
Polygon -7500403 true true 150 30 15 255 285 255

triangle 2
false
0
Polygon -7500403 true true 150 30 15 255 285 255
Polygon -16777216 true false 151 99 225 223 75 224

truck
false
0
Rectangle -7500403 true true 4 45 195 187
Polygon -7500403 true true 296 193 296 150 259 134 244 104 208 104 207 194
Rectangle -1 true false 195 60 195 105
Polygon -16777216 true false 238 112 252 141 219 141 218 112
Circle -16777216 true false 234 174 42
Rectangle -7500403 true true 181 185 214 194
Circle -16777216 true false 144 174 42
Circle -16777216 true false 24 174 42
Circle -7500403 false true 24 174 42
Circle -7500403 false true 144 174 42
Circle -7500403 false true 234 174 42

turtle
true
0
Polygon -10899396 true false 215 204 240 233 246 254 228 266 215 252 193 210
Polygon -10899396 true false 195 90 225 75 245 75 260 89 269 108 261 124 240 105 225 105 210 105
Polygon -10899396 true false 105 90 75 75 55 75 40 89 31 108 39 124 60 105 75 105 90 105
Polygon -10899396 true false 132 85 134 64 107 51 108 17 150 2 192 18 192 52 169 65 172 87
Polygon -10899396 true false 85 204 60 233 54 254 72 266 85 252 107 210
Polygon -7500403 true true 119 75 179 75 209 101 224 135 220 225 175 261 128 261 81 224 74 135 88 99

wheel
false
0
Circle -7500403 true true 3 3 294
Circle -16777216 true false 30 30 240
Line -7500403 true 150 285 150 15
Line -7500403 true 15 150 285 150
Circle -7500403 true true 120 120 60
Line -7500403 true 216 40 79 269
Line -7500403 true 40 84 269 221
Line -7500403 true 40 216 269 79
Line -7500403 true 84 40 221 269

wolf
false
0
Polygon -16777216 true false 253 133 245 131 245 133
Polygon -7500403 true true 2 194 13 197 30 191 38 193 38 205 20 226 20 257 27 265 38 266 40 260 31 253 31 230 60 206 68 198 75 209 66 228 65 243 82 261 84 268 100 267 103 261 77 239 79 231 100 207 98 196 119 201 143 202 160 195 166 210 172 213 173 238 167 251 160 248 154 265 169 264 178 247 186 240 198 260 200 271 217 271 219 262 207 258 195 230 192 198 210 184 227 164 242 144 259 145 284 151 277 141 293 140 299 134 297 127 273 119 270 105
Polygon -7500403 true true -1 195 14 180 36 166 40 153 53 140 82 131 134 133 159 126 188 115 227 108 236 102 238 98 268 86 269 92 281 87 269 103 269 113

wolf 2
false
0
Rectangle -7500403 true true 195 106 285 150
Rectangle -7500403 true true 195 90 255 105
Polygon -7500403 true true 240 90 217 44 196 90
Polygon -16777216 true false 234 89 218 59 203 89
Rectangle -1 true false 240 93 252 105
Rectangle -16777216 true false 242 96 249 104
Rectangle -16777216 true false 241 125 285 139
Polygon -1 true false 285 125 277 138 269 125
Polygon -1 true false 269 140 262 125 256 140
Rectangle -7500403 true true 45 120 195 195
Rectangle -7500403 true true 45 114 185 120
Rectangle -7500403 true true 165 195 180 270
Rectangle -7500403 true true 60 195 75 270
Polygon -7500403 true true 45 105 15 30 15 75 45 150 60 120

wolf 6
false
0
Polygon -7500403 true true 105 75 105 45 45 0 30 45 45 60 60 90
Polygon -7500403 true true 45 165 30 135 45 120 15 105 60 75 105 60 180 60 240 75 285 105 255 120 270 135 255 165 270 180 255 195 255 210 240 195 195 225 210 255 180 300 120 300 90 255 105 225 60 195 45 210 45 195 30 180
Polygon -16777216 true false 120 300 135 285 120 270 120 255 180 255 180 270 165 285 180 300
Polygon -7500403 true true 195 75 195 45 255 0 270 45 255 60 240 90
Polygon -16777216 true false 225 75 210 60 210 45 255 15 255 45 225 60
Polygon -16777216 true false 75 75 90 60 90 45 45 15 45 45 75 60
Circle -16777216 true false 88 118 32
Circle -16777216 true false 178 118 32

x
false
0
Polygon -7500403 true true 270 75 225 30 30 225 75 270
Polygon -7500403 true true 30 75 75 30 270 225 225 270
@#$#@#$#@
NetLogo 6.3.0
@#$#@#$#@
@#$#@#$#@
@#$#@#$#@
@#$#@#$#@
VIEW
210
10
647
448
0
0
0
1
1
1
1
1
0
1
1
1
-16
16
-16
16

BUTTON
32
49
95
82
Setup
NIL
NIL
1
T
OBSERVER
NIL
NIL

BUTTON
132
62
195
95
Go
NIL
NIL
1
T
OBSERVER
NIL
NIL

@#$#@#$#@
default
0.0
-0.2 0 0.0 1.0
0.0 1 1.0 0.0
0.2 0 0.0 1.0
link direction
true
0
Line -7500403 true 150 150 90 180
Line -7500403 true 150 150 210 180
@#$#@#$#@
0
@#$#@#$#@

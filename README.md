# Russiablock
This is a Russia block application on windows console.

## build
`$ g++ box.cpp -std=c++11 -o box.exe`

## play
if you really want play this simple game

- w for rotate a block
- a for move left
- d for move right
- s to try fall down

## others
there is currently a line under the board say what the block do just before, and because of something inside it may be strange. but it do nothing to the game.

## future plan
there are not much thing to add to the simple code.
1. score and next block prompt. put it right of the board is rather unconfortable. and put it down make the board too long.
2. different level of game. higher level influence fall down speed of the block.
3. play hint of the game in above [play](#play).
4. start and pause operation. I insist that a russia dont need pause, it is easy to be used as cheat way. but it does no harm to me.

there are also other fantancy ideas that currently not in implementation list.
1. use something instead of console.
2. fall down hint of a block. this is used to know early what influence the block fall down will make.
3. continue game. if you just lose one game, you can continue without restart the program.
4. score challenge. save highest score since the program start. also a database can be used to record more time.

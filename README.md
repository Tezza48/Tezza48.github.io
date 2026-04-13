# Website Generator

Typically i just compile and build with `cc main.c -o main && ./main`

Outputs the website to ./dist

For hot reloads use something like nodemon `npx nodemon --exec "cc main.c -o main  && ./main" ./main.c`

I'm also serving it with `npx http-server ./dist`

# Website Generator

Typically i just compile and build with `cc website.c -o website && ./website`

Outputs the website to ./dist

For hot reloads use something like nodemon `npx nodemon --exec "cc website.c -o website  && ./website" ./website.c`

I'm also serving it with `npx http-server ./dist`

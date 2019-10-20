
'use strict';

const express = require('express');
const app = express();


function LoggerMiddleware(req, res, next) {
    const d = new Date();
    const n = d.getTime();
    console.log(n);
    next();
}

app.use(LoggerMiddleware);

app.get('/', (req, res) => {
    res.send('hello world!');
})

const port = 4000;
app.listen(port, () => console.log(`Example app listening on port ${port}!`));

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


app.route('/t').get((req, res)=>{
    console.log('aasdfasf');
})

app.get('/a', (req, res) => {
    res.send('a');
})

app.get('/b', (req, res) => {
    res.send('b');
})

var router = express.Router()

app.use('/subroute', router);

router.get('/a', (req, res) => {
    res.send('a');
})

router.get('/b', (req, res) => {
    res.send('b');
})

const port = 8081;
app.listen(port, () => console.log(`Example app listening on port ${port}!`));
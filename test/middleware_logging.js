const express = require('express');
const fetch = require('node-fetch');

const app = express();

let route_called = false;
const message = "hello world";

function LoggerMiddleware(req, res, next) {
  const d = new Date();
  const n = d.getTime();
  console.log(`unix time ${n}`);
  next();
}

app.use(LoggerMiddleware);

app.route('/a').get((req, res) => {
  route_called = true;
  res.send(message);
});

const port = 8081;
app.listen(port, () => {
  console.log(`Example app listening on port ${port}!`);
  fetch(`http://localhost:${port}/a`)
    .then(async (r) => {
      console.log(await r.text());
      console.log('finished');
    })
    .catch((e) => {
      console.error('ERROR: ' + e)
    });
});
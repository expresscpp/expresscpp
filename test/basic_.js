const express = require('express');
const app = express();
const fetch = require('node-fetch');
const assert = require('assert');

app.get("/", (req, res) => {
  res.send("/")
})

const port = 8082;
const server = app.listen(port, () => {
  fetch(`http://localhost:${port}/`)
    .then(async (r) => {
      const response = await r.text();
      assert(response === "/");
      server.close();
    })
    .catch((e) => {
      console.error('ERROR: ' + e)
      assert(false);
    });
});
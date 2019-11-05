const express = require('express');
const app = express();
const fetch = require('node-fetch');


app.get("/", (req, res) => {
  res.send("get")
})

app.post("/", (req, res) => {
  res.send("post")
})
app.delete("/", (req, res) => {
  res.send("delete")
})
app.patch("/", (req, res) => {
  res.send("patch")
})

const port = 8081;
app.listen(port, () => {
  console.log(`Example app listening on port ${port}!`)
  fetch(`http://localhost:${port}/a`)
    .then(async (r) => {
      console.log(await r.text());
      console.log('finished');
    })
    .catch((e) => {
      console.error('ERROR: ' + e)
    });
});
const express = require('express');
const app = express();
const assert = require('assert');

app.get("/", (req, res) => {
  res.send("")
});

console.log(app._router.stack)

assert(app._router.stack[2].route.path === "")
// assert(app._router.stack[3].route.path === "/a")

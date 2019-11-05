const express = require('express');
const app = express();
const assert = require('assert');

app.get("", (req, res) => {
  res.send("")
});
app.get("/a", (req, res) => {
  res.send("/a")
});
app.post("/a", (req, res) => {
  res.send("/a")
});
 
 
console.log(app._router.stack)

assert(app._router.stack[2].route.path === "")
assert(app._router.stack[3].route.path === "/a")

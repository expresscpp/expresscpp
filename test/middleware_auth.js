const express = require('express');
const app = express();

function AuthMiddleware(req, res, next) {
  if(req.headers["Authorization"] === "secret_token") {
    next();
    return;
  }
  res.status(401).send("Access Denied")
}

app.use(AuthMiddleware);
app.get("/secret", (req, res)=>{
  res.send("secret stuff")
})

const port = 8081;
app.listen(port, () => console.log(`Example app listening on port ${port}!`));
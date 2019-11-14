# Testing

## stress tests

```bash
sudo apt install apache2-utils
ab -kc 20 -t 60 http://localhost:8081/index.html
# flood test
ab -n 5000 -c 200 http://localhost:8081/index.html
```

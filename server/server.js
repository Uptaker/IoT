const express = require("express");
const app = express();
const port = process.env.PORT || 3000;

let number = 0

app.use('/', express.static('public'))

app.get(["/status"], (req, res) => {
    res.send(`${number}`)
});

app.post(["/status/:num"], (req, res) => {
    const newNum = req.params['num']
    if (!newNum) res.status(404)
    else {
        number = newNum
        res.status(200)
    }
    res.send('ok')
});

app.listen(port, () => console.log(`Node Server listening on port ${port}!`));

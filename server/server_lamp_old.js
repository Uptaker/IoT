const express = require("express");
const app = express();
const port = process.env.PORT || 3000;

let isLampOn = false

app.use('/', express.static('public'))

app.get(["/status"], (req, res) => {
    res.send(`${isLampOn ? 'on' : 'off'}`)
});

app.post(["/lamp/:status"], (req, res) => {
    const status = req.params.status
    if (!status) res.status(404)
    else if (status == 'on') {
        isLampOn = true
        res.status(200)
    }
    else if (status == 'off') {
        isLampOn = false;
        res.status(200)
    }
    res.send('ok')
});

app.listen(port, () => console.log(`Node Server listening on port ${port}!`));

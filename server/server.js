import express from 'express'

const app = express()
const port = process.env.PORT || 3000

let value = 0

app.use('/', express.static('public'))

app.get(["/status"], (req, res) => {
    res.send(`${value}`)
});

app.post(["/status/:value"], (req, res) => {
    const newValue = req.params['value']
    if (!newValue) res.status(404)
    else {
        value = newValue
        res.status(200)
    }
    res.send('ok')
});

app.listen(port, () => console.log(`Node Server listening on port ${port}!`));

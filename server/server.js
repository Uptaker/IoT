import express from 'express'

const app = express()
const port = process.env.PORT || 3000

let value = 0

app.use('/', express.static('public'))

app.get('/status', (req, res) => {
    console.log(`[HTTP /status - ${new Date(Date.now()).toLocaleString()}] ${req.protocol}://${req.hostname}${req.url}`)
    const newValue = req.query['value']
    if (newValue && !isNaN(newValue)) value = parseInt(newValue)
    res.status(200).send(`${value}`)
});

app.listen(port, () => console.log(`Node Server listening on port ${port}!`));

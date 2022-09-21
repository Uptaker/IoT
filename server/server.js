import express from 'express'

const app = express()
const port = process.env.PORT || 3000

let value = 0

// "24H time": "when to switch off, 0 means always on during this hour"
let limits =  resetLimit()

function resetLimit() {
    let limit = {}
    for (let i = 0; i < 24; i++) limit[i] = 0
    return limit
}

function validateLimits(newLimits) {
    if (!newLimits) return false
    for (let i = 0; i < 24; i++) {
        if (isNaN(newLimits[i])) return false
    }
    return true
}

function shouldBeOn(value) {
    const currentHour = new Date().getHours()
    let currentLimit = limits[currentHour]
    if (currentLimit === 0) return true
    else return value < currentLimit;
}

app.use(express.json())
app.use('/', express.static('public'))

app.get('/status', (req, res) => {
    console.log(`[HTTP /status - ${new Date(Date.now()).toLocaleString()}] ${req.protocol}://${req.hostname}${req.url}`)
    const newValue = req.query['value']
    if (newValue && !isNaN(newValue)) value = parseInt(newValue)
    res.status(200).send(newValue ? shouldBeOn(value) : value.toString())
});

app.post('/limits', (req, res) => {
    console.log(`[HTTP /limits - ${new Date(Date.now()).toLocaleString()}] ${req.protocol}://${req.hostname}${req.url}`)
    const body = req.body
    console.log('bodyBefore', body)
    if (validateLimits(body)) {
        console.log('body', body)
        limits = body
        console.log('New limits set', limits)
        return res.sendStatus(200)
    } else {
        console.log('Could not validate new limits')
        return res.sendStatus(400)
    }
})

app.get('/limits', (req, res) => {
    console.log(`[HTTP /limits - ${new Date(Date.now()).toLocaleString()}] ${req.protocol}://${req.hostname}${req.url}`)
    res.status(200).send(limits)
})

app.listen(port, () => console.log(`Node Server listening on port ${port}!`));

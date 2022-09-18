document.querySelector("#submit").addEventListener('click', postLamp)

async function postLamp() {
    const form = document.querySelector("#form")
    const toggle = form.elements.toggle.value
    await fetch(`/lamp/${toggle === 'on' ? 'on' : 'off'}`, {method: 'POST'})
    console.log('request')
}
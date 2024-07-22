var nativeAddon = require("./")

function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms))
}

void async function() {
    while (true) {
        nativeAddon.allocateMemoryBlock()
        await sleep(1000)
    }
}()
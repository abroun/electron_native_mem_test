const { ipcRenderer } = require("electron/renderer")

window.addEventListener("DOMContentLoaded", () => {

    const btnAllocateNativeClickHandler = async () => {
        const allocatedData = await ipcRenderer.invoke("allocateMemoryNative")

        let infoDiv = document.getElementById("memoryInfoNative")
        infoDiv.innerHTML = `<br>Allocated 1GB block. Now ${allocatedData.numBlocks} blocks<br>`
            + `Mem MB - Total: ${allocatedData.totalNumMB.toFixed(2)} Free: ${allocatedData.freeNumMB.toFixed(2)} Used: ${allocatedData.usedNumMB.toFixed(2)}`
    }

    const btnAllocateMainClickHandler = async () => {
        const allocatedData = await ipcRenderer.invoke("allocateMemoryMain")

        let infoDiv = document.getElementById("memoryInfoMain")
        infoDiv.innerHTML = `<br>Allocated 1GB block. Now ${allocatedData.numBlocks} blocks<br>`
            + `Mem MB - Total: ${allocatedData.totalNumMB.toFixed(2)} Free: ${allocatedData.freeNumMB.toFixed(2)} Used: ${allocatedData.usedNumMB.toFixed(2)}`
    }

    document.getElementById("btnAllocateNative").addEventListener("click", btnAllocateNativeClickHandler)
    document.getElementById("btnAllocateMain").addEventListener("click", btnAllocateMainClickHandler)
})